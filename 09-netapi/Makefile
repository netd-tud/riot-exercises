# name of your application
APPLICATION = gnrc_netapi

# If no BOARD is found in the environment, use this default:
BOARD ?= feather-nrf52840-sense

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# enable default networking devides in the platform
USEMODULE += netdev_default

# automatically initialize network interfaces for enabled devices
USEMODULE += auto_init_gnrc_netif

# enable single interface optimization.
# remove this if more than one interface is present
USEMODULE += gnrc_netif_single

# add minimal IPv6 support
USEMODULE += gnrc_ipv6_default

# enable ping support
USEMODULE += gnrc_icmpv6_echo

# add module to hex dump data
USEMODULE += od

# add also the shell, some shell commands
USEMODULE += shell
USEMODULE += shell_cmds_default

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
