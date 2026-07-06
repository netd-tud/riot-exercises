/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <string.h>

#include "shell.h"

/* [Aufgabe 2: Füge die Funktion für den deine toggle-Befehl hier ein] */


int echo_command(int argc, char **argv)
{
  /* Prüfen, ob der Befehl richtig aufgerufen wurde. */
    if (argc != 2) {
        puts("Benutzung: echo <message>");
        puts("Anmerkung: um mehrere Worte wiederzugeben setze sie in Anführungszeichen (\"\")");
        return 1;
    }

    /* Gebe das erste Argument aus. */
    puts(argv[1]);

    return 0;
}

/* [Aufgabe 2: Registriere hier deinen neuen Befehl. ] */

SHELL_COMMAND(echo, "Echo eine Nachricht aus.", echo_command);

int main(void)
{
    /* Zwischenspeicher für Nachrichten */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    /* Starte die Shell. */
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
