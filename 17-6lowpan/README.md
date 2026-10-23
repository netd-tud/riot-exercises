# 6LowPAN

6LoWPAN (IPv6 over Low-Power Wireless Personal Area Networks) is an open standard and communication protocol developed by the Internet Engineering Task Force (IETF). It allows very small, battery-powered, and resource-constrained devices to connect directly to the internet using IPv6.

In this example, we implement a 6LoWPAN network among multiple devices. We realize multicast communication among these devices, i.e., the data from one device can be sent to all the other devices within the network. The data is sent as a UDP datagram.

The goal of this example is to quickly implement a 6LowPAN network for multicast communication among multiple devices.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../19-6lowpan
```

## Task 1
**Creat a socket object and bind to the PORT (1234).**

RIOT provides many APIs related to sockets, such as `sock_udp_create()`. This function creates a socket object and binds it to a local endpoint. The local endpoint is defined by a port number and a network interface. In this example, we bind the socket to the predefined PORT (1234) in the code on the network interface `SOCK_ADDR_ANY_NETIF`.

The following code creates a socket object and binds it to the PORT (1234) on the network interface `SOCK_ADDR_ANY_NETIF`.

```C
sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
local.port = PORT;
sock_udp_create(&sock, &local, NULL, 0);
```

## Task 2
**Generate, send and print UDP data.**

The API `sock_udp_send()` sends a UDP datagram to a remote endpoint. Similar to a local point, a remote endpoint is defined by a port number and a network interface. There is no need to manually construct a UDP packet, such as the IP header, the UDP header, and the data. Instead, the `sock_udp_send()` API takes care of it automatically. We only need to give the user data to the API, and the library will take care of the rest.

One can easily generate, send, and print UDP data by using the following code.

```C
uint8_t payload[5];
uint8_t random_data = random_uint32_range(0, 256);
for (uint8_t i = 0; i < sizeof(payload); i++) {
    payload[i] = random_data;
}
sock_udp_send(&sock, payload, sizeof(payload), &remote);
printf("TX: ");
for (unsigned i = 0; i < sizeof(payload); i++) {
    printf("%02x ", payload[i]);
}
printf("\n");
```

## Task 3
**Receive and print UDP data.**

The API `sock_udp_recv()` receives a UDP datagram by checking the receive queue of the socket. The received datagram is stored in the buffer provided by the user. We can then print the received datagram.

One can use the following code to receive and print UDP data.

```C
int len = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, NULL);
if (len > 0) {
    printf("RX: ");
    for (int i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}
```

After implementing all the above code, a device should be ready for 6LowPAN communication. Since this example implements both a transmit thread and a receive thread, it allows each device to send and receive data simultaneously. By flashing this example to multiple boards, they can form a 6LowPAN network. Each device sends data to all the other devices in the network, and each device receives data from all the other devices in the network.
