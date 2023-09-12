# Event Queues

For user applications, the Event Queue module provides a versatile mechanism to
schedule tasks in a thread. In a nutshell, the mechanism works as follows:

- A thread has an event queue, and waits for new events to be posted to it.
- An event contains a pointer to an event handler function (a task).
- When an event is posted on a queue (from an interrupt, or from another thread)
the thread that listens to the queue will execute the task pointed by the event.

In order to use events, we add the `event` module to the application Makefile:
```Makefile
USEMODULE += event
```

The event module API is exposed via the `event.h` header file
```C
#include "event.h"
```

RIOT provides event queue helpers to implement common patterns such as
executing events on timeout, executing periodic tasks, etc. It also provides
predefined event queues.

To learn more about this mechanism, visit the
[Event Queue documentation page](https://doc.riot-os.org/group__sys__event.html).

For this experience, we focus only on the basic Event Queue support.

## Create an event

Event tasks are `event_t` objects that point to a handler function:
```C
void event_handler(event_t *event)  /* The task handler */
{
    /* important task to execute */
}

event_t my_event = { .handler = event_handler }; /* An event that executes `event_handler`
when posted */
```

## Start the event queue

A thread that receives events must initialize a thread queue and run the event
loop. The event loop mainly blocks the thread until a new event is posted to the
queue. At that moment the event is served by calling its handler function. After
the event has been served, the thread goes back to waiting for a new event to be
posted. For example:

```C
event_queue_t queue;    /* An event queue */

int main(void)
{
    event_queue_init(&queue);   /* Initialize the event queue */
    event_loop(&queue);         /* Wait for an event */
}
```

As mentioned earlier, RIOT provides three predefined queues via the
`event_thread` module. These queues are defined in the `event/thread.h` header.

```c
/**
 * @brief   Pointer to the event queue handling highest priority events
 */
#define EVENT_PRIO_HIGHEST  (&event_thread_queues[EVENT_QUEUE_PRIO_HIGHEST])
/**
 * @brief   Pointer to the event queue handling medium priority events
 */
#define EVENT_PRIO_MEDIUM   (&event_thread_queues[EVENT_QUEUE_PRIO_MEDIUM])
/**
 * @brief   Pointer to the event queue handling lowest priority events
 */
#define EVENT_PRIO_LOWEST   (&event_thread_queues[EVENT_QUEUE_PRIO_LOWEST])
```

## Post an event

To post events use the `event_post()` function. We can post events from interrupts
or from other threads. Events are executed in a first in first out (FIFO)
scheme.

```C
event_post(
            &queue,     /* queue that executes the event */
            &my_event   /* the event */
        );
```

For example, the following snippet posts `my_event` to the queue with highest
priority.

```c
event_post(EVENT_PRIO_HIGHEST, &my_event);
```

## Task 1

Configure an interrupt on the board button. When the button is pressed,
wait for 2 seconds before turning the LED0 on.

Let's take a simple approach without events first.

**1. Implement the content of `button_callback` function to wait for 2 seconds before toggling the LED:**
```C
/* inside button_callback */

/* get the current time */
ztimer_now_t start = ztimer_now(ZTIMER_MSEC);

/* wait until 2000 ms have passed */
while (ztimer_now(ZTIMER_MSEC) - start < 2000) { }

LED0_TOGGLE;
puts("Done");
```
**2. Build and flash the application. Open a serial communication:**
```sh
$ make all flash term
```

You will notice that, under normal circumstances, the main thread prints a
message every 1 second. When you push the button the message does not appear
until the 2 seconds have passed. This is because interrupts have higher priority
than threads. We need to utilize some mechanism to offload tasks from
interrupts, so we don't block other threads. Let's use events.

## Task 2

Post an event to the highest priority queue whenever the button is pressed.

**1. Create the event handler function, which executes the task:**
```C
void event_handler(event_t *event)
{
    (void) event;   /* Not used */

    ztimer_sleep(ZTIMER_MSEC, 2000);
    LED0_TOGGLE;
    puts("Done");
}
```

**2. Instantiate the event pointing to the handler:**
```C
event_t event = { .handler = event_handler };
```

**3. Add the `event_thread` module to the application Makefile:**

```Makefile
USEMODULE += event_thread
```

**4. Include the `event/thread.h` header to `main.c`**

```c
#include "event/thread.h"
```

**5. Modify the `button_callback` to post the event instead:**
```C
void button_callback(void *arg)
{
    (void) arg;    /* Not used */

    event_post(EVENT_PRIO_HIGHEST, &event);
}
```

**6. Build and flash the application. Open a serial communication.**
