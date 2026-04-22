# Hallo Welt

In diesem Beispiel lernst du, wie du RIOT in deinem eigenen Programm verwenden kannst.
Das Programm gibt den bekannten Text `Hallo Welt!` aus.
Der Code sieht ähnlich aus wie ein typisches Einsteigerbeispiel in der Programmiersprache *C*.

## Aufgabe 1 - Lerne dein Terminal kennen

Ein **Terminal** ist ein Programm auf deinem Computer, in das du Textbefehle eingeben kannst, anstatt mit der Maus zu klicken.
Du siehst es in der unteren Hälfte deines Bildschirms:

```sh
pi@riot-raspi-0ac8c0:~/workspace $
```

Mit dem Terminal kannst du zum Beispiel:

- Dateien und Ordner öffnen (`cd`)
- Programme starten (`make`)
- Mit Geräten (wie deinem Board) kommunizieren

Um einen Befehl auszuführen, tippst du ihn auf der Tastatur ein und drückst dann *Enter*.
Probiere es gleich einmal mit dem `echo`-Befehl aus. Klicke dazu in das Terminal und gib ein:

```sh
echo Hallo!
```

Schau, was passiert, wenn du statt `Hallo!` einen anderen Text eingibst.
Wir werden das Terminal nun weiter nutzen, um mit deinem Board zu arbeiten.

## Aufgabe 2 - Baue das Programm und spiele es auf dein Board

In dieser Aufgabe baust du die Anwendung, spielst das Programm auf das Board und verbindest dich über die serielle Schnittstelle.
Die **serielle Schnittstelle** ist die Kabelverbindung, über die dein Computer mit dem Board kommuniziert.

1. **Wechsle im Terminal in das Anwendungsverzeichnis:**
   Das Terminal hat immer einen aktuellen *Pfad*, der zeigt, in welchem Ordner du dich gerade befindest.
   Mit dem Befehl `cd` kannst du in einen anderen Ordner wechseln. Gib ein:

   ```sh
   cd aufgaben/01-hallo-welt
   ```

   Falls du dich in einem anderen Ordner befindest, kannst du mit folgendem Befehl in den richtigen Ordner wechseln:

   ```sh
   cd ../01-hallo-welt
   ```

   - Das `/` trennt Ordner und Unterordner.
   - Mit `../` wechselst du in den übergeordneten Ordner.

2. **Baue die Anwendung:**
   Führe den Befehl `make all` aus, um das Programm zu bauen.
   Dabei wird der C-Code in eine ausführbare Datei umgewandelt:

   ```sh
   make all
   ```

3. **Verbinde dein Board:**
   Schließe dein Board mit einem USB-Kabel an deinen Computer an.

4. **Spiele das Programm auf das Board:**
   Mit dem Befehl `make flash` wird das Programm in den Speicher des Boards geladen:

   ```sh
   make flash
   ```

5. **Starte die serielle Verbindung:**
   Mit dem Befehl `make term` verbindest du dich mit dem Board und kannst die Ausgaben des Programms sehen:

   ```sh
   make term
   ```

6. **Setze dein Board zurück:**
   Drücke den „Reset“-Knopf auf deinem Board. Du solltest die folgende Ausgabe sehen:

   ```sh
   Hallo Welt!
   Du führst RIOT auf einem adafruit-feather-nrf52840-sense Board aus.
   Dieses Board verfügt über einen nrf52-Mikrocontroller.
   ```

7. **Beende die serielle Verbindung:**
   Um das Terminal zu verlassen, drücke die Tastenkombination `Strg + C`.

**Tipp:** Du kannst alle oben genannten Befehle in einer einzigen Zeile ausführen:

```sh
make all flash term
```

---

## Aufgabe 3 - Der Programmcode

Den Programmcode deiner Anwendung findest du im Ordner _01-hallo-welt_ in der Datei `main.c`.

```c
int main(void)
{
    printf("Hallo Welt!");

    printf("Du führst RIOT auf einem %s Board aus.\n", RIOT_BOARD);
    printf("Dieses Board verfügt über einen %s-Mikrocontroller.\n", RIOT_CPU);

    return 0;
}
```

### Erklärung

- Die `main`-Funktion ist der Einstiegspunkt des Programms. Sie wird automatisch von RIOT aufgerufen, sobald das Board startet.
- Mit der Funktion `printf` kannst du Text ausgeben. Dieser Text wird über die serielle Schnittstelle an deinen Computer gesendet und im Terminal angezeigt.
- Die Konstanten `RIOT_BOARD` und `RIOT_CPU` enthalten Informationen über die Hardware und werden von RIOT automatisch festgelegt, wenn du das Programm baust.
- Mit `return 0` wird das Programm beendet. Die Null signalisiert, dass das Programm erfolgreich ausgeführt wurde.

## Aufgabe 4 - Teste deinen eigenen Text

Füge oberhalb von `return 0` eine eigene Ausgabe hinzu.
Nutze dazu die Funktion `printf` und schreibe eine beliebige Nachricht.
Beachte, dass der Text immer in Anführungszeichen (`"..."`) stehen muss.

Beispiel:

```c
printf("Das ist mein erster eigener Text!\n");
```

Baue dann erneut das Programm, lade es auf dein Board und überprüfe die Ausgabe:

```sh
make all flash term
```
