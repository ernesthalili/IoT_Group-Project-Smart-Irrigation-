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


//#include "main.h"


#if IS_USED(MODULE_PERIPH_RTC)
#include "periph/rtc.h"
#else
#include "timex.h"
#include "ztimer.h"
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

/* Messages are sent every 20s to respect the duty cycle on each channel */
#define PERIOD_S            (20U)

#define SENDER_PRIO         (THREAD_PRIORITY_MAIN - 1)
static kernel_pid_t sender_pid;
static char sender_stack[THREAD_STACKSIZE_MAIN / 2];


void read_temperature_humidity(void);
void irrigate(void);
void read_soil(void);
void write_display(char* message);
int init_components(void);

// function for LoRaWAN
int loramac_init(void);
int send_message(char* message);


//Definition of Array used as Stack by threads
//static char stack[THREAD_STACKSIZE_DEFAULT];
//static char temperature_stack[THREAD_STACKSIZE_DEFAULT];
//static char soil_stack[THREAD_STACKSIZE_DEFAULT];

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


//display pin SDA D14 and SCK D15
#define TEST_OUTPUT_I2C 4
//u8g2_t u8g2;
/*u8x8_riotos_t user_data =
{
    .device_index = TEST_I2C,
    .pin_cs = TEST_PIN_CS,
    .pin_dc = TEST_PIN_DC,
    .pin_reset = TEST_PIN_RESET,
};*/

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

static const char* message = "This is Smart Irrigation";

static uint8_t deveui[LORAMAC_DEVEUI_LEN];
static uint8_t appeui[LORAMAC_APPEUI_LEN];
static uint8_t appkey[LORAMAC_APPKEY_LEN];

static void _alarm_cb(void *arg)
{
    (void) arg;
    msg_t msg;
    msg_send(&msg, sender_pid);
}

static void _prepare_next_alarm(void)
{
#if IS_USED(MODULE_PERIPH_RTC)
    struct tm time;
    rtc_get_time(&time);
    /* set initial alarm */
    time.tm_sec += PERIOD_S;
    mktime(&time);
    rtc_set_alarm(&time, _alarm_cb, NULL);
#else
    timer.callback = _alarm_cb;
    ztimer_set(ZTIMER_MSEC, &timer, PERIOD_S * MS_PER_SEC);
#endif
}

static void _send_message(void)
{
    printf("Sending: %s\n", message);
    /* Try to send the message */
    uint8_t ret = semtech_loramac_send(&loramac,
                                       (uint8_t *)message, strlen(message));
    if (ret != SEMTECH_LORAMAC_TX_DONE)  {
        printf("Cannot send message '%s', ret code: %d\n", message, ret);
        return;
    }
}

static void *sender(void *arg)
{
    (void)arg;

    msg_t msg;
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    while (1) {
        msg_receive(&msg);

        /* Trigger the message send */
        _send_message();

        /* Schedule the next wake-up alarm */
        _prepare_next_alarm();
    }

    /* this should never be reached */
    return NULL;
}


void read_temperature_humidity(void){ //dht22 sensor
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
}

void irrigate(void){ //relay water pump
   /* printf("Turn-off water pump\n");
    gpio_clear(pin_relay);
    xtimer_sleep(IRRIGATION_TIME);
    printf("Turn-off water pump\n");
    gpio_set(pin_relay);*/

}


void read_soil(void){ //soil sensor
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


int loramac_init(void){
    
    return 0;
}


int send_message(char* message){
    uint8_t config_cnf = CONFIG_LORAMAC_DEFAULT_TX_MODE;
    uint8_t config_port = CONFIG_LORAMAC_DEFAULT_TX_PORT;

    semtech_loramac_set_tx_mode(&loramac, config_cnf);
    semtech_loramac_set_tx_port(&loramac, config_port);

    if( semtech_loramac_send(&loramac,(uint8_t *)message, strlen(message)) != SEMTECH_LORAMAC_TX_DONE ){
        printf("Cannot send message %s\n", message);
    }

    return 0;
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
    
    puts("Starting Up\n");
    puts("LoRaWAN Class A low-power application");
    puts("=====================================");

    // components init
    
    /*if(init_components() == 1){
        printf("Failed to Initialize Components\n");
        return 1;
    }*/
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

    semtech_loramac_init(&loramac);
    semtech_loramac_set_deveui(&loramac, deveui);
    semtech_loramac_set_appeui(&loramac, appeui);
    semtech_loramac_set_appkey(&loramac, appkey);
    semtech_loramac_set_dr(&loramac, LORAMAC_DR_5);

    puts("Starting join procedure");
    if (semtech_loramac_join(&loramac, LORAMAC_JOIN_OTAA) != SEMTECH_LORAMAC_JOIN_SUCCEEDED) {
        puts("Join procedure failed");
        return 1;
    }
    puts("Join procedure succeeded");
    //Starting Thread that handles temperature and humidity measurement
    //thread_create(temperature_stack,sizeof(temperature_stack),THREAD_PRIORITY_MAIN - 1, 0,read_temperature_humidity, NULL, "temp_sensor_logic");
    //puts("Temperature Thread Started");

    //Starting Thread that handles soil moisture measurement
    //thread_create(soil_stack,sizeof(soil_stack),THREAD_PRIORITY_MAIN -1, 0,read_soil, NULL, "soil_sensor_logic");
    //puts("Soil Thread Started");
    /* start the sender thread */
    sender_pid = thread_create(sender_stack, sizeof(sender_stack),
                               SENDER_PRIO, 0, sender, NULL, "sender");

    /* trigger the first send */
    msg_t msg;
    msg_send(&msg, sender_pid);
    
    return 0;
}