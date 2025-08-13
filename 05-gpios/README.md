# GPIOs

General Purpose Input/Outputs (GPIOs) are a peripheral that allows
microcontrollers to interact with the physical world. They are 
commonly known as pins. As the name suggests, they can be either configured as
digital inputs or digital outputs. That is, they can read a digital value from
the outside, or present a digital value to the outside (which translates in
presenting a voltage of either 0V for digital `0` or commonly 3.3V for `1`).

They are useful for a huge range of tasks. As outputs, they could drive an
indication LED or a circuit like a valve or a lock (special electrical
adaption is needed in this case, such as a transistor, as the GPIOs don't
handle high currents). As inputs, they can be used to read buttons, switches
or digital sensors (e.g. a presence or a window sensor).

RIOT provides the `periph_gpio` module to interact with the peripheral. It
exposes a simple API that allows to use GPIOs independently of the underlying
platform. In this example we will make use of the most common functionalities
of the GPIO module.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../05-gpios
```

## Task 1

Drive an LED using the GPIO API. So far we have operated on LEDs using a series
of macros provided by the board. What these macros do under the hood is to
write particular registers that control the GPIO peripheral.

**1. To use the functionalities of the GPIO module, you need the `periph/gpio.h` header:**
```C
/* at the top of main.c */
#include "periph/gpio.h"
```

**2. Outside the `main` function define the LED0 GPIO pin as an output.**
**In the `adafruit-feather-nrf52840-sense` we are currently using, the LED0 is connected to the**
**Port 1, Pin 9:**
```C
gpio_t led0 = GPIO_PIN(1, 9);
gpio_mode_t led0_mode = GPIO_OUT;
```

**3. The LEDs on the board are on when the GPIO outputs `1`.**
**Inside the `main` function, start by initializing the GPIO and turning the LED off (setting the GPIO to `0`):**
```C
gpio_init(led0, led0_mode);
gpio_clear(led0);
```

**4. Inside a loop, periodically toggle the value of the GPIO:**
```C
while (1) {
    gpio_toggle(led0);
    ztimer_sleep(ZTIMER_MSEC, 500);
}
```

**5. Build and flash the application:**
```sh
$ make all flash
```

## GPIO interrupts

We could constantly read a GPIO input to check if the value has changed, but this
consumes CPU cycles and energy. Instead, we can configure a GPIO to generate an
event: an **interrupt** (we will see more about threads and interrupts in the
next task). Interrupts are generated when certain pre-established conditions
are met. We can configure a GPIO to generate an interrupt when the external
value changes to `0`, to `1`, or whenever there is a change.

## Task 2

Turn the LED1 on whenever a button is pressed, and turn it off when the button has been released.
Use an interrupt to detect the value change.

**1. The same way as done in the previous task, initialize the GPIO pin for the LED1.**
**LED1 is connected to the Port 1, Pin 10.**
**Define `led1` outside the main function.**

**2. Write a callback function, which will be called when an interrupt occurs.**
**Whenever the button is pressed, the pin value is set to `0`.**
```C
void button_callback (void *arg)
{
    (void) arg; /* the argument is not used */
    if (!gpio_read(button)) {
        gpio_set(led1);
    }
    else {
        gpio_clear(led1);
    }
}
```

**3. Initialize the GPIO pin connected to the user button on your board as input with an internal pull-up.**
**The user button is connected to the Port 1, Pin 2.**
```C
/* define button outside the main function, as we will use it later */
gpio_t button = GPIO_PIN(1, 2);

/* initialize button inside the main function */
gpio_init_int(button, GPIO_IN_PU, GPIO_BOTH, button_callback, NULL);
```

**4. Build and flash the application.**
