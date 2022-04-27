#include <stdio.h>
#include <string.h>
#include "periph/gpio.h"
#include "xtimer.h"
#include "thread.h"
#include "cpu.h"
#include "board.h"
#include "dht_params.h"
#include "dht.h"
#include "seesaw_soil.h"
#include "seesaw_soil_params.h"
#include "seesaw_soil_regs.h"
#include "fmt.h"
#include "net/loramac.h"
#include "semtech_loramac.h"
#include "periph/i2c.h"
#include "timex.h"
#include "ztimer.h"
#include "u8g2.h"
#include "u8x8_riotos.h"
#include "main.h"



//dht22 sensor
dht_t dev;
dht_params_t dht_params =
{
    .pin = GPIO_PIN(PORT_A,9); //D8
    .in_mode = DHT_PARAM_PULL;
};


//soil sensor
seesaw_soil_t dev;

//relay water pump
gpio_t pin_relay = GPIO_PIN(PORT_B, 13); //D13


//display pin SDA D14 and SCK D15
#define TEST_OUTPUT_I2C 4
u8g2_t u8g2;
u8x8_riotos_t user_data =
{
    .device_index = TEST_I2C,
    .pin_cs = TEST_PIN_CS,
    .pin_dc = TEST_PIN_DC,
    .pin_reset = TEST_PIN_RESET,
};


void read_temperature_humidity(void){ //dht22 sensor
    int16_t temp, hum;

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

        
    printf("DHT values - temp: %s°C - relative humidity: %s%%\n",temp_s, hum_s);
}


void irrigate(void){ //relay water pump
    printf("Turn-off water pump\n");
    gpio_clear(pin_relay);
    xtimer_sleep(2);
    printf("Turn-off water pump\n");
    gpio_set(pin_relay);

}


void read_soil(void){ //soil sensor
    size_t len;
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

    printf("Reading: T: %s °C  Moist: %s\n", tstr, mstr);
    xtimer_sleep(5);
}


void write_display(char* message){ //Display

    u8g2_FirstPage(&u8g2);

    do {
        u8g2_SetDrawColor(&u8g2, 1);
        u8g2_SetFont(&u8g2, u8g2_font_helvB12_tf);
        u8g2_DrawStr(&u8g2, 12, 22, message);
    } while (u8g2_NextPage(&u8g2));

}


void init_components(void){ //initialize all components
//dht22 sensor inizialization
    if (dht_init(&dev, &dht_params) == DHT_OK){
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

}

int main(void){
    
    init_components();

    while(1){
        read_temperature_humidity();
        read_soil();
        write_display();

        xtimer_sleep(100);  // time to sleep after every read

    }
    
    return 0;
}