# sock

To implement application protocols, RIOT provides the `sock` APIs to abstract away network stacks.
As an example we will look at `sock_udp` for sending and receiving UDP packets.

The most important operations are defined in `net/sock/udp.h`

- `sock_udp_create()` to initialize a `sock` object.
- `sock_udp_close()` to close an initialized `sock` object.
- `sock_udp_recv()` to receive on an initialized `sock` object.
- `sock_udp_send()` to send. A `sock` object is optional.

There is also functionality to use it with the [`event` API](../07-events) which we will go into
later.

We provide a very simple skeleton `main.c`. The lines of interest in the
[`Makefile`](./Makefile) are the following to include `sock_udp` itself, `event` support for `sock`,
and some helper functions, e.g., for endpoint parsing, from `sock_util`.

```Makefile
# add UDP sock support for GNRC
USEMODULE += sock_udp

# add event support for sock
USEMODULE += sock_async_event

# Helper functions for sock
USEMODULE += sock_util
```

## Task 1

Implement a simple UDP client.

As long as an endpoint is provided, `sock_udp_send()` does not require an initialized `sock` object.

```C
int res
sock_udp_ep_t remote = { 0 };

if (sock_udp_str2ep(&remote, addr_str) < 0) {
    puts("Unable to parse destination address");
    return 1;
}

if((res = sock_udp_send(NULL, data, data_len, &remote)) < 0) {
    puts("could not send");
    return 1;
}
else {
    printf("Success: send %u byte to %s\n", (unsigned) res, addr_str);
    return 0;
}
```

Using that, implement a command `udp` that expects an address/port pair as its first argument and a
string message as its second argument that sends the message to the given address/port pair via UDP.

You can use `sock_udp_str2ep()` from `net/sock/util.h` to parse an address/port pair into a `sock`
endpoint. It expects the following format: `[fe80::dead:co:ffee%4]:1234` with `fe80::dead:co:ffee`
being an IPv6 address, `4` being the interface to send over (due to `gnrc_netif_single` this is
optional), and `1234` being the port.

Coordinating with another group, you can use the application [`example/gnrc_networking` in
`RIOT`](../RIOT/examples/gnrc_networking) to test this.

## Task 2

Implement a simple UDP server.

A `sock` is bound to a port (and possibly also address) using the `sock_udp_create()` function:

```C
sock_udp_t sock;
sock_udp_ep_t server = { .port = atoi(port_str), .family = AF_INET6 };

if(sock_udp_create(&sock, &server, NULL, 0) < 0) {
    return NULL;
}
```

Once initialized in such a manner, you can receive on the given UDP port using the `sock_udp_recv()`
function:

```C
static char server_buffer[SERVER_BUFFER_SIZE];

while (1) {
    int res;

    if ((res = sock_udp_recv(&sock, server_buffer,
                             sizeof(server_buffer) - 1, SOCK_NO_TIMEOUT,
                             NULL)) < 0) {
        puts("Error while receiving");
    }
    else if (res == 0) {
        puts("No data received");
    }
    else {
        printf("Received: %.*s\n", res, server_buffer);
    }
}
```

Using this, write a shell command `udps` that receives a port as an argument, which starts a thread
that receives UDP messages on that given port. Test this with another group using their
implementation from Task 1.

## Task 3

Use events to implement a UDP server.

`sock` also has support to be used with the `event` API. For that, the `net/sock/async/event.h`
provides the `sock_udp_event_init()` function. `net/sock/async/types.h` defines several event types
for sockets, but the most interesting for us is `SOCK_ASYNC_MSG_RECV` which signals that a message
was received on the `sock` object. Note, that timeout 0 is used with `sock_udp_recv()`. This way,
the function is called non-blocking.

```C
static sock_udp_t sock;
static char server_buffer[SERVER_BUFFER_SIZE];

void handler(sock_udp_t *sock, sock_async_flags_t type, void *arg)
{
    (void)arg;
    if (type & SOCK_ASYNC_MSG_RECV) {
        sock_udp_ep_t remote;
        ssize_t res;
        if ((res = sock_udp_recv(sock, server_buffer,
                                 sizeof(server_buffer) - 1, 0,
                                 &remote)) < 0) {
            puts("Error while receiving");
        }
        else if (res == 0) {
            puts("No data received");
        }
        else {
            printf("Received: %.*s\n", res, server_buffer);
        }
    }
}

/* ... */

int main(void) {
    sock_udp_ep_t server = { .port = 1234, .family = AF_INET6 };
    event_queue_t queue;

    if(sock_udp_create(&sock, &server, NULL, 0) < 0) {
        return NULL;
    }

    event_queue_init(&queue);
    sock_udp_event_init(&sock, &queue, handler, NULL);
    event_loop(&queue);
}
```

With that knowledge, modify your application from Task 2 to use events instead of blocking on
receive.

Last, add a shell command that stops a running UDP server. Use `event_post()` to call an event
handler on the event queue of the `sock`. That event handler in turn can call `sock_udp_close()` on
the `sock` object of the server. Again, test this with another group, using their implementation
from Task 1.
