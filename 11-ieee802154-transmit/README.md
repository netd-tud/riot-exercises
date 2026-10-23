# IEEE 802.15.4 transmit

IEEE 802.15.4 is a low-power, low-data-rate wireless communication protocol. It is designed to be a rather simple protocol, and is therefore easy to implement. In this example, we will implement a simple transmitter that sends data to a receiver.

In RIOT, we can easily implement an IEEE 802.15.4 transmitter by using the `gnrc` module. This example implements a simplest IEEE 802.15.4 transmitter. Together with [16-ieee802154-receive](../16-ieee802154-receive/README.md), we will be able to establish communication between two devices.

The goal of this example is to help understand how to achieve communication through IEEE 802.15.4 in RIOT.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../15-ieee802154-transmit
```

## Task 1
**Necessary configurations. They should match with the receiver.**

There are certain configurations that need to be defined to realize the IEEE 802.15.4 communication, such as PAN ID and channel. The PAN ID is a unique identifier for a network. The channel relates to the radio frequency. This example achieves a periodic transmission of data, thus we also define a transmission interval.

By implementing the following code, we complete the configurations.

```C
#define APP_PAN_ID          (0x1234)
#define APP_CHANNEL         (26)
#define APP_SEND_INTERVAL   (1000) // in milliseconds
```

## Task 2
**Form an IEEE 802.15.4 packet (header + payload = packet).**

Each communication protocol has its own data format. To achieve the communication, we need to form an IEEE 802.15.4 packet (header + payload = packet). `gnrc` module provides functions to help us achieve this, such as `gnrc_netif_hdr_build()`, `gnrc_pktbuf_add()`, and `gnrc_pkt_prepend()`. They are used to build a packet header, add a payload, and combine the header and payload.

The following code achieves the packet formation.

```C
/* Build and set packet header */
header = gnrc_netif_hdr_build(NULL, 0, NULL, 0);
gnrc_netif_hdr_t *hdr = header->data;
hdr->flags |= GNRC_NETIF_HDR_FLAGS_BROADCAST;
gnrc_netif_hdr_set_netif(hdr, iface);
/* Format packet payload */
payload = gnrc_pktbuf_add(NULL, data, len, GNRC_NETTYPE_UNDEF);
/* Combine header and payload */
packet = gnrc_pkt_prepend(payload, header);
```

## Task 3
**Print the transmitted data.**

After transmitting a data, we can see the transmitted data by printing it. This is also to compare with the received data in the [16-ieee802154-receive](../16-ieee802154-receive/README.md) example.

The code below prints the transmitted data.

```C
printf("[TX #%d] data=\"%s\"\n", seq, buf);
```

After implementing all the above code, an IEEE 802.15.4 transmitter should be ready.
