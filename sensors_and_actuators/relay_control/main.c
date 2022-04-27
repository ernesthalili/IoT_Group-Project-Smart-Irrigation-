#include <stdio.h>
#include "periph/gpio.h"
#include "xtimer.h"

int main(void){
    printf("Water pump application");

    gpio_t pin_out = GPIO_PIN(PORT_B, 5);
    if (gpio_init(pin_out, GPIO_OUT)){
        printf("Error to initialize GPIO_PIN(%d %d)\n", PORT_B, 5);
        return -1;
    }
        
    printf("Turn-off water pump\n");
    gpio_clear(pin_out);

    xtimer_sleep(2);

    printf("Turn-off water pump\n");
    gpio_set(pin_out);

    return 0;
}