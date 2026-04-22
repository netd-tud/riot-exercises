# Einfaches Timer-Beispiel

In diesem Beispiel lernst du, wie du die Timer (interne Uhren) deiner Hardware nutzen kannst.
Wir verwenden den Timer, um zu messen, wie viel Zeit vergeht, und um unser Programm für eine bestimmte Zeit schlafen zu lassen (zu blockieren).

Um in den Ordner der aktuellen Übung zu wechseln, benutze folgenden Befehl im Terminal:

```sh
cd ../02-timer
```

## Aufgabe 1 - LED blinken lassen

1. **Baue und flashe die Anwendung. Verbinde dich mit dem Board:**

   ```sh
   make all flash term
   ```

2. **Setze das Board zurück, indem du den Reset-Knopf drückst.**

   Du wirst sehen, dass eine LED blinkt.

### Erklärung des Codes

```c
/* Lese den aktuellen Timer-Zählstand */
ztimer_now_t start = ztimer_now(ZTIMER_MSEC);

/* Setze die Dauer auf 10 Sekunden */
uint32_t dauer = 10 * MS_PER_SEC;

/* Blinke die LED in einer Schleife. */
while (timer_laeuft(start, dauer)) {
    /* Schalte die LED */
    LED0_TOGGLE;
    /* Warte 1 Sekunde */
    uint32_t warte_zeit = 1 * MS_PER_SEC;
    ztimer_sleep(ZTIMER_MSEC, warte_zeit);
}
```

In diesem Beispiel verwenden wir die Bibliothek `ztimer`, um Timer zu steuern.
Damit können wir zum Beispiel:

- den aktuellen Timer-Zählstand lesen (`ztimer_now`)
- unser Programm für eine bestimmte Zeit pausieren (`ztimer_sleep`)

**Wie funktioniert der Code?**

- Der Timer startet, indem wir den aktuellen Zählerstand mit `ztimer_now` speichern.
- Wir legen die Zielzeit (`dauer`) auf 10 Sekunden fest.
- Mit einer `while`-Schleife blinkt die LED so lange, wie die Funktion `timer_laeuft` wahr ist.
  Die Funktion `timer_laeuft` prüft, ob die vergangene Zeit seit `start` kleiner ist als die Zielzeit.
- In jeder Iteration der Schleife schalten wir die LED um (`LED0_TOGGLE`) und lassen das Programm für 1 Sekunde schlafen (`ztimer_sleep`).

## Aufgabe 2 - LED länger blinken lassen

Ändere das Programm so, dass die LED für 20 Sekunden blinkt und dabei alle 2 Sekunden geschaltet wird.

1. **Passe die Zielzeit (`dauer`) auf 20 Sekunden an:**

   ```c
   uint32_t dauer = 20 * MS_PER_SEC;
   ```

2. **Ändere die Wartezeit auf 2 Sekunden:**

   ```c
   uint32_t warte_zeit = 2 * MS_PER_SEC;
   ```

3. **Baue und flashe die Anwendung erneut:**

   ```sh
   make all flash term
   ```

## Aufgabe 3 - Timer mit Callback-Funktion

In den bisherigen Beispielen haben wir das Programm während der Wartezeiten schlafen lassen.
Jetzt nutzen wir eine Callback-Funktion, damit das Programm währenddessen andere Aufgaben erledigen kann.

Eine **Callback-Funktion** ist eine Funktion, die automatisch ausgeführt wird, wenn ein bestimmtes Ereignis eintritt.
In unserem Fall wollen wir nach Ablauf des Timers die LED1 anschalten.

### Schritte

1. **Definiere eine neue Callback-Funktion oberhalb der `main`-Funktion:**

   ```c
   void led_callback(void *argument)
   {
       (void) argument; /* Das Argument wird nicht verwendet. */
       LED1_ON;         /* LED1 einschalten */
   }
   ```

   Diese Funktion wird später ausgeführt, sobald der Timer abläuft.

2. **Erstelle und konfiguriere eine neue Timer-Variable innerhalb der `main`-Funktion:**
   Die Callback-Funktion wird mit dem Timer verknüpft.

   ```c
   ztimer_t led_timeout;                 /* Neuen Timer erstellen */
   led_timeout.callback = led_callback;  /* Funktion festlegen, die ausgeführt wird */
   ```

3. **Setze den Timer so, dass er nach 5 Sekunden auslöst:**

   ```c
   ztimer_set(ZTIMER_SEC, &led_timeout, 5);
   ```

   - `ztimer_set` legt fest, dass der Timer nach 5 Sekunden abläuft.
   - Sobald das passiert, wird die `led_callback`-Funktion automatisch ausgeführt.

4. **Baue und flashe die Anwendung erneut:**

   ```sh
   make all flash term
   ```
  
   Jetzt sollten folgende Dinge passieren:

   - LED0 blinkt weiterhin wie zuvor.
   - LED1 wird nach 5 Sekunden eingeschaltet.
