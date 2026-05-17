APPLICATION = saul_example

# The name of the crate (as per Cargo.toml package name, but with '-' replaced with '_')
#
# The presence of this triggers building Rust code contained in this
# application in addition to any C code.
APPLICATION_RUST_MODULE = saul_example

# If no BOARD is found in the environment, use this default:
BOARD ?= adafruit-feather-nrf52840-sense

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

include $(RIOTBASE)/Makefile.include
