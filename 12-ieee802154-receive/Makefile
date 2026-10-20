# Name of your application
APPLICATION = IEEE802154_receive

# This application is built for the following board
BOARD ?= adafruit-feather-nrf52840-sense

# This is the path to the RIOT base directory
RIOTBASE ?= $(CURDIR)/../RIOT

# Include netdev and gnrc modules
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif
USEMODULE += gnrc

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Change this to 0 to not consider warnings as errors:
WERROR ?= 0

include $(RIOTBASE)/Makefile.include
