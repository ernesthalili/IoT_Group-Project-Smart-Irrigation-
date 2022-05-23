#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "msg.h"
#include "cpu.h"
#include "board.h"

//Importing General Purpose Libraries
#include "xtimer.h"
#include "thread.h"
#include "ztimer.h"

//Importing Libraries for DHT22 Sensor
#include "fmt.h"
#include "dht.h"
#include "dht_params.h"

//Importing Libraries for Soil Moisture Sensor
#include "seesaw_soil.h"
#include "seesaw_soil_params.h"
#include "seesaw_soil_regs.h"

//Importing Libraries for Relay
#include "periph/gpio.h"
#include "periph/spi.h"

// Importing Libraries for display
#include "ucg.h"
#include "ucg_riotos.h"
#include "logo.h"

#include "main.h"


#if IS_USED(MODULE_PERIPH_RTC)
#include "periph/rtc.h"
#endif

#include "net/loramac.h"
#include "semtech_loramac.h"

#if IS_USED(MODULE_SX127X)
#include "sx127x.h"
#include "sx127x_netdev.h"
#include "sx127x_params.h"
#endif

#if IS_USED(MODULE_SX126X)
#include "sx126x.h"
#include "sx126x_netdev.h"
#include "sx126x_params.h"
#endif

// sleeping time for the components
int dht_sleeping_time = 20;  // 2 hours = 2*60*60 = 7 200 seconds 
int seesaw_sleeping_time = 20; // 6 hours  = 6*60*60 = 21 600 seconds
int display_sleeping_time = 10; // 1 minute = 60 seconds
int pump_irrigation_time = 1; // 1 second (activated by an event)

// danger level for soil moisture
int soil_danger_level = 600;

// flag to guarantee the Mutual Exclusion on sending messages
u_int8_t sending_messages = SEMTECH_LORAMAC_TX_DONE;

// flag to avoid writing on display while irrigating
bool irrigating = false;

static semtech_loramac_t loramac;
#if IS_USED(MODULE_SX127X)
static sx127x_t sx127x;
#endif
#if IS_USED(MODULE_SX126X)
static sx126x_t sx126x;
#endif
#if !IS_USED(MODULE_PERIPH_RTC)
static ztimer_t timer;
#endif


//Definition of Array used as Stack by threads
static char temperature_stack[THREAD_STACKSIZE_DEFAULT];
static char soil_stack[THREAD_STACKSIZE_DEFAULT];
static char display_stack[THREAD_STACKSIZE_DEFAULT];

// string to show at display
u_int16_t env_temp_display, env_hum_display, soil_temp_display, soil_hum_display;

//Definitons the Pins for Relay and dht22, soil sensor and display
dht_t dht_device;
seesaw_soil_t seesaw_device;
ucg_t ucg;

gpio_t pin_out = GPIO_PIN(PORT_B,5);
static ucg_riotos_t user_data =
{
    .device_index = SPI_DEV(0),
    .pin_cs = GPIO_PIN(PORT_B, 6),
    .pin_cd = GPIO_PIN(PORT_B, 12),  
    .pin_reset = GPIO_PIN(PORT_A, 9),
};

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

void* read_temperature_humidity(void* arg){ //dht22 sensor
    (void) arg;
    int16_t temp, hum;
    char temp_s[10]="25";  // value created for testing purposes
    char hum_s[10];

    while(1){/*
        if(dht_read(&dht_device, &temp, &hum) != DHT_OK){
            printf("Error reading temperature values\n");
            continue;
        }
        else{
            size_t n = fmt_s16_dfp(temp_s, temp, -1);
            temp_s[n] = '\0';

            size_t c = fmt_s16_dfp(hum_s, hum, -1);
            hum_s[c] = '\0';
   
            // print the values
            printf("DHT values - temp: %s°C - relative humidity: %s\n",temp_s, hum_s);
        }

        if(temp > temperature_danger_level )  {
            seesaw_sleeping_time = 21;
        }
        else {
            seesaw_sleeping_time = 42;
        }
    
        // change values for display
        env_temp_display = temp;
        env_hum_display  = hum;
*/
        
        char final[] = "T";
        strcat(final,temp_s);
        char* message = final;
        
        //printf("sending: %s\n",message);

        send_message(message);
        xtimer_sleep(dht_sleeping_time); //sleeping
    
    }


    return NULL;
}

