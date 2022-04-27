#include <stdio.h>
#include "periph/gpio.h"
#include "xtimer.h"

#include "dht_params.h"
#include "dht.h"
#include "fmt.h"


int main(void){
   printf("Temperature and Humidity Monitoring\n");

   dht_params_t dht_params;
   dht_params.pin = GPIO_PIN(PORT_A,10);
   dht_params.in_mode = DHT_PARAM_PULL;

   //Initialization of DHT22 sensor      
   dht_t dev;

   if (dht_init(&dev, &dht_params) == DHT_OK){
      printf("DHT22 sensor connected\n");
   }   
   else{
      printf("Failed to connect to DHT sensor\n");
      return 1;
   }


   //Reading from the sensor
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
   
   // print the values
   printf("DHT values - temp: %s°C - relative humidity: %s%%\n",temp_s, hum_s);
   
   return 0;
}