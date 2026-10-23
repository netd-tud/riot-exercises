# BLE multi-hop peripheral

Multi-hop networks mean to connect multiple devices to each other. This way, information can be tranferred among multiple devices, thus the communication range increases.

In this example, we base ourselves on the [11-ble-peripheral](../11-ble-peripheral/README.md), and extend it to be ready for multi-hop communication. Together with [14-ble-multi-hop-central](../14-ble-multi-hop-central/README.md), we will have two peripheral devices connected to one central device, and the data from one peripheral device can be sent to the other peripheral device through the central device.

The goal of this example is to help understand how to implement a BLE peripheral ready for multi-hop communication and how BLE communication achieves certain behaviors, such as receiving `write` data from central.

We provide a very simple skeleton `main.c` and `Makefile`. The Makefile is quite similar to the [Makefile](../11-ble-peripheral/Makefile) of [11-ble-peripheral](../11-ble-peripheral/README.md) example. Thus, the explanation is not repeated.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../13-ble-multi-hop-peripheral
```

## Task 1
**Define a UUID for a write characteristic.**

We have already learned `notify` in the [11-ble-peripheral](../11-ble-peripheral/README.md) example. In this example, we will define a write characteristic. `write` is used to send data from the central device to the peripheral device. In short, if data needs to be sent from peripheral to central, the peripheral should `notify`. If data needs to be sent from central to peripheral, the central should `write`. They both rely on the characteristics defined on the peripheral device.

The code below defines a UUID for the write characteristic on the peripheral device.

```C
#define CUSTOM_CHR_WRITE_UUID                   0xee01
```

## Task 2
**Define the write characteristic.**

We have defined a UUID for the write characteristic on the peripheral device. However, we have not added the characteristic to the service. In this task, we will add the characteristic to the service.

The following code adds the characteristic to the service.

```C
{
    /* Custom Write Characteristic */
    .uuid = BLE_UUID16_DECLARE(CUSTOM_CHR_WRITE_UUID),
    .access_cb = custom_write_access_cb,
    .val_handle = &custom_write_data_val_handle,
    .flags = BLE_GATT_CHR_F_WRITE,
},
```

- `.uuid` links to the defined UUID for the write characteristic.
- `.access_cb` links to the callback function for the write characteristic.
- `.val_handle` links to the value handle for the write characteristic.
- `.flags` links to the characteristic flags for the write characteristic.

## Task 3
**Print the received data from central if any.**

On the peripheral, the callback function `custom_write_access_cb()` is called when the central device writes data to the write characteristic. As a result, we can access and process the data from the central in this function.

The code below helps show the received data from the central if any.

```C
printf("Peripheral: received data --> ");
for (uint8_t i = 0; i < ctxt->om->om_len; i++) {
    printf("%02x ", ctxt->om->om_data[i]);
}
printf("\n");
```

After implementing the above code, the peripheral device should be ready for multi-hop communication. The later example [BLE multi-hop central](../14-ble-multi-hop-central/README.md) will show how to prepare the central device for multi-hop communication. The goal is to have two peripheral devices connected to one central device, and the data from one peripheral device can be sent to the other peripheral device through the central device.