void* read_soil(void* arg){ //soil sensor
    (void)arg;
    uint16_t moist=100 ;
    uint16_t temp;

    while (1){/*
        if(seesaw_soil_moisture(&seesaw_device, &moist) == SEESAW_SOIL_BUSERR || seesaw_soil_moisture(&seesaw_device, &temp) == SEESAW_SOIL_BUSERR){
            printf("Error on reading the soil sensor\n");
            continue;
        }
        else {
            printf("moist: %d\n", moist);
        }
        

        // check whether to irrigate 
        if (moist < soil_danger_level ){
            irrigate(pump_irrigation_time);
        }

        // change values for display
        soil_hum_display = moist;
        soil_temp_display = temp;
*/

        //  code for testing with created values
        moist++;
        char info[5];
        sprintf(info, "%d", moist);
        //----------------------------


        char final[] = "S";
        strcat(final,info);
        char* message = final;

        //printf("sending: %s\n",message);

        send_message(message);
        xtimer_sleep(seesaw_sleeping_time); //sleeping time
    }

    return NULL;
}

void *display_logic(void *arg){
    (void)arg;
    /*
    while(1){
        // add a control to avoid the display writing values while irrigating
        if(irrigating) {
            xtimer_sleep(pump_irrigation_time); // sleep for the time we are irrigating
            continue;
        }
        //print on display 
        ucg_ClearScreen(&ucg);
        for (int y = 0; y < 48; y++) {
            for (int x = 0; x < 96; x++) {
                uint32_t offset = (x + (y * 96)) * 3;
                ucg_SetColor(&ucg, 0, logo[offset + 0], logo[offset + 1],logo[offset + 2]);
                ucg_DrawPixel(&ucg, x, y);
            }
        }

        ucg_SetColor(&ucg, 0, 0, 255, 0);
        ucg_DrawString(&ucg,120,40,0,"Smart Irrigation");
        ucg_SetColor(&ucg, 0, 255, 0, 0);

        char temp_s[10];
        size_t n = fmt_s16_dfp(temp_s, env_temp_display, -1);
            temp_s[n] = '\0';

        char hum_s[10];
        size_t c = fmt_s16_dfp(hum_s, env_hum_display, -1);
        hum_s[c] = '\0';

            
        char tstr[8];
        int len = fmt_s16_dfp(tstr, soil_temp_display, -2);
        tstr[len] = '\0';

        char mstr[8];
        len = fmt_u16_dec(mstr, soil_hum_display);
        mstr[len] = '\0';

        char message[20]="";

        strcpy(message,"Env temperature:");
        strcat(message,temp_s);
        ucg_DrawString(&ucg,8,90,0,message);

        strcpy(message,"Env Humidity:");
        strcat(message,hum_s);
        ucg_DrawString(&ucg,8,120,0,message);

        strcpy(message,"Soil Temperature:");
        strcat(message,tstr);
        ucg_DrawString(&ucg,8,150,0,message);

        strcpy(message,"Soil Humidity:");
        strcat(message,mstr);
        ucg_DrawString(&ucg,8,180,0,message);
    
        
        xtimer_sleep(display_sleeping_time); //sleeping time
    }
*/
    return NULL;
}

void irrigate(int irrigate){ //relay water pump
    /*irrigating = true;
    gpio_clear(pin_out);
    printf("Turn-on water pump\n");

    xtimer_sleep(irrigate);

    gpio_set(pin_out);
    irrigating = false;
    printf("Turn-off water pump\n");

    printf("Irrigated for %d seconds\n",pump_irrigation_time);

    char info[5];
    sprintf(info, "%d", pump_irrigation_time);
    char final[] = "I";
    strcat(final,info);
    char* message = final;
    
    send_message(message);*/
    
}



