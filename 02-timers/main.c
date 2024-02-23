/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>

#include "ztimer.h"
/* needed to manipulate the LEDs */
#include "board.h"

void message_callback(void *argument)
{
    char *message = (char *)argument;
    puts(message);
}

/* [TASK 3: insert your callback function here] */

int main(void)
{
    puts("This is a timers example");

    /* we can configure an event to occur in the future by setting a timer */
    ztimer_t timeout;                     /* create a new timer */
    timeout.callback = message_callback;  /* set the function to execute */
    timeout.arg = "Timeout!";             /* set the argument that the function will receive */
    ztimer_set(ZTIMER_SEC, &timeout, 2);  /* set the timer to trigger in 2 seconds */

    /* [TASK 3: insert your timer here] */

    /* in parallel, we can perform other tasks on this thread */
    /* get the current timer count */
    ztimer_now_t start = ztimer_now(ZTIMER_MSEC);

    /* blink an LED for 10 seconds */
    while ((ztimer_now(ZTIMER_MSEC) - start) <= 10000) {
        /* this blinks the LED twice a second */
        LED0_TOGGLE;
        ztimer_sleep(ZTIMER_MSEC, 500);
    }

    puts("Done!");

    return 0;
}
