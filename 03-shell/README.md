# System-Shell

Wie du bereits in **Aufgabe 1** des ersten Tutorials gelernt hast, ist ein **Terminal** ein Programm auf deinem Computer, in das du Textbefehle eingibst, anstatt mit der Maus zu klicken.
Dort siehst du eine Eingabeaufforderung, die etwa so aussieht:

```sh
pi@riot-raspi-0ac8c0:~/workspace $
```

Das Terminal ist also das Fenster, über das du mit deinem Computer oder deinem RIOT-Board kommunizierst.
Die **System-Shell** ist das Programm, das im Hintergrund läuft und deine eingegebenen Befehle interpretiert und ausführt.

Wenn du einen Text eintippst, zerlegt die Shell diesen automatisch in einzelne Wörter:

- Das **erste Wort** ist der **Befehl**, also das, was ausgeführt werden soll.
- Alle **weiteren Wörter** sind die **Argumente**, das sind zusätzliche Informationen, die dem Befehl mitgegeben werden (zum Beispiel ein Dateiname oder eine Zahl).

Die Shell schaut dann in einer Liste nach, ob sie den Befehl kennt. Wenn ja, startet sie die dazugehörige Funktion im Programm und gibt ihr die Argumente weiter.

## Vorbereitung: In den richtigen Ordner wechseln

Um in den Ordner der aktuellen Übung zu wechseln, benutze folgenden Befehl im Terminal:

```sh
cd ../03-shell
```

## Eigene Befehle erstellen

Um einen neuen Befehl zu der RIOT Shell hinzuzufügen, musst du zwei Dinge tun:

### Schritt 1: Den Befehl registrieren

Zuerst meldest du den Befehl mit dem sogenannten `SHELL_COMMAND`-**Makro** an.
Ein **Makro** ist eine Art Abkürzung im Code. Es steht für eine längere Anweisung, die automatisch vom Computer ergänzt wird. So sieht das aus:

```c
SHELL_COMMAND(echo, "Gibt eine Nachricht aus", echo_command);
```

Die drei Angaben in den Klammern bedeuten:

- `echo`: der Befehl, den du später im Terminal eintippst.
- `"Gibt eine Nachricht aus"`: ein kurzer Hilfetext, der dem Nutzer erklärt, was der Befehl macht.
- `echo_command`: der Name der Funktion in deinem Code, die beim Aufruf des Befehls ausgeführt wird.

### Schritt 2: Die Funktion definieren

Als Nächstes schreibst du die Funktion, die beim Aufrufen des Befehls ausgeführt werden soll:

```c
int echo_command(int argc, char **argv)
{
  /* Hier passiert die Magie */
}
```

Jede solche Funktion bekommt immer zwei Informationen:

- **`argc`** (*argument count*, Anzahl der Argumente): eine Zahl, die angibt, wie viele Wörter die Shell insgesamt gezählt hat (Befehl + alle Argumente).
- **`argv`** (*argument values*, Werte der Argumente): eine Liste, in der all diese Wörter gespeichert sind.

Am Ende gibt die Funktion eine Zahl zurück:
- `0` bedeutet: alles hat geklappt.
- Jede andere Zahl zeigt an, dass etwas schiefgelaufen ist.

### Beispiel: So teilt die Shell eine Eingabe auf

Angenommen, du tippst im Terminal Folgendes ein:

```sh
echo nachricht jetzt
```

Die Shell teilt das dann so auf:

| Position | Inhalt        | Bedeutung        |
|----------|---------------|------------------|
| 1        | `echo`        | Der Befehl       |
| 2        | `nachricht`   | Das 1. Argument  |
| 3        | `jetzt`       | Das 2. Argument  |

In deiner Funktion ist `argc` dann `3` und `argv` enthält die drei Wörter: `{ "echo", "nachricht", "jetzt" }`.

**Hinweis:** Soll ein Argument ein Leerzeichen enthalten, setze es in Anführungszeichen. Zum Beispiel:

```sh
echo "nachricht jetzt"
```

