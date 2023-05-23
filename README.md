# RIOT in the Internet of Things - Exercises
## Overview

Collection of practical exercises, assignments, and tasks to become familiar
with IoT technologies using Linux and RIOT-OS.

## Getting started

**1. Open a terminal at the local user's home directory**

**2. Clone this repository locally:**
```sh
$ git clone --recurse-submodules https://github.com/smartuni/exercises.git
```

**3. Navigate to the repository and open an editor in the directory:**
```sh
$ cd exercises
$ codium .
```

**4. To avoid installing toolchains, you can use Docker instead. Follow the installation instructions [here](https://doc.riot-os.org/getting-started.html#docker)**

**5. Go through the exercises starting with [`01-hello-world`](./01-hello-world/README.md). Each contains a `README.md` with**
   **detailed instructions and tasks to solve.**


## Conventions
Throughout the tutorials, we will specify commands and outputs.
The conventions are as follows:

**Leading `$` means that the command is executed on the linux shell:**
```sh
$ make
```

**Leading `>` means that the command is executed on the RIOT shell:**
```sh
> help
```

**No symbol means an output from the RIOT node on the terminal:**
```sh
Command              Description
---------------------------------------
echo                 Echo a message
```
