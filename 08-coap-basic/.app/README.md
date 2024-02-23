# Server for tasks 4 and 5 of exercise 8

This will expose a CoAP resource with the temperature readings and another with
humidity.

**Note**: For both tasks set a border router and make sure that everyone is
getting a global IP address.

In addition to the border router, flash two `feather-nrf52840-sense` nodes, one with task
4 application and one with task 5. You can select which application you are
building in the Makefile.

## Task 4
Flash the task 4 application. All resources will be exposed on the
`/.well-known/core`. For this task you will need to use the `ifconfig` command
to get your global IP address. Share this address with the rest.

## Task 5
For this task you will need aiocoap. Install it using pip:
```sh
$ pip3 install --upgrade "aiocoap[all]"
```

Initiate a resource directory on the computer connected to the border router.
Bind it to the assigned IP (normally `fd00:dead:beef::1`):
```sh
$ aiocoap-rd --bind [fd00:dead:beef::1]
```

Share the IP address where the resource directory is bound with the others.
**Make sure it is reachable to all.**

Flash the task 5 application to the node. Make sure you can reach the
computer by pinging it:
```sh
> ping fd00:dead:beef::1
```

Use the shell to register to the resource directory:
```sh
> cord_ep register [fd00:dead:beef::1]
```

**Potential pitfall**: when they are getting information from the lookup resource, make sure that
the trailing `/` is also included in the path.
