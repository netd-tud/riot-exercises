# Simple timer example

In this example we see two fundamental ways of using the timers of your hardware.
First, we configure a timer to trigger an event that gets executed in the future.
Second, we use the timer to sleep (block the thread) for a known amount of time.

## Adding modules to your RIOT application
RIOT is highly modular, meaning that functionalities are split into modules, which
are only built when required.

In order to interact with timers, we need to use the `ztimer` module. To make it
available to us, we add it to the list of used modules in the application's
Makefile: `USEMODULE += ztimer`. Depending on the granularity that we want when
handling time, we can specify different extra modules:

```Makefile
USEMODULE += ztimer_usec # microsecond precision
USEMODULE += ztimer_msec # millisecond precision
USEMODULE += ztimer_sec  # second precision
```

To learn more about the `ztimer` module, visit the [documentation page](https://doc.riot-os.org/group__sys__ztimer.html).

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../02-timers
```

## Task 1
**1. Build and flash the application. Connect to the serial port:**
```sh
$ make all flash term
```

**2. Reset the board. You should see a "Timeout!" string after 2 seconds.**

## Task 2
Modify the application so that the board blinks the LED every 250 ms for 5 seconds.

**1. Adapt the condition in the `while` loop to be true only until 5000 ms:**
```C
while ((ztimer_now(ZTIMER_MSEC) - start) <= 5000) {
```

**2. Sleep only for 250 ms for each iteration:**
```C
ztimer_sleep(ZTIMER_MSEC, 250);
```

**3. Build and flash the application. Connect to the serial port:**
```sh
$ make all flash term
```

## Task 3

Add a new timer to turn LED1 on after 1 second.

**1. Define a new callback function to execute the task:**
```C
void led_callback(void *argument)
{
    (void) argument; /* we don't use the argument */
    LED1_ON;         /* turn LED 1 on */
}
```

**2. Instantiate and configure a new `ztimer_t` variable. The callback should be `led_callback`.**
**The argument is not used:**

```C
/* inside the main function, before the while loop */
ztimer_t led_timeout;                     /* create a new timer */
led_timeout.callback = led_callback;  /* set the function to execute */
```

**3. Set the timer to fire in 1 second:**
```C
/* set the timer to trigger in 1 seconds */
ztimer_set(ZTIMER_SEC, &led_timeout, 1);
```

**4. Recompile and build the application. Press the reset button.**
```sh
$ make all flash term
```
**You should see the LED 1 going on after a second, then the message "Timeout!" should be printed**
