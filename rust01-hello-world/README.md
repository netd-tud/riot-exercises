# Hello World!

This is a basic example how to use RIOT in your embedded application.
It prints out a variation of the famous text `Hello World!`.
The code itself may look like your usual Rust beginners hello-world example.

## Task 1
Build the application, flash the firmware and connect to the board through the serial port.

**1. Open a terminal and navigate to the application directory.**

```sh
$ cd exercises/rust01-hello-world
```

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../rust01-hello-world
```

**2. Build the application by executing GNU Make.**
```sh
$ make all
```

**3. Connect your board using the USB cable.**

**4. Flash the built firmware image into the board's memory.**
```sh
$ make flash
```
**5. Initiate a serial communication with the board.**
```sh
$ make term
```

**6. Reset your board by pressing the 'Reset' button. You should see the "Hello Rust!" message.**

```sh
main(): This is RIOT! (Version: 2024.01)
Hello Rust!
You are running RIOT on a(n) feather-nrf52840-sense board.
```

**7. To exit the serial terminal program, press Ctrl + C.**

Optionally, you can run all the commands above as a single line:
```sh
$ make all flash term
```

## Intro to RIOT build system
This example should foremost give you an overview how to use the Makefile system.
Let's take a look at the `Makefile` file in the application directory:

* First you must give your application a name, which is commonly the same as the name of the directory it resides in.

* Then you can define a default BOARD for which the application was written.
  By using e.g. `make BOARD=nrf52840dk` you can override the default board.

* The variable `RIOTBASE` contains an absolute or relative path to the directory where you have checked out RIOT.
  If your code resides in a subdirectory of RIOT, then you can use `$(CURDIR)` as it's done in here.

* The variable `QUIET`, which is either `1` or `0`, defines whether to print verbose compile information, or hide them, respectively.

* The Rust block tells RIOT that this requires Rust to build,
  and adds the crate stored in the current directory as a module.

  The Rust build system is configured in `Cargo.toml`.

  Most entries there are the same as with any crate -- package name, version etc.

  As the Rust code is linked by the RIOT build system, we build a static library crate;
  consequently, our code lives in `src/lib.rs`.
  Nonetheless, our application will have some kind of a main function.

* The last line of your Makefile must be `include $(RIOTBASE)/Makefile.include`.
