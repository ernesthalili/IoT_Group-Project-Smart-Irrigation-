#include <stdio.h>
#include "periph/gpio.h"
#include "xtimer.h"

int main(void){
    printf("RIOT Traffic light application");
           
    gpio_t yellow = GPIO_PIN(PORT_B, 5);
    if (gpio_init(yellow, GPIO_OUT)) {
        printf("Error to initialize GPIO_PIN(%d %d)\n", PORT_B, 5);
        return -1;
    }
    gpio_t green = GPIO_PIN(PORT_B, 4);
    if (gpio_init(green, GPIO_OUT)) {
        printf("Error to initialize GPIO_PIN(%d %d)\n", PORT_B, 4);
        return -1;
    }
    gpio_t red = GPIO_PIN(PORT_B, 3);
    if (gpio_init(red, GPIO_OUT)) {
        printf("Error to initialize GPIO_PIN(%d %d)\n", PORT_B, 3);
        return -1;
    }

    while (1) {
        printf("Turn-on HIGH-yellow\n");
        gpio_set(yellow);

        xtimer_sleep(5);

        printf("Turn-off LOW-yellow\n");
        gpio_clear(yellow);

        printf("Turn-on HIGH-green\n");
        gpio_set(green);

        xtimer_sleep(5);

        printf("Turn-off LOW-green\n");
        gpio_clear(green);

        printf("Turn-on HIGH-red\n");
        gpio_set(red);

        xtimer_sleep(5);

        printf("Turn-off LOW-red\n");
        gpio_clear(red);
         
    }

    return 0;
}