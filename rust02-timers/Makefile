# name of your application
APPLICATION = timers-example

# If no BOARD is found in the environment, use this default:
BOARD ?= feather-nrf52840-sense

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# required modules
USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += ztimer_sec

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1


# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Tell the build system to use the Rust crate here
FEATURES_REQUIRED += rust_target
APPLICATION_RUST_MODULE = timers_example
BASELIBS += $(APPLICATION_RUST_MODULE).module

include $(RIOTBASE)/Makefile.include
