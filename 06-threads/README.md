# Threads

Threads are functions designed to be scheduled and executed by the CPU
independently. An example of a thread is the `main()` function.
In single-core MCUs only one thread can have control of the CPU at a given time.
The scheduler is in charge of deciding which thread takes control, depending on
its state and priority. Usually threads are waiting for some event to occur,
and are not 'runnable' (e.g., waiting for a certain time to pass, a packet to
arrive, an external interrupt or an event to be posted on a queue).

To create threads, we first define a function with the following signature:

```C
void *thread_handler(void *arg)
```

The function content is what will be executed whenever our thread is running.
The function gets called from the beginning only once: at the moment of creation.
That is why, commonly, thread handlers contain a loop which would run forever.

In addition to a handler function, threads require memory: a stack. There local
variables and other information will be stored when performing function calls.
For this we simply define an array of a given size:
```C
char thread_stack[THREAD_STACKSIZE_DEFAULT];   /* Stack for the thread */
```

To create a new thread, we use `thread_create()` from `thread.h`:

```C
kernel_pid_t my_thread;            /* Stores the id */
void *thread_handler(void *arg) {  /* Thread handler */
    /* some task for our thread */
}

my_thread = thread_create(
    thread_stack,              /* Memory stack */
    sizeof(thread_stack),      /* Size of the memory stack */
    THREAD_PRIORITY_MAIN - 1,  /* Priority of the thread. The lower value, the
                                  higher priority */
    THREAD_CREATE_STACKTEST,   /* Thread flags. By default, enable stack memory usage measurements. */
    thread_handler,            /* The thread handler function */
    NULL,                      /* Argument of the thread handler */
    "my_thread");              /* Human readable name of the thread */
```

RIOT scheduler executes a thread until one of the following conditions occurs:
- The thread finishes
- The thread waits for an incoming event (e.g event queue)
- An interrupt source is triggered (button, GPIO pin, timer, etc). In this case,
  the scheduler resumes execution in the highest priority thread that does not have
  pending events.

For more information visit the
[thread documentation page](https://doc.riot-os.org/group__core__thread.html).

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
```C
void *blinky_handler(void *arg)
{
    (void) arg; /* argument not used */

    /* get the current thread descriptor */
    thread_t * this_thread = thread_get_active();

    /* get the thread name */
    const char *this_thread_name = thread_get_name(this_thread);

    while (1) {
        printf("Thread %s\n", this_thread_name);
        LED1_TOGGLE;
        ztimer_sleep(ZTIMER_MSEC, 250);
    }
}
```

**2. Define a stack for your thread:**
```C
char blinky_stack[THREAD_STACKSIZE_DEFAULT];
```

**3. Create the blinky thread from the `main` thread:**
```C
thread_create(
    blinky_stack,
    sizeof(blinky_stack),
    THREAD_PRIORITY_MAIN - 1,
    THREAD_CREATE_STACKTEST,
    blinky_handler,
    NULL,
    "blinky"
);
```

**4. Build and flash your application. Open a serial communication:**
```sh
$ make all flash term
```

You should see messages from both threads and the LEDs blinking at different rates.
