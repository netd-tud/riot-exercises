# SAUL: Interaktion mit Sensoren und Aktoren

SAUL ist eine generische Schnittstelle für Aktoren und Sensoren in RIOT. Ihr Ziel ist es, eine einheitliche Interaktion mit einer Vielzahl von Sensoren und Aktoren durch eine Reihe definierter Zugriffsfunktionen und eine gemeinsame Datenstruktur zu ermöglichen.

- **Sensoren** sind Geräte, die etwas messen, zum Beispiel Temperatur oder Helligkeit.
- **Aktoren** sind Geräte, die etwas ausführen, zum Beispiel eine LED, die leuchtet.

Jedes Gerät, das SAUL nutzt, trägt sich in eine zentrale Liste ein: das sogenannte SAUL-Register.
Ein Register ist hier einfach eine geordnete Liste, in der alle angeschlossenen Geräte vermerkt sind.
Dort können Geräte gefunden, aufgelistet und benutzt werden.
Jedes Gerät gibt dabei seinen Namen und seinen Typ an, zum Beispiel „Temperatursensor“.
Diese Informationen können für die automatische Suche und Zuordnung von Geräten verwendet werden (z. B. automatisches Verbinden eines Lichtsensors mit der Farbe einer RGB-LED).

Um mehr über SAUL und das Register zu erfahren, sieh dir die Online-Dokumentation [hier](https://api.riot-os.org/group__drivers__saul.html) und [hier](https://api.riot-os.org/group__sys__saul__reg.html) an.

## Vorbereitung: In den richtigen Ordner wechseln

Um in den Ordner der aktuellen Übung zu wechseln, benutze folgenden Befehl im Terminal:

```sh
cd ../04-saul
```

## Aufgabe 1

Teste die Anwendung und überprüfe, ob die rote LED eingeschaltet wird, wenn die gemessene Temperatur den Schwellenwert von 24 °C überschreitet.

**Hinweis**: Berühre den Temperatursensor auf deinem Board, um die gemessene Temperatur zu ändern (es ist die kleine graue Box rechts von der RGB-LED).

1. **Baue und flashe die Anwendung.**

   ```sh
   make all flash term
   ```

## Aufgabe 2

Ändere die Anwendung so, dass die rote LED leuchtet, wenn die Temperatur 25 °C überschreitet.

**1. Ändere den Schwellenwert.**

**2. Baue und flashe die Anwendung erneut.**

### Phydat

Das `phydat`-Modul bietet eine einheitliche Sicht auf physikalische Daten in RIOT. Diese Daten sind typischerweise die Ausgabe von Sensorwerten, Datenaggregation und auch die Eingabe für Aktoren.

Die Struktur ist wie folgt definiert:

```C
typedef struct {
    int16_t val[PHYDAT_DIM];    /**< die 3 generischen Dimensionen der Daten */
    uint8_t unit;               /**< die (physikalische) Einheit der Daten */
    int8_t scale;               /**< der Skalierungsfaktor, 10^*scale* */
} phydat_t;
```

Eine `phydat_t`-Struktur enthält Daten einer bestimmten physikalischen Größe. Abhängig von der Größe (bestimmt durch das `unit`-Feld) werden 1, 2 oder 3 Positionen des `val`-Arrays verwendet. Ein Feuchtigkeitswert würde nur eine Dimension benötigen, während eine Position oder Beschleunigung drei Dimensionen erfordern könnte. Die Skalierung wird als Zehnerpotenz (10 ^ Faktor) ausgedrückt.

Schauen wir uns ein Beispiel aus unserer Anwendung an. Wenn wir eine Temperaturmessung durchführen, übergeben wir einen Zeiger auf eine `phydat_t`-Struktur, in der das Ergebnis gespeichert wird.

```C
phydat_t result;
int dimensions = saul_reg_read(temperature_device, &result);
```

Die Funktion `saul_reg_read` gibt die Anzahl der Dimensionen zurück, die der gelesene Wert belegt (d. h. wie viele Positionen des `val`-Arrays gültig sind). Angenommen, die aktuelle Temperatur beträgt 25,5 °C. Da wir einen Temperaturwert lesen, wird `dimensions` den Wert `1` enthalten. Wenn wir den Wert von `result.unit` überprüfen, wird er `UNIT_TEMP_C` entsprechen. Im speziellen Fall unseres Sensors wird der Wert mit einem Faktor von `10^-2` ausgedrückt, sodass `scale` den Wert `-2` enthält. Der in `val[0]` gespeicherte Wert wird `2550` sein.

Für weitere Informationen über `phydat`, wie Einheiten und bereitgestellte Funktionen, sieh dir die [Online-Dokumentation](https://api.riot-os.org/group__sys__phydat.html) an.

## Aufgabe 3

Ändere die Anwendung so, dass auch Werte vom eingebauten Beschleunigungssensor gelesen werden.

1. **Finde den eingebauten Beschleunigungssensor, indem du nach einem Gerät des Typs `SAUL_SENSE_ACCEL` suchst, mit `saul_reg_find_type`.**

    ```C
    saul_reg_t *accel_sensor = saul_reg_find_type(SAUL_SENSE_ACCEL);
    ```

2. **Ändere die Hauptschleife, um eine Messung mit dem Beschleunigungssensor durchzuführen. Gib den gelesenen Wert mit `phydat_dump` aus:**

    ```C
    /* einen Beschleunigungswert vom Sensor lesen */
    phydat_t acceleration;
    int acc_dim = saul_reg_read(accel_sensor, &acceleration);
    if (acc_dim < 1) {
        puts("Error reading a value from the device");
        break;
    }

    phydat_dump(&acceleration, acc_dim);
    ```

3. **Baue und flashe die Anwendung.**

## Aufgabe 4

Erkenne, wenn dein Board um 180° gedreht wurde, und schalte die (blaue) `LED1` ein.

1. **Überprüfe den aktuellen Wert der Beschleunigung auf der Z-Achse.**

   Um auf den Wert der Z-Dimension zuzugreifen, verwende `acceleration.val[2]`.
   Berücksichtige bei Vergleichen Messfehler (z. B. +- 100 mG).

2. **Wenn der Wert den Schwellenwert überschreitet, schalte die LED ein (`LED1_ON`).**

   Vergiss nicht, die LED auszuschalten (`LED1_OFF`), wenn der Wert den Schwellenwert nicht überschreitet.

3. **Baue und flashe die Anwendung.**
