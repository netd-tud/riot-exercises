# SAUL: interacting with sensors and actuators

SAUL is a generic actuator/sensor interface in RIOT. Its purpose is to enable
unified interaction with a wide range of sensors and actuators through a set of
defined access functions and a common data structure.

Each device driver implementing this interface has to expose a set of predefined
functions, and it has to register itself to the central SAUL registry. From here
devices can be found, listed, and accessed.

Each device exposes its name and type. This information can be used for
automated searching and matching of devices (e.g. connect light sensor
automatically with the color of an RGB LED...).

To learn more about SAUL and the registry, check the online documentation
[here](https://doc.riot-os.org/group__drivers__saul.html) and
[here](https://doc.riot-os.org/group__sys__saul__reg.html).

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../rust04-saul
```

## Task 1

Test the application and verify that when the sensed temperature surpasses the
threshold of 24 °C the red LED turns on.

**Hint**: Touch the temperature sensor on your board
to change the measured temperature (it is the small gray box right of the RGB LED).

**1. Build and flash the application. Open a serial port communication.**
```sh
$ make all flash term
```

## Task 2

Modify the application to light the red LED whenever the temperature surpasses 25 °C.

**1. Modify the threshold value.**

**2. Build and flash the application again.**

## Phydat

The phydat module provides a common view on physical data throughout RIOT.
This data is typically the output from sensor readings, data aggregation, and
also the input for actuators.

The structure is defined as follows:
```C
typedef struct {
    int16_t val[PHYDAT_DIM];    /**< the 3 generic dimensions of data */
    uint8_t unit;               /**< the (physical) unit of the data */
    int8_t scale;               /**< the scale factor, 10^*scale* */
} phydat_t;
```

A `phydat_t` structure contains data of a specific physical magnitude.
Depending on the magnitude (determined by the `unit` field), 1, 2, or 3 of the
positions of the `val` array will be used. A humidity value would require only
one dimension, whereas a position or acceleration may require three dimensions.
The scale is expressed as power of 10 (10 ^ factor).

Rust provides a view on the struct as the [Phydat](https://rustdoc.etonomy.org/riot_wrappers/saul/struct.Phydat.html) type,
which also stores the number of valid elements obtained at read time,
and makes the unit available as an enum.

Let's assume that the current temperature is 25.5 °C. As we are reading a temperature
value, there is a single value, so `.value()` returns a 1-long slice of i16 values.
If we check the value of `result.unit()`, it will equal `Some(Unit::TempC)`. In
the particular case of our sensor, the value is expressed with a factor of
`10^-2`, so `.scale()` will contain the value `-2`. The value stored in
`value()[0]` will be `2550`.

For more information about phydat, like units and provided functions, check the
[online documentation](https://doc.riot-os.org/group__sys__phydat.html).

## Task 3

Modify the application to also read values from the onboard accelerometer.

**1. Find the onboard accelerometer, by searching for a device of type `SAUL_SENSE_ACCEL` using `saul_reg_find_type`.**
```rust
let accel_sensor = RegistryEntry::all()
    .filter(|e| matches!(e.type_(), Some(Class::Sensor(Some(SensorClass::Accel)))))
    // ...
```

**2. Modify the main loop to perform a read with the accelerometer.**
**Print the read value:**
```rust
let accel = accel_sensor
    .read()
    .expect("Error reading acceleration");

println!("Read acceleration: {accel:?}");
```

The `{accel:?}` uses Rust's string formatting macros:
`println!` and similar macros expand variables between `{}`.
The `:?` tells the macro to use [Debug](https://doc.rust-lang.org/std/fmt/trait.Debug.html) formatting --
we need to use it because the "plain" [Display](https://doc.rust-lang.org/std/fmt/trait.Display.html) human-readable format is not available on that type.

**3. Build and flash the application. Open a serial port communication.**

## Task 4

Detect when your board has been flipped 180 ° and turn the (blue) `LED1` on.

**1. Check the current value of the acceleration on the Z axis.**
**To access the value of the Z dimension, use `accel.val()[2]`.**
**When comparing account for measurement error (e.g. +- 100 mG)**

**2. If the value surpasses than the threshold, turn LED 1 on.**
**Don't forget the turn the LED off when the value does not surpass the threshold.**

**3. Build and flash the application. Open a serial port communication.**

## Task 5

SAUL also has actuators; in particular, there is an actuator of type `Some(Class::Actuator(Some(ActuatorClass::LedRgb))`.
Use that indicator to show the spatial orientation of your board.

Before writing, you will need to transform the value range,
because this RGB LED implementation takes values in 0..=255 Device RGB range.
For eye protection, you might use:

```rust
let rgb = [
    (accel.value()[0] / 50).clamp(0, 100),
    (accel.value()[1] / 50).clamp(0, 100),
    (accel.value()[2] / 50).clamp(0, 100),
];
let rgb = Phydat::new(&rgb, None, 0);
```

SAUL actuators have a `.write()` method analogous to sensors `.read()` method.
