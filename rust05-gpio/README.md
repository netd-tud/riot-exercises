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
$ cd ../rust05-gpios
```

## Task 1

Drive an LED using the GPIO API. So far we have operated on LEDs using a series
of LED abstractions provided by the board. What these macros do under the hood is to
write particular registers that control the GPIO peripheral.

**1. Outside the `main` function, `use` the gpio module to type less later:**

```rust
use riot_wrappers::gpio::{GPIO, OutputMode};
```

**2. Inside the `main` function define the led0 GPIO pin, and configure it as an output.**
**In the `feather-nrf52840-sense` we are currently using, the LED0 is connected to the**
**Port 1, Pin 9:**

```rust
let led0 = match riot_wrappers::BOARD {
    "feather-nrf52840-sense" => GPIO::from_port_and_pin(1, 9),
    _  => panic!("No pinout known for this board"),
};

let mut led0 = led0
    .expect("Pin should be available")
    .configure_as_output(OutputMode::Out)
    .expect("Pin should be usable as output");
```

By gating the pin creation with the board's name,
we can ensure that changing the board will not accidentally drive a pin that releases the magic smoke.

**3. The LEDs on the board are on when the GPIO outputs `1`.**
**Inside the `main` function, periodically set the GPIO to high (turning the LED on) and low (turning the LED off):**

```rust
loop {
    led0.set_high();
    Clock::msec().sleep_extended(Duration::from_millis(200));
    led0.set_low();
    Clock::msec().sleep_extended(Duration::from_millis(800));
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

*Unfortunately, there are no safe Rust wrappers for this functionality yet.*
*Thus, we jump right into how C functions are accessed in unsafe Rust --*
*an exercise that under ideal conditions is needed rarely, but hey:*
*learning how to do it by hand is a way towards having the high-level wrappers around!*

## Task 2

Turn the LED1 on whenever a button is pressed, and turn it off when the button has been released.
Use an interrupt to detect the value change.

**1. The same way as done in the previous task, initialize the GPIO pin for the LED1.**
**LED1 is connected to the Port 1, Pin 10.**
**Define `led1` in the main function.**

**2. Add crates we will need:**

```
$ cargo add riot-sys
$ cargo add static-cell
```

**and add**

```rust
use static_cell::StaticCell;
use riot_wrappers::gpio::OutputGPIO;
```

**below the use of task 1.**

We will use riot-sys to gain access to the low-level C functions,
and static-cell to safely obtain a mutable reference to static memory.

**2. Define a struct to transfer ownership of the used pins to the callback function.**
**While this could be done with statics just as well, mutable statics are discouraged in Rust.**

```rust
struct PinsForInterrupt {
    button: GPIO,
    led1: OutputGPIO,
}
```

**3. Write a callback function, which will be called when an interrupt occurs.**
**Whenever the button is pressed, the button's pin value is read to `0`.**
**As all we can pass across the C API for interrupts is a pointer,**
**we define that this pointer has the semantics of a `&'static mut PinsForInterrupt` pointer,**
**and cast it back accordingly.**

```rust
extern "C" fn button_callback(arg: *mut riot_sys::libc::c_void) {
    let pins = unsafe { &mut *(arg as *mut PinsForInterrupt) };

    // On a regular input pin we could run `.is_low()`, but the `riot_wrappers::gpio::OutputGPIO`
    // type would reconfigure the pin in its constructor, so we even read it manually.
    if unsafe { riot_sys::gpio_read(pins.button.to_c()) } == 0 {
        pins.led1.set_high();
    }
    else {
        pins.led1.set_low();
    }
}
```

**3. Define the GPIO pin connected to the user button on your board, and store its address for initialization.**
**The user button is connected to the Port 1, Pin 2.**

```rust
let button = GPIO::from_port_and_pin(1, 2)
    .expect("Pin should be available");
let button_address = button.to_c();
```

**4. Define a local static, move the pins into it, and get a static mutable reference to it.**

```rust
static PINS: StaticCell<PinsForInterrupt> = StaticCell::new();
let pins: &mut PinsForInterrupt = PINS.init(PinsForInterrupt {
    led1,
    button,
});
```

The documentation of [static_cell](https://docs.rs/static_cell/latest/static_cell/) explains well when it is convenient,
and which alternatives there are.
Particularly tempting alternatives here are
just unsafely using mutable statics (we know what we are doing … but we may miss details when refactoring later),
or using a RIOT Mutex (but then we'd have to do more error handling, because if a mutex is locked, it can not be awaited in an interrupt).

**5. Initialize the button from its address, configuring the callback to be called on any change, and passing in control of the pins.**

```rust
unsafe {
    riot_sys::gpio_init_int(button_address, riot_sys::gpio_mode_t_GPIO_IN_PU, riot_sys::gpio_flank_t_GPIO_BOTH, Some(button_callback), pins as *mut _ as *mut riot_sys::libc::c_void);
};
```

Note that the safety of this relies on GPIO interrupts not preempting themselves:
While RIOT can be used with priorized interrupts, the same interrupt will not trigger a jump while it is being executed.

**6. Build and flash the application.**

## Task 3

With another board available,
porting between RIOT boards is trivial.

* **1. Connect a micro:bit v2 to your system.**

* **2. Add a LED0 definition for the `BOARD == "microbit-v2"` case`.**
  **Use Port 0 Pin 20 -- that's actually enabling the microphone, but it is the closest that board has to a simple LED.**

* **3. Flash the example:**

```sh
$ BOARD=microbit-v2 make all flash
```

## Task 4

Refactor the blinking loop to make it portable across the embedded Rust ecosystem.
By not using methods on a RIOT [`OutputGPIO`](https://rustdoc.etonomy.org/riot_wrappers/gpio/struct.OutputGPIO.html)
but accessing it as an implementation of embedded-hal's [`OutputPin`](https://docs.rs/embedded-hal/latest/embedded_hal/digital/trait.OutputPin.html),
and doing the same with the ZTimer [clock](https://rustdoc.etonomy.org/riot_wrappers/ztimer/struct.Clock.html)
(as embedded-hal's [`DelayNs`](https://docs.rs/embedded-hal/latest/embedded_hal/delay/trait.DelayNs.html)),
we create the same binary program
using different abstractions.

* **1. Move the loop into a dedicated function. The signature could look like this:**

```rust
fn blink(mut led0: OutputGPIO, clock: Clock<1000>) -> ! {
```

You may need to adjust the convenience imports; the Rust compiler will make sensible suggestions.

* **2. Add the [`embedded-hal`](https://crates.io/crates/embedded-hal) crate as a dependency:**

```sh
$ cargo add embedded-hal
```

Note that this does not really increase the dependency tree:
`riot-wrappers` already depends on that crate,
but we need to explicitly depend on it to name its traits.

* **3. Alter the `blink` function to be generic over the traits' implementation:**
  **The signature could look like this:**

```rust
fn blink2(
    mut led0: impl embedded_hal::digital::OutputPin,
    mut clock: impl embedded_hal::delay::DelayNs,
) -> ! {
```

While GPIO pins' inherent methods are identical to the OutputPin's,
generic clocks require exclusive clock access and do not provide a [`Duration`](https://doc.rust-lang.org/std/time/struct.Duration.html) based sleep.
Instead, look up a suitable delay method in the [documentation of `DelayNs`](https://docs.rs/embedded-hal/latest/embedded_hal/delay/trait.DelayNs.html).
