#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "msg.h"
#include "thread.h"
#include "fmt.h"

#include "cpu.h"
#include "board.h"

// DHT sensor
#include "dht_params.h"
#include "dht.h"

// Seesaw sensor
#include "seesaw_soil.h"
#include "seesaw_soil_params.h"
#include "seesaw_soil_regs.h"

// display
#include "u8g2.h"
#include "u8x8_riotos.h"

#include "periph/gpio.h"
#include "periph/i2c.h"

#include "timex.h"
#include "xtimer.h"
#include "ztimer.h"


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


#define THRESHOLD_TEMPERATURE 25  // CELCIUS DEGREE
#define THRESHOLD_HUMIDITY 30  // IN PERCENTAGE 
#define IRRIGATION_TIME 5 // TIME IN SECONDS

#define TEMP_PERIOD 2 // IN REALITY IT SHOULD BE 2 HOURS = 2 * 60 * 60 = 7 200 SECONDS  

// ! VARIABLE PERIOD
int soil_period = 6;  // 6 HOURS = 21 600 SECONDS  

// to check if there are sending messages and which is they status
u_int8_t sending_messages = SEMTECH_LORAMAC_TX_DONE;

// LoRa
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

void* read_temperature_humidity(void* arg);
void* read_soil(void* arg);
void irrigate(void);
void write_display(char* message);
int init_components(void);

// function for LoRaWAN
static int loramac_init(void);
static void send_message(char* message);


//Definition of Array used as Stack by threads
static char temperature_stack[THREAD_STACKSIZE_DEFAULT];
static char soil_stack[THREAD_STACKSIZE_DEFAULT];

//dht22 sensor
//dht_t dev;
/*dht_params_t dht_params =
{
    .pin = GPIO_PIN(PORT_A,9); //D8
    .in_mode = DHT_PARAM_PULL;
};*/


//soil sensor
//seesaw_soil_t dev;

//relay water pump
//gpio_t pin_relay = GPIO_PIN(PORT_B, 13); //D13

//display
//u8g2_t u8g2;
/*u8x8_riotos_t user_data =
{
    .device_index = TEST_I2C,
    .pin_cs = TEST_PIN_CS,
    .pin_dc = TEST_PIN_DC,
    .pin_reset = TEST_PIN_RESET,
};*/

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

void* read_temperature_humidity(void* arg){ //dht22 sensor
    (void)arg;
    /*int16_t temp, hum;

    if (dht_read(&dev, &temp, &hum) != DHT_OK) {
        printf("Error reading values\n");
    }

    //Use of fmt module to format the 16-bit intgers returned by read action in string

    char temp_s[10];
    size_t n = fmt_s16_dfp(temp_s, temp, -1);
    temp_s[n] = '\0';

    char hum_s[10];
    size_t c = fmt_s16_dfp(hum_s, hum, -1);
    hum_s[c] = '\0';

    // THE LOGIC THAT MODIFIES THE SOIL PERIOD
    if( temp < THRESHOLD_TEMPERATURE){
        soil_period = 12;
    }
    else{
        soil_period = 6;
    }

    // todo send messages to LoraWan
    // todo display the read values /// DONE 
    printf("DHT values - temp: %s°C - relative humidity: %s%%\n",temp_s, hum_s);
    
 */
    char *message = "soil";
    send_message(message);


    return NULL;
}

void* read_soil(void* arg){ //soil sensor
    (void)arg;
    /*size_t len;
    int16_t temp;
    uint16_t moist;
    char tstr[8];
    char mstr[8];
    seesaw_soil_temperature (&dev, &temp);
    seesaw_soil_moisture(&dev, &moist);

    len = fmt_s16_dfp(tstr, temp, -2);
    tstr[len] = '\0';

    len = fmt_u16_dec(mstr, moist);
    mstr[len] = '\0';

    // the logic that activates the relay
    if (moist < THRESHOLD_HUMIDITY ) irrigate();

    // todo send messages to LoraWan
    // todo display the read values

    printf("Reading: T: %s °C  Moist: %s\n", tstr, mstr);
    xtimer_sleep(5);*/

    char *message = "temp";
    send_message(message);

    return NULL;
}

void irrigate(void){ //relay water pump
   /* printf("Turn-off water pump\n");
    gpio_clear(pin_relay);
    xtimer_sleep(IRRIGATION_TIME);
    printf("Turn-off water pump\n");
    gpio_set(pin_relay);*/

}


void write_display(char* message){ //Display

    /*
    u8g2_FirstPage(&u8g2);

    do {
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_SetFont(&u8g2, u8g2_font_helvB12_tf);
        u8g2_DrawStr(&u8g2, 12, 22, message);
    } while (u8g2_NextPage(&u8g2));
*/
    printf("%s",message);
}


static int loramac_init(void){
    
    puts("LoRaWAN Class A low-power application");
    puts("=====================================");

    /* Convert identifiers and application key */
    fmt_hex_bytes(deveui, CONFIG_LORAMAC_DEV_EUI_DEFAULT);
    fmt_hex_bytes(appeui, CONFIG_LORAMAC_APP_EUI_DEFAULT);
    fmt_hex_bytes(appkey, CONFIG_LORAMAC_APP_KEY_DEFAULT);

    /* Initialize the radio driver */
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

    /* Initialize the loramac stack */
    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);


    /* Use a fast datarate, e.g. BW125/SF7 in EU868 */
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    /* Start the Over-The-Air Activation (OTAA) procedure to retrieve the
     * generated device address and to get the network and application session
     * keys.
     */
    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }
    puts("Join procedure succeeded");

    return 0;
}


static void send_message(char* message){
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
        xtimer_sleep(5);
    }
}

int init_components(void){ //initialize all components

//dht22 sensor inizialization
    /*if (dht_init(&dev, &dht_params) == DHT_OK){
        printf("DHT22 sensor connected\n");
    }
    else {
        printf("Failed to connect to DHT sensor\n");
    return 1;
    }
//soil sensor inizialization
    int test = 0;    
    test = seesaw_soil_init(&dev, &seesaw_soil_params[0]);
    if (test == SEESAW_SOIL_OK){
        puts("soil sensor connected\n");
    }
    else{
        puts("Failed to connect to soil sensor\n");
    }
//relay inizialization
    if (gpio_init(pin_out, GPIO_OUT)) {
        printf("Error to initialize GPIO_PIN(%d %d)\n", PORT_B, 13);
    return -1;
    }
//display inizialization
    TEST_DISPLAY(&u8g2, U8G2_R0, u8x8_byte_hw_i2c_riotos, u8x8_gpio_and_delay_riotos);
    u8g2_SetUserPtr(&u8g2, &user_data);
    u8g2_SetI2CAddress(&u8g2, TEST_ADDR);
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    */

    
    if (loramac_init() == 1){
        printf("Failed to init the LoRa\n");
        return 1;
    }
    return 0;
}

int main(void){
    
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

    
    return 0;
}