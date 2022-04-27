#include <stdio.h>
#include "xtimer.h"
#include "seesaw_soil.h"
#include "seesaw_soil_params.h"
#include "seesaw_soil_regs.h"
#include "fmt.h"

seesaw_soil_t dev;
int16_t temp;
uint16_t moist;
char tstr[8];
char mstr[8];

int main(void){
    int test = 0;    
    test = seesaw_soil_init(&dev, &seesaw_soil_params[0]);
    if (test == SEESAW_SOIL_OK){
        puts("OK\n");
        //puts("hello-1\n");
    }
    else{
        puts("FAILED\n");
    }
    //puts("hello0\n");
    while(1){
        size_t len;
        //puts("hello1\n");
        seesaw_soil_temperature (&dev, &temp);
        seesaw_soil_moisture(&dev, &moist);

        len = fmt_s16_dfp(tstr, temp, -2);
        tstr[len] = '\0';

        len = fmt_u16_dec(mstr, moist);
        mstr[len] = '\0';

        printf("Reading: T: %s °C  Moist: %s\n", tstr, mstr);
        //puts("hello2\n");
        xtimer_sleep(5);
    }

	return 0;
}
