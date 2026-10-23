# Name of the application
APPLICATION = BLE_multihop_central

# This application is built for the following board
BOARD ?= adafruit-feather-nrf52840-sense

# This is the path to the RIOT base directory
RIOTBASE ?= $(CURDIR)/../RIOT

# Include a directory for necessary header files in main.c
CFLAGS += -I$(RIOTBASE)/build/pkg/nimble/nimble/host/src

# Some RIOT modules needed for this example
USEMODULE += ztimer_sec

# Include NimBLE
USEMODULE += nimble_svc_gap
USEMODULE += nimble_svc_gatt



# /* [TASK 1: Allow the device to hold multiple connections.] */




# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Change this to 0 to not consider warnings as errors:
WERROR ?= 0

include $(RIOTBASE)/Makefile.include
