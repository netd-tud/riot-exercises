# Threads

Threads are functions designed to be scheduled and executed by the CPU
independently. An example of a thread is the `main()` function.
In single-core MCUs only one thread can have control of the CPU at a given time.
The scheduler is in charge of deciding which thread takes control, depending on
its state and priority. Usually threads are waiting for some event to occur,
and are not 'runnable' (e.g., waiting for a certain time to pass, a packet to
arrive, an external interrupt or an event to be posted on a queue).

To create threads, we first define a function with the following signature:

```rust
fn thread_handler()
```

The function content is what will be executed whenever our thread is running.
The function gets called from the beginning only once: at the moment of creation.
That is why, commonly, thread handlers contain a loop which would run forever.

In addition to a handler function, threads require memory: a stack. There local
variables and other information will be stored when performing function calls.

To create a new thread, we provide stack memory, a function, and spawn a thread:

```C
fn thread_handler() {
    // Some task for our thread
}

let my_thread = riot_wrappers::thread::spawn(
    // Memory stack
    thread_stack,
    // The thread handler function
    &mut thread_handler,
    // Human readable name of the thread
    c"my_thread",
    // Priority of the thread. The lower value, the higher priority
    (riot_sys::THREAD_PRIORITY_MAIN - 1) as _,
    // Thread flags. By default, enable stack memory usage measurements.
    (riot_sys::THREAD_CREATE_STACKTEST) as _,
    );
```

RIOT scheduler executes a thread until one of the following conditions occurs:
- The thread finishes
- The thread waits for an incoming event (e.g event queue)
- An interrupt source is triggered (button, GPIO pin, timer, etc). In this case,
  the scheduler resumes execution in the highest priority thread that does not have
  pending events.

For more information visit the
[thread documentation page](https://doc.riot-os.org/group__core__thread.html).

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../riot06-threads
```

## A note on interrupts

We usually don't want to perform lengthy tasks from interrupt contexts, as they
will block the execution of all threads. What is commonly done instead, is to
use some signalization mechanism to offload a task to a thread. A good choice is
to use events, as they are allocated by the sender and we are certain that they
will not get lost (as opposed to messages in RIOT). We will see how to utilize
events on the next exercise.

## Task 1

Create a "blinky" thread that toggles the LED1 every 250 milliseconds.

**1. Create a handler function for your thread.**
**As we want the task to be repeated indefinitely, we include an infinite loop:**


```rust
fn blinky_handler() {
    let mut led1 = riot_wrappers::led::LED::<1>::new();

    loop {
        println!(
            "Thread {}\n",
            riot_wrappers::thread::get_pid()
                .get_name()
                .unwrap_or("unnamed")
        );

        led1.on().unwrap();
        Clock::msec().sleep_extended(Duration::from_millis(20));
        led1.off().unwrap();
        Clock::msec().sleep_extended(Duration::from_millis(180));
    }
}
```

**2. Define a stack for your thread:**
```rust
use riot_sys::THREAD_STACKSIZE_DEFAULT;
use static_cell::StaticCell;
static THREAD_STACK: StaticCell<[u8; THREAD_STACKSIZE_DEFAULT as usize]> = StaticCell::new();
```

**3. Take possession of the stack, and spawn your thread.**
```rust
let thread_stack = THREAD_STACK.init_with(|| [0; THREAD_STACKSIZE_DEFAULT as usize]);
// Due to a bug in the spawn function
// <https://github.com/RIOT-OS/rust-riot-wrappers/issues/99>, we must provide the pointer
// inside a &'static mut
static STATIC_POINTER: StaticCell<fn()> = StaticCell::new();
let static_pointer = STATIC_POINTER.init(blinky_handler);
let my_thread = riot_wrappers::thread::spawn(
    thread_stack,
    static_pointer,
    c"blinky",
    (riot_sys::THREAD_PRIORITY_MAIN - 1) as _,
    (riot_sys::THREAD_CREATE_STACKTEST) as _,
);
```

**4. Build and flash your application. Open a serial communication:**
```sh
$ make all flash term
```

You should see messages from both threads and the LEDs blinking at different rates.
