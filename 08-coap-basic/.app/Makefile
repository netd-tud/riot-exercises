# name of your application
APPLICATION = coap-example

# If no BOARD is found in the environment, use this default:
BOARD ?= pba-d-01-kw2x

# SELECT A TASK
TASK_4 = 1
# TASK_5 = 1

ifdef TASK_4
  CFLAGS += -DTASK_4
endif

ifdef TASK_5
  CFLAGS += -DTASK_5
  CFLAGS += -DCONFIG_CORD_LT=60
  USEMODULE += cord_ep_standalone
endif

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../RIOT

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

# object dump allows use to print streams of bytes
USEMODULE += od

# fmt allows use to format strings
USEMODULE += fmt

USEMODULE += netutils
# Add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += ps

# include SAUL to interact with onboard sensors and actuators
USEMODULE += saul

# include and auto-initialize all available sensors
USEMODULE += saul_default

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
