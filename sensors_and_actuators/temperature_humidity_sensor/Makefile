# name of your application
APPLICATION = temperature_humidity

# If no BOARD is found in the environment, use this default:
BOARD ?= nucleo-f401re

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..  # todo change the path for RIOT

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += periph_gpio
USEMODULE += xtimer
USEMODULE += dht
USEMODULE += fmt

include $(RIOTBASE)/Makefile.include
