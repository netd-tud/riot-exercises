# name of your application
APPLICATION = thread_example

# The name of the crate (as per Cargo.toml package name, but with '-' replaced with '_')
#
# The presence of this triggers building Rust code contained in this
# application in addition to any C code.
APPLICATION_RUST_MODULE = thread_example

# If no BOARD is found in the environment, use this default:
BOARD ?= adafruit-feather-nrf52840-sense

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

USEMODULE += ztimer
USEMODULE += ztimer_msec
USEMODULE += ztimer_sec

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1


# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
