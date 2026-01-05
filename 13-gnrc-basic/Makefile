APPLICATION = raw_mac

# If no BOARD is found in the environment, use this default:
BOARD ?= feather-nrf52840-sense

RIOTBASE ?= $(CURDIR)/../RIOT


# Required modules
USEMODULE += xtimer
USEMODULE += shell
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif

# Include GNRC network stack for IEEE 802.15.4
USEMODULE += gnrc_netif
USEMODULE += gnrc_neterr
USEMODULE += gnrc_pktdump

# IEEE 802.15.4 specific modules
USEMODULE += netdev_ieee802154


# Disable shell to save space and enforce no-shell operation
#DISABLE_MODULE += shell
UF2_MOUNTPOINT = /media/pi/feather

include $(RIOTBASE)/Makefile.include

ifeq ($(SENDER),1)
  CFLAGS += -DSENDER=1
else
  CFLAGS += -DSENDER=0
endif
