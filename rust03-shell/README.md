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
$ cd ../rust03-shell
```

## Implementing commands

Let's see an example. To add a new command we need to do two things. First, we
register our command using the `SHELL_COMMAND` macro:
```rust
riot_wrappers::static_command!(static_echo, "echo", "Echo message", echo);
```

The arguments are a symbol name for the command,
the command name itself (i.e. the string the shell will use to compare to the user input).
Then comes a help message (`"Echo a message"`),
and finally the function that handles the command.

Secondly, we define a handler function. In our example we would define:

```rust
pub fn echo<'a>(_w: &mut impl Write, args: impl IntoIterator<Item=&'a str>) {
  /* do something cool */
}
```

The handler function receives two arguments:

- `w`: an output writer. By using `write!` on it rather than `print!`,
  we can later use the same function where its output is sent to other places
  than stdout.
- `args`: an iterator over the arguments that were passed in.

Let's imagine the user enters the following:
```sh
> echo message now
```
The shell module will split it into:
- "echo" (command)
- "message" (first argument)
- "now" (second argument)

When our `echo_command` function gets called, `w` will be standard out,
and `args` produces three string values: "echo", "message" and "now".

Note that if the user wants to enter a string containing spaces as a single
argument, it should be wrapped in `" "`. For example:
```sh
> echo "message now"
```
This would be parsed as only two substrings.
Now `args` would produce two values, "echo" and "message now".

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
```rust
pub fn toggle<'a>(w: &mut impl Write, args: impl IntoIterator<Item=&'a str>) {
    let mut args = args.into_iter();
    let commandname = args.next().expect("argv is always present");

    if args.next().is_some() {
        writeln!(w, "Usage: {commandname}").unwrap();
        return;
    };

    use switch_hal::ToggleableOutputSwitch;
    riot_wrappers::led::LED::<0>::new().toggle().unwrap();
}
```

**2. Register the command:**
```rust
riot_wrappers::static_command!(static_toggle, "toggle", "Toggle LED 0", toggle);
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

**1. As the command now takes 1 argument, we need to adapt the initial check in the `toggle` function.**
**Also, adapt the usage `printf` to indicate that an argument is required (`"usage: {} <led_number>"`).**

```rust
    let (Some(number), None) = (args.next(), args.next()) else {
      ...
    }
```

**2. Based on the value of `number`, toggle the right LED.**
**You can also update the command help message to indicate the new functionality.**

Note that the number of the LED is a const generic (`::<0>`).

Therefore, there is no benefit to turning the string in `number` into a numeric value;
instead, we can match on it right away:

```rust
match number {
  "0" => riot_wrappers::led::LED::<0>::new().toggle().unwrap(),
  "1" => ...,
  _ => {
    // write an error message
  }
}
```

**4. Build flash and open the serial communication with the board:**
```sh
$ make all flash term
```

**5. Test your new command:**
```sh
> toggle 1
```
