/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdint.h>
#include <stdio.h>
#include "ztimer.h"
#include "board.h"

/* Gibt true zurück wenn die Zeit abgelaufen ist, ansonsten false. */
bool timer_running(ztimer_now_t start, uint32_t duration)
{
    uint32_t now = ztimer_now(ZTIMER_SEC);
    return (now - start) < duration;
}

/* [AUFGABE 3] Füge hier die Call-back Funktion ein. */


int main(void)
{
    printf("Das ist ein Timer Beispiel.\n");

    /* [AUFGABE 3] Füge hier den Timer ein. */


    /* Lese den aktuellen Timer Zählstand */
    ztimer_now_t start = ztimer_now(ZTIMER_SEC);

    /* Setze die Dauer auf 10 Sekunden */
    uint32_t duration = 10;

    /* Blinke die LED in einer Schleife.*/
    while (timer_done(start, duration)) {
        /* Schalte die LED */
        LED0_TOGGLE;
        /* Warte 1 Sekunde */
        uint32_t wait = 1;
        ztimer_sleep(ZTIMER_SEC, wait);
    }

    printf("Fertig!\n");

    return 0;
}
