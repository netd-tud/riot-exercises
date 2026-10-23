# BLE peripheral

To achieve Bluetooth Low Energy (BLE) communication, RIOT uses [NimBLE](https://github.com/apache/mynewt-nimble) (an open-source BLE stack). This is a simple example to get RIOT to be a BLE peripheral.

There are many functions and operations used in the example to achieve the implementation. However, we do not introduce everything to avoid too much workload or overhead. The goal is to help understand fast how to implement a BLE peripheral and how BLE communication achieves certain behaviors, such as advertising, connecting, and notifying.

We provide a very simple skeleton `main.c`. The lines of interest in the
[`Makefile`](./Makefile) are the following to include modules like `random`, `ztimer_sec`, `nimble_svc_gap`, and `nimble_svc_gatt`. The first two relate to some functions we need during the implementation. The later two modules help achieve the BLE communication. They also ensure certain background behaviors of BLE communication. As shown by the module names, they are from NimBLE.

```Makefile
# Some RIOT modules needed for this example
USEMODULE += random
USEMODULE += ztimer_sec

# Include NimBLE
USEMODULE += nimble_svc_gap
USEMODULE += nimble_svc_gatt
```

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../11-ble-peripheral
```

## Preparation
**1. Flash the current example onto the board, by the following command.**
```
$ make flash
```

**2. Start a terminal to see the output information from the board. Flexibly use the terminal and `printf` to see what the board is doing with BLE.**
```
$ make term
```

**3. Download and install the [nRF Connect for Mobile](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-mobile) app on your phone.**

**4. Use the `scan` function in "nRF Connect for Mobile" to observe the BLE signals in the air, and other functions to play with BLE communication. Flexibly use the app to interact with your board, and check if certain functions have been properly implemented.**

Note that you will not see the device in the app before implementing Task 1.

## Task 1
**Enable the advertisement for the peripheral board.**

Some basic concepts in BLE communication are as follows.

**BLE Peripheral (Server):** The peripheral (server) typically contains data and services that other devices can access. It advertises its services to potential clients.

As the peripheral, to deliver the data to the central, the first step is to advertise our services. In the example code, certain steps are missing in the `advertise()` function. The following code completes the `advertise()` function, thus enables the advertisement.

```C
int rc;
rc = ble_gap_adv_set_fields(&fields);
assert(rc == 0);
rc = ble_gap_adv_start(addr_type, NULL, 100,
                        &adv_params, peripheral_conn_event, NULL);
assert(rc == 0);
```

- `ble_gap_adv_set_fields()` configures the fields to include in BLE advertisements. Certain advertising information and settings, such as device name, Tx power level, and advertising interval, can be included in the fields. In this example, the device discovery mode, device name, and our custom service UUID are set specifically in the `fields`.
- `ble_gap_adv_start()` configures and starts advertising procedure for BLE. Its inputs are advertisement duration, additional advertisement parameters, a callback function that associates with this advertising procedure, etc.

**Hints:** Check the terminal and "nRF Connect for Mobile" app. Try to make a connection between "nRF Connect for Mobile" and your BLE application.

## Task 2
**Add our custom service (already defined in the example) and reload the GATT server.**

Some basic concepts in BLE communication are as follows.

**BLE Services:** These are the top-level groupings of related data on a BLE device, serving a specific function.

**BLE Characteristics:** Within each service, you'll find individual data points called characteristics.

**GATT (Generic Attribute Profile):** BLE uses GATT as a way to define and manage the services and characteristics exposed by a device. 

BLE allows the developers to define custom services and characteristics for their own applications. However, these custom services and characteristics will not be automatically added or loaded by the BLE stack. Thus, the following commands are needed to achieve so.

```C
/* Add our custom services */
rc = ble_gatts_count_cfg(gatt_svr_svcs);
assert(rc == 0);
rc = ble_gatts_add_svcs(gatt_svr_svcs);
assert(rc == 0);

/* Reload the GATT server to link our added services */
rc = ble_gatts_start();
assert(rc == 0);
```

- `ble_gatts_add_svcs()` adds a set of self-defined services.
- `ble_gatts_start()` makes all added services available to other BLE devices.

The difference can be seen in the "nRF Connect for Mobile" app after connecting to the BLE application. An unknown service (i.e., our custom service) shows in the BLE application. By clicking it, we can see our custom characteristic. The UUIDs of the service and characteristics can confirm if they are our custom ones or not.

## Task 3
**Check if someone subscribed to our custom characteristic.**

Some basic concepts in BLE communication are as follows.

**Subscribe:** In BLE, "subscribe" refers to the process where a central device registers with a peripheral device to receive notifications about changes to a specific characteristic. It is achieved by writing a value of 0x0001 to the characteristic’s Client Characteristic Configuration Descriptor (CCCD) on a server (as in the example code).

The BLE stack uses `BLE_GAP_EVENT_SUBSCRIBE` to link and show subscribe events happened in a BLE connection. Hence, by checking it within `peripheral_conn_event()` function, any subscribe events and their related information can be found.

The code below helps show the subscribe events, and related information can be obtained if necessary. For instance, `cur_notify` can tell if the peripheral device is sending notifications or not.

```C
case BLE_GAP_EVENT_SUBSCRIBE:
    notify_state = event->subscribe.cur_notify;
    if (notify_state == 1) {
        printf("Peripheral: Someone subscribed to our custom characteristic\n");
    } else {
        printf("Peripheral: Someone unsubscribed from our custom characteristic\n");
    }
    return 0;
```

The terminal can tell if the subscription is successful or not.

## Task 4
**Achieve a periodic notification in the `while` loop inside the `main()` function.**

In BLE, **notification** is one of the mechanisms for a peripheral to send data to a central device. It is commonly used, so we introduce it here. Other possible mechanisms are indication, read, and write, but they are not further discussed in this example.

```C
uint8_t custom_notify_data[5];
uint8_t random_data = random_uint32_range(0, 256);
printf("Peripheral: data --> ");
for (uint8_t i = 0; i < sizeof(custom_notify_data); i++) {
    custom_notify_data[i] = random_data;
    printf("%02x ", custom_notify_data[i]);
}
printf("\n");
/* Send notification */
struct os_mbuf *om = ble_hs_mbuf_from_flat(custom_notify_data, sizeof(custom_notify_data));
ble_gatts_notify_custom(conn_handle, custom_notify_data_val_handle, om);
```

- `ble_hs_mbuf_from_flat()` puts the data in a buffer.
- `ble_gatts_notify_custom()` sends a characteristic notification.

Note that the data has a size of 5, which is just a random number implemented in this example. Without enabling some BLE features (such as Data Length Extension and MTU Exchange), this number can only go up to 20. With such features, it can go up to 244 or 247, depending on the BLE version and the enabled features.

After implementing all the four tasks correctly, you should see data notification through the "nRF Connect for Mobile" app. The received data should be the same as the one printed in the terminal.

