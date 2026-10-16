# BLE multi-hop central

Multi-hop networks mean to connect multiple devices to each other. This way, information can be tranferred among multiple devices, thus the communication range increases.

In this example, we base ourselves on the [12-ble-central](../12-ble-central/README.md) example, and extend it to be ready for multi-hop communication. Together with [13-ble-multi-hop-peripheral](../13-ble-multi-hop-peripheral/README.md), we will have two peripheral devices connected to one central device, and the data from one peripheral device can be sent to the other peripheral device through the central device.

The goal of this example is to help understand how to implement a BLE central ready for multi-hop communication and how BLE communication achieves certain behaviors, such as forwarding data from one peripheral to another peripheral using `write` characteristic.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../14-ble-multi-hop-central
```

## Task 1
**Allow the device to hold multiple connections.**

By default, NimBLE allows each BLE deivce to hold only one connection. In this example, we need the BLE central device to hold multiple connections to achieve a BLE multi-hop network. For this purpose, we must change a NimBLE configuration. This can be easily done by adding the following line to the `Makefile`. By setting `MYNEWT_VAL_BLE_MAX_CONNECTIONS` to 2, the BLE central device can hold 2 connections.

```C
CFLAGS += -DMYNEWT_VAL_BLE_MAX_CONNECTIONS=2
```

## Task 2
**Discover the write characteristic from the peripheral device.**

The write characteristic has been defined on the peripheral side in the [13-ble-multi-hop-peripheral](../13-ble-multi-hop-peripheral/README.md) example. What we still need to do it to discover the write characteristic from the peripheral device, and save it on the central device. In this task, we will add the following code to the `discover_chr_cb()` function.

```C
else if (ble_uuid_cmp(&chr->uuid.u, &custom_chr_write_uuid.u) == 0) {
    custom_write_data_val_handle = chr->val_handle;
}
```

## Task 3
**Forward the received data to the other connected peripheral device using the write characteristic.**

From the [12-ble-central](../12-ble-central/README.md) example, we know that a central handles the `notify` event from the peripheral device at `case BLE_GAP_EVENT_NOTIFY_RX:`. This does not change in a multi-hop network. Hence, we realize the data forwarding from one peripheral to the other connected peripheral device at `case BLE_GAP_EVENT_NOTIFY_RX:`. This relies on the write characteristic defined on the peripheral side in the [13-ble-multi-hop-peripheral](../13-ble-multi-hop-peripheral/README.md) example.

The code below achieves the data forwarding from one peripheral to the other connected peripheral device.

```C
struct os_mbuf *om;
om = os_mbuf_dup(event->notify_rx.om);
ble_gattc_write_no_rsp(c->bhc_handle, custom_write_data_val_handle, om);
printf("Central: forwarded data --> ");
for (uint8_t i = 0; i < om->om_len; i++) {
    printf("%02x ", om->om_data[i]);
}
printf("\n");
```

After implementing the above code, the central device should be ready for multi-hop communication.

By flashing the [13-ble-multi-hop-peripheral](../13-ble-multi-hop-peripheral/README.md) example to two BLE boards, and the [14-ble-multi-hop-central](../14-ble-multi-hop-central/README.md) example to one BLE board, we should be able to achieve a simple multi-hop network. The two peripheral devices should be connected to the central device, and the data from one peripheral device can be sent to the other peripheral device through the central device.
