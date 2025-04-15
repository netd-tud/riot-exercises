# name of your application
APPLICATION = BLE_central

# If no BOARD is found in the environment, use this default:
BOARD ?= feather-nrf52840-sense

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# Some RIOT modules needed for this example
USEMODULE += ztimer_sec

# Include NimBLE
USEMODULE += nimble_svc_gap
USEMODULE += nimble_svc_gatt

# It depends on RIOT version, if the following CFLAG is needed
# Use the high frequency crystal oscillator
# This is needed for the BLE stack to work properly
# Specifically on the feather-nrf52840-sense board
CFLAGS += -DCLOCK_HFXO_ONBOOT=1

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