Dann gilt alles zwischen den Anführungszeichen als ein einziges Argument, und `argc` ist nur `2`.

## Aufgabe 1: Den Echo-Befehl testen

1. **Baue das Programm und verbinde dich mit dem Board:**

   ```sh
   make all flash term
   ```

2. **Zeige alle verfügbaren Befehle an:**

   ```sh
   help
   ```

3. **Teste den Echo-Befehl:**

   ```sh
   echo "Das ist RIOT!"
   ```

---

## Aufgabe 2: Erstelle den Befehl `toggle`

In dieser Aufgabe erstellst du einen neuen Befehl namens `toggle`, der die LED 0 auf deinem Board ein- oder ausschaltet.

### Schritt 1: Die Funktion schreiben

Füge folgende Funktion in deinen Code ein:

```c
int toggle_command(int argc, char **argv)
{
  /* Prüfen, ob der Befehl richtig aufgerufen wurde (es darf nur 1 Wort geben) */
  if (argc != 1) {
      printf("Benutzung: %s\n", argv[0]);
      return 1;
  }

  /* LED 0 umschalten */
  LED0_TOGGLE;
  return 0;
}
```

Die Überprüfung am Anfang stellt sicher, dass der Befehl ohne zusätzliche Argumente aufgerufen wird.
Falls doch etwas mitgegeben wurde, gibt das Programm einen Hinweis zur richtigen Benutzung aus und bricht ab (`return 1`).

### Schritt 2: Die Hardware-Informationen einbinden

Damit das Programm weiß, was eine LED ist und wie es sie ansprechen soll, musst du eine sogenannte **Bibliothek** einbinden.
Eine **Bibliothek** ist eine Sammlung fertiger Code-Bausteine, die du in deinem Programm nutzen kannst.
Füge ganz oben in der Datei `main.c` diese Zeile ein:

```c
#include "board.h"
```

### Schritt 3: Den Befehl registrieren

```c
SHELL_COMMAND(toggle, "Schaltet LED 0 um", toggle_command);
```

### Schritt 4: Programm übertragen und testen

Führe im Terminal folgenden Befehl aus und gib danach `toggle` ein:

```sh
make all flash term
```

## Aufgabe 3: Den Befehl erweitern

Jetzt erweiterst du den `toggle`-Befehl so, dass du selbst angeben kannst, welche LED umgeschaltet werden soll – LED 0 oder LED 1.

1. **Die Überprüfung anpassen**

   Da du nun ein Argument angeben musst (z. B. `toggle 1`), muss `argc` jetzt den Wert `2` haben (Befehl + eine Zahl). Passe die Überprüfung entsprechend an:

   ```c
   if (argc != 2) {
     printf("Benutzung: %s <led_nummer>\n", argv[0]);
     return 1;
   }
   ```

2. **Text in eine Zahl umwandeln**

   Das Argument, das du eingibst, wird von der Shell zunächst als Text gespeichert – also zum Beispiel als die Zeichenkette `"1"` und nicht als die Zahl `1`. Um damit rechnen zu können, musst du den Text in eine Zahl umwandeln. Dafür gibt es die Funktion **`atoi`** (sprich: "a-to-i", kurz für *ASCII to integer*: Text in ganze Zahl umwandeln).

   Damit du `atoi` verwenden kannst, bindest du zunächst eine weitere Bibliothek ein. Füge diese Zeile oben in `main.c` hinzu:

   ```c
   #include <stdlib.h>
   ```

   Wandle dann in deiner Funktion das Argument in eine Zahl um:

   ```c
   int nummer = atoi(argv[1]);
   ```

3. **Logik einbauen**

   Nutze eine `if`-Abfrage, um je nach dem Wert von `nummer` die richtige LED umzuschalten:

   - Wenn `nummer` gleich `0` ist, rufe `LED0_TOGGLE` auf.
   - Wenn `nummer` gleich `1` ist, rufe `LED1_TOGGLE` auf.

4. **Testen**

   Baue das Programm neu und teste es mit:

```sh
toggle 1
```
