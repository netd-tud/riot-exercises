# System shell

The system shell allows to implement a CLI (command line interface) on your RIOT
node. The shell module waits for commands via the standard input and executes
the corresponding function.

When a new string arrives, the shell module parses the incoming string into
substrings. The first one is the command, the remaining are arguments. The
module will try to find the command string among the registered commands. If it
finds it, it calls the corresponding handler function with the incoming
arguments.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../03-shell
```

## Implementing commands

Let's see an example. To add a new command we need to do two things. First, we
register our command using the `SHELL_COMMAND` macro:
```C
SHELL_COMMAND(echo,"Echo a message", echo_command);
```

The first macro argument (`echo`) is the string to call our command (i.e. the string the shell will
use to compare to the user input). The second (`"Echo a message"`) is a help message to be
printed to the user. The third one (`echo_command`) is the pointer to our handler function.

Secondly, we define a handler function. In our example we would define:

```C
int echo_command(int argc, char **argv)
{
  /* do something cool */
}
```

As you can see, handler functions have a specific signature. They receive
two arguments:

- `argc`: an integer containing the number of substrings the shell module found
- `argv`: the array with the substrings the shell module found (it will be
  `argc` items long).

The return value must be an integer. It should return `0` when no errors occurred, or
`!=0` otherwise.

Let's imagine the user enters the following:
```sh
> echo message now
```
The shell module will split it into:
- "echo" (command)
- "message" (first argument)
- "now" (second argument)

When our `echo_command` function gets called, `argc` will contain the value
`3`, and `argv` will be an array of strings containing: ` { "echo", "message",
"now" }`.

Note that if the user wants to enter a string containing spaces as a single
argument, it should be wrapped in `" "`. For example:
```sh
> echo "message now"
```
This would be parsed as only two substrings.
Now `argc` would have the value `2`, and `argv` would be `{ "echo", "message now" }`.

## Task 1
Test the `echo` command.

**1. Build flash and open the serial communication with the board:**
```sh
$ make all flash term
```

**2. Explore the available commands with `help`:**
```sh
> help
```

**3. Test the echo command:**
```sh
> echo "This is RIOT!"
```

## Task 2

Create a new shell command `toggle`, that toggles the LED 0.

**1. Create a handler function for the command:**
```C
int toggle_command(int argc, char **argv)
{
  /* check that the command is called correctly (no extra arguments) */
  if (argc != 1) {
      printf("usage: %s\n", argv[0]);
      return 1;
  }

  /* toggle the LED */
  LED0_TOGGLE;
  return 0;
}
```

**2. As seen in the previous exercise, to use LEDs we need the `board.h` header:**
```C
/* at the top of main.c */
#include "board.h"
```

**3. Register the command:**
```C
SHELL_COMMAND(toggle, "Toggle LED 0", toggle_command);
```

**4. Build flash and open the serial communication with the board:**
```sh
$ make all flash term
```

**5. Test your new command:**
```sh
> toggle
```

## Task 3

Modify the `toggle` command so it accepts as first argument `0` or `1`, and toggle
the specific LED (using `LED0_TOGGLE` or `LED1_TOGGLE`).

**1. As the command now takes 1 argument, we need to adapt the initial check in the `toggle_command` function.**
**Also, adapt the usage `printf` to indicate that an argument is required (`"usage: %s <led_number>"`).**
```C
if (argc != 2) {
  /* ... */
}
```

**2. The first argument (`argv[1]`) contains the LED number to toggle.**
**To convert a string into a number, make use of the `atoi` function, defined in `stdlib.h` header.**

```C
/* at the top of main.c */
#include <stdlib.h>

/* ... */

/* inside toggle_command, convert the argument to a number */
int number = atoi(argv[1]);
```

**3. Based on the value of `number`, call `LED0_TOGGLE` or `LED1_TOGGLE`.**
**You can also update the command help message to indicate the new functionality.**

**4. Build flash and open the serial communication with the board:**
```sh
$ make all flash term
```

**5. Test your new command:**
```sh
> toggle 1
```