static void send_message(char* message)
{
    
    while(1){

        if(sending_messages == SEMTECH_LORAMAC_TX_DONE){
            sending_messages = 0;
            printf("Sending: %s\n", message);
            sending_messages = semtech_loramac_send(&loramac,(uint8_t *)message, strlen(message)); 
            if (sending_messages != SEMTECH_LORAMAC_TX_DONE)  {
                printf("Cannot send message '%s', ret code: %d\n", message, sending_messages);
                return;
            }
            break;
        }
        xtimer_sleep(1);
    }
    
}


static int loramac_init(void){
    
    puts("LoRaWAN Class A low-power application");
    puts("=====================================");

    // Convert identifiers and application key
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

    // Initialize the radio driver 
#if IS_USED(MODULE_SX127X)
    sx127x_setup(&sx127x, &sx127x_params[0], 0);
    loramac.netdev = &sx127x.netdev;
    loramac.netdev->driver = &sx127x_driver;
#endif

#if IS_USED(MODULE_SX126X)
    sx126x_setup(&sx126x, &sx126x_params[0], 0);
    loramac.netdev = &sx126x.netdev;
    loramac.netdev->driver = &sx126x_driver;
#endif

    // Initialize the loramac stack 
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);


    // Use a fast datarate, e.g. BW125/SF7 in EU868 
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    // Start the Over-The-Air Activation (OTAA) procedure to retrieve the
    // generated device address and to get the network and application session keys.
     
    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }
    puts("Join procedure succeeded");

    return 0;
    
}

int init_components(void){ //initialize all components

    /*
    // relay init
    if (gpio_init(pin_out, GPIO_OUT)) {
        printf("Error to initialize GPIO_PIN(%d %d)\n", PORT_B, 5);
        return -1;
    }
    puts("Relay's Pin Initialized\n");
    gpio_set(pin_out);

    // display init
    ucg_SetUserPtr(&ucg, &user_data);
    ucg_Init(&ucg, ucg_dev_ili9341_18x240x320, ucg_ext_ili9341_18,ucg_com_hw_spi_riotos);

    ucg_SetRotate270(&ucg);
    ucg_SetFontMode(&ucg, UCG_FONT_MODE_SOLID);
    ucg_SetFont(&ucg, ucg_font_helvR18_tf);

    // Dht init
    dht_params_t dht_params;
    dht_params.pin = GPIO_PIN(PORT_A,8);
    dht_params.in_mode = DHT_PARAM_PULL;
    if (dht_init(&dht_device, &dht_params) != DHT_OK){
        printf("Failed to initialize to DHT sensor\n");
        return -1;
    }
    printf("DHT Sensor Initialized\n");

    // Seesaw init
    if (seesaw_soil_init(&seesaw_device, &seesaw_soil_params[0]) != SEESAW_SOIL_OK){
        printf("Failed to initialize soil sensor\n");
        return -1;
    }
    printf("Soil Moisture Initialized\n");
*/
    if (loramac_init() == 1){
        printf("Failed to init the LoRa\n");
        return 1;
    }
    printf("LoRa initialized\n");
    return 0;
    
}

int main(void)
{

    puts("Smart Irrigation System");
    puts("=====================================");
    if(init_components() == 1){
        printf("Failed to Initialize Components\n");
        return 1;
    }

    //Starting Thread that handles temperature and humidity measurement
    thread_create(temperature_stack,sizeof(temperature_stack),THREAD_PRIORITY_MAIN -1, 0,read_temperature_humidity, NULL, "temp_sensor_logic");
    puts("Temperature Thread Started");

    //Starting Thread that handles soil moisture measurement
    thread_create(soil_stack,sizeof(soil_stack),THREAD_PRIORITY_MAIN -1, 0,read_soil, NULL, "soil_sensor_logic");
    puts("Soil Thread Started");


    //Starting Thread that handles display
    thread_create(display_stack,sizeof(display_stack),THREAD_PRIORITY_MAIN -1, 0,display_logic, NULL, "display_logic");
    printf("Display Thread Started\n");


    return 0;
}
