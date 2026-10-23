# IEEE 802.15.4 receive

IEEE 802.15.4 is a low-power, low-data-rate wireless communication protocol. It is designed to be a rather simple protocol, and is therefore easy to implement. In this example, we will implement a simple receiver that receives data from a transmitter (see [15-ieee802154-transmit](../15-ieee802154-transmit/README.md)).

In RIOT, we can easily implement an IEEE 802.15.4 receiver by using the `gnrc` module. This example implements a simplest IEEE 802.15.4 receiver. Together with [15-ieee802154-transmit](../15-ieee802154-transmit/README.md), we will be able to establish communication between two devices.

The goal of this example is to help understand how to achieve communication through IEEE 802.15.4 in RIOT.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../16-ieee802154-receive
```

## Task 1
**Necessary configurations. They should match with the transmitter.**

To receive data from a dedicated transmitter, we must use the same configurations on the receiver side, such as PAN ID and channel. Different from the transmitter, the receiver does not need a parameter to achieve a periodic transmission/reception of data. Instead, it needs to register a message queue to receive packets. As a result, we also define a message queue size.

By implementing the following code, we complete the configurations on the receiver side.

```C
#define APP_PAN_ID          (0x1234)
#define APP_CHANNEL         (26)
#define APP_MSG_QUEUE_SIZE  (8)
```

## Task 2
**Print the received data.**

After receiving a data, we can see the received data by printing it. This is also to compare with the transmitted data in the [15-ieee802154-transmit](../15-ieee802154-transmit/README.md) example.

The code below prints the received data.

```C
rx_count++;
printf("[RX #%d] data=\"", rx_count);
for (int i = 0; i < pkt->size; i++) {
    char c = ((char *)pkt->data)[i];
    putchar((c >= 32 && c < 127) ? c : '.');
}
puts("\"");
```

After implementing all the above code, an IEEE 802.15.4 receiver should be ready. With both a transmitter ([15-ieee802154-transmit](../15-ieee802154-transmit/README.md)) and a receiver ([16-ieee802154-receive](../16-ieee802154-receive/README.md)) running on two boards, we should be able to achieve a simple communication between the two devices.
