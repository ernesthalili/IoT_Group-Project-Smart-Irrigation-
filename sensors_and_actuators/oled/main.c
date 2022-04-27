//set of include for display
#include "periph/gpio.h"
#include "periph/spi.h"
#include "ucg.h"
#include "ucg_riotos.h"
#include "logo.h"

int main(void){
    printf("Display on\n");

    static ucg_riotos_t user_data ={
        .device_index = SPI_DEV(0),
        .pin_cs = GPIO_PIN(PORT_B, 6),
        .pin_cd = GPIO_PIN(PORT_C, 7),
        .pin_reset = GPIO_PIN(PORT_A, 9),
    };

    ucg_t ucg;
    ucg_SetUserPtr(&ucg, &user_data);

    ucg_Init(&ucg, ucg_dev_ili9341_18x240x320, ucg_ext_ili9341_18,ucg_com_hw_spi_riotos);

    ucg_SetRotate270(&ucg);
    ucg_SetFontMode(&ucg, UCG_FONT_MODE_SOLID);
    ucg_SetFont(&ucg, ucg_font_helvR18_tf);

    ucg_ClearScreen(&ucg);

    for (int y = 0; y < 48; y++){
        for (int x = 0; x < 96; x++){
            uint32_t offset = (x + (y * 96)) * 3;
            ucg_SetColor(&ucg, 0, logo[offset + 0], logo[offset + 1],
                        logo[offset + 2]);
            ucg_DrawPixel(&ucg, x, y);
        }
    }

    ucg_SetColor(&ucg, 0, 0, 255, 0);
    ucg_DrawString(&ucg,120,40,0,"Smart Irrigation");
    ucg_SetColor(&ucg, 0, 255, 0, 0);
    ucg_DrawString(&ucg,8,90,0,"Room Temperature=40");
    ucg_DrawString(&ucg,8,120,0,"Room Humidity=50");
    ucg_DrawString(&ucg,8,150,0,"Soli Temperature=40");
    ucg_DrawString(&ucg,8,180,0,"Soil Moisture=50");

    return 0;
}



