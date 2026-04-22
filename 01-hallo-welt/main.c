/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include "ztimer.h"

int main(void)
{
    printf("Hallo Welt!\n");

    printf("Du führst RIOT auf einem %s Board aus.\n", RIOT_BOARD);
    printf("Dieses Board verfügt über einen %s-Mikrokontroller.\n", RIOT_CPU);

    /* [AUFGABE 4] Füge hier deinen eigene Ausgabe hinzu. */
    return 0;
}
