# name of your application
APPLICATION = threads_and_events

# If no BOARD is found in the environment, use this default:
BOARD ?= feather-nrf52840-sense

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../RIOT

# use the shell module
USEMODULE += shell

FEATURES_REQUIRED += periph_gpio_irq

# use the event module and the predefined high priority queue
USEMODULE += event
USEMODULE += event_thread_highest

# Enable the milliseconds timer.
USEMODULE += ztimer_msec

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
