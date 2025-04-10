# name of your application
APPLICATION = coap-example

# If no BOARD is found in the environment, use this default:
BOARD ?= feather-nrf52840-sense

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# enable default networking devides in the platform
USEMODULE += netdev_default

# automatically initialize network interfaces for enabled devices
USEMODULE += auto_init_gnrc_netif

# add minimal IPv6 support
USEMODULE += gnrc_ipv6_default

# add ICMPv6 support (ping)
USEMODULE += gnrc_icmpv6_echo

# add CoAP module
USEMODULE += gcoap

ifdef DBG_ID_COAP
  DEBUG_ADAPTER_ID=$(DBG_ID_COAP)
  PORT=/dev/ttyACM1
endif

# object dump allows use to print streams of bytes
USEMODULE += od

# fmt allows use to format strings
USEMODULE += fmt

USEMODULE += netutils
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ps

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1


# We need a larger buffer in order to send some commands without truncation.
CFLAGS += -DSTDIO_RX_BUFSIZE=128

# The default size of GCOAP_PDU_BUF_SIZE can be too small for the response
# from the RD server. Use a larger value if the client drops response packet
# from RD server.
CFLAGS += -DCONFIG_GCOAP_PDU_BUF_SIZE=512

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
