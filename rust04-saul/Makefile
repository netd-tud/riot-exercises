APPLICATION = saul_example

# If no BOARD is found in the environment, use this default:
BOARD ?= feather-nrf52840-sense

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# include SAUL to interact with onboard sensors and actuators
USEMODULE += saul

# we use ztimer to read sensors periodically
USEMODULE += ztimer
USEMODULE += ztimer_sec
USEMODULE += ztimer_msec

# include and auto-initialize all available sensors
USEMODULE += saul_default

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1


# Tell the build system to use the Rust crate here
FEATURES_REQUIRED += rust_target
APPLICATION_RUST_MODULE = saul_example
BASELIBS += $(APPLICATION_RUST_MODULE).module

include $(RIOTBASE)/Makefile.include
