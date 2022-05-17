APPLICATION = smart_irrigation

BOARD ?= b-l072z-lrwan1

RIOTBASE ?= $(CURDIR)/../..


LORA_DRIVER ?= sx1276
LORA_REGION ?= EU868
USEPKG += semtech-loramac
USEMODULE += $(LORA_DRIVER)
USEMODULE += semtech_loramac_rx
USEMODULE += auto_init_loramac

USEMODULE += shell
USEMODULE += shell_commands
USEMODULE += fmt

USEMODULE += periph_gpio
USEMODULE += xtimer
USEMODULE += periph_gpio_irq

FEATURES_OPTIONAL += periph_eeprom
FEATURES_REQUIRED = periph_gpio


USEMODULE += ztimer
USEMODULE += ztimer_usec
USEPKG += u8g2

TEST_OUTPUT ?= 4

USEMODULE += periph_i2c
FEATURES_REQUIRED += periph_gpio periph_i2c


include $(RIOTBASE)/Makefile.include



