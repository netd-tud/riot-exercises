/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>

#include "ztimer.h"
#include "phydat.h"
#include "saul_reg.h"
#include "board.h"

#define TEMPERATURE_THRESHOLD 2400 /* Faktor von 10^-2 */

int main(void)
{
    puts("SAUL Beispiel");

    /* Beginne mit der Suche nach einem Temperatursensor im System */
    saul_reg_t *temp_sensor = saul_reg_find_type(SAUL_SENSE_TEMP);
    if (!temp_sensor) {
        puts("Kein Temperatursensor vorhanden.");
        return 1;
    }
    else {
        printf("Temperatursensor gefunden: %s\n", temp_sensor->name);
    }

    /* [AUFGABE 3: Finde hier dein Gerät] */

    /* Merke dir den Startzeitpunkt. */
    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_MSEC);

    while (1) {

        /* Lese einen Temperaturwert vom Sensor. */
        phydat_t temperature;
        int dimensions = saul_reg_read(temp_sensor, &temperature);
        if (dimensions < 1) {
            puts("Fehler beim Lesen des Temperaturwertes.");
            break;
        }

        /* Gib den gelesenen Wert auf STDIO aus */
        phydat_dump(&temperature, dimensions);

        /* [AUFGABE 3: Füge hier das Lesen der Beschleunigung zu] */

        /* Überprüfe, ob der Temperaturwert über dem Schwellenwert liegt. */
        if (temperature.val[0] >= TEMPERATURE_THRESHOLD) {
            LED0_ON;
        }
        else {
            LED0_OFF;
        }

        /* Warte 500 ms */
        ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, 500);
    }

    return 0;
}