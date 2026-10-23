# BLE central

To achieve Bluetooth Low Energy (BLE) communication, RIOT uses NimBLE (an open-source BLE stack). This is a simple example to achieve the central role on a BLE device.

**Note that you should only test this example when everything in the [BLE peripheral example](../11-ble-peripheral/README.md) example has been successfully implemented.**

There are many functions and operations used in the example to achieve the implementation. However, we do not introduce everything to avoid too much workload or overhead. The goal is to help understand fast how to implement a BLE central and how BLE communication achieves certain behaviors, such as scanning, connecting, and receiving data.

We provide a very simple skeleton `main.c` and `Makefile`. The Makefile is quite similar to the [Makefile](../11-ble-peripheral/Makefile) of [BLE peripheral example](../11-ble-peripheral/README.md). Thus, the explanation is not repeated.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../12-ble-central
```

## Preparation
Two boards are needed, for this example to interact with the [BLE peripheral example](../11-ble-peripheral/README.md).

**1. Keep the completed version of [BLE peripheral example](../11-ble-peripheral/README.md) running on a board (peripheral board).**

**2. Start a terminal to see the output information from the peripheral board. Flexibly use the terminal and `printf` to see what the board is doing with BLE.**

**3. Flash this example onto the other board (central board).**

**4. Start another terminal to see the output information from the central board. Flexibly use the terminal and `printf` to see what the board is doing with BLE.**

Keep an eye on the two terminals when implementing the steps and tasks of this example.

## Task 1
**Set scan parameters and perform scanning.**

Some basic concepts in BLE communication are as follows.

**BLE Central (Client):** The central (client) is a device that initiates a connection to the peripheral (server). It requests data or services from the peripheral (server) and then can use the data it receives. To find the peripheral (server), the central (client) must scan for advertising packets in the air.

The code below completes the `scan()` function in the example. The implementation of it should also allow the central device to connect to the peripheral device. This is because the connection command `ble_gap_connect()` has been implemented in `central_scan_event()`. You can find these two functions in the example code already.

```C
const struct ble_gap_disc_params scan_params = {10000, 200, 0, 0, 0, 1};
int rc = ble_gap_disc(addr_type, 100, &scan_params, central_scan_event, NULL);
assert(rc == 0);
```

- `ble_gap_disc()` performs the scan procedures based on the defined `scan_params`. `scan_params` includes parameters for scanning, such as scan interval and scan window.

After implementing the above code, the effect can be seen through the output from the two terminals.

## Task 2
**Achieve service discovery (If failed, terminate the connection and re-scan).**

Since we defined our own custom service and characteristic, we know the UUIDs of them (They can be found in the example code.). As a result, we can ask the central to discover the peripheral service directly through the UUID.

It is possible that the service discovery fails. Once a failure occurs, we can simply disconnect from the current connection, and try to reconnect with the peripheral by scanning for the advertising packets in the air again.

The following code realizes the above procedures.

```C
uint16_t conn_handle = event->connect.conn_handle;
int rc = ble_gattc_disc_svc_by_uuid(conn_handle, &custom_svc_uuid.u, 
                                discover_svc_cb, NULL);
if (rc != 0) {
    notify_state = 0;
    ble_gap_terminate(event->connect.conn_handle, BLE_ERR_REM_USER_CONN_TERM);
    conn_state = 0;
    scan();
} else {
    notify_state = 1;
}
```

- `ble_gattc_disc_svc_by_uuid()` discovers a service by its service UUID.
- `ble_gap_terminate()` terminates an established connection.
- `scan()` starts a scan event.

Note that the central also subscribes to the peripheral notification, if the service discovery is successful. The realization of subscription is in the example code.

After implementing the above code, the effect can be seen through the output from the two terminals.

## Task 3
**Print out the received data (i.e., the notification data sent from the peripheral device).**

The BLE stack uses `BLE_GAP_EVENT_NOTIFY_RX` to link and show received notification events happened in a BLE connection. Hence, by checking it within `central_conn_event()` function, any received notification events and their related information (such as the data inside) can be found.

The code below helps show the received notification events, and related information can be obtained if necessary. For instance, `om` stores the notification data.

```C
case BLE_GAP_EVENT_NOTIFY_RX:
    printf("Central: Received data --> ");
    for (uint8_t i = 0; i < event->notify_rx.om->om_len; i++) {
        printf("%02x ", event->notify_rx.om->om_data[i]);
    }
    printf("\n");
    return 0;
```

After implementing the above code, the effect can be seen through the output from the two terminals.

After implementing all the three tasks correctly, the connection between the peripheral and the central should be correctly established. Besides, the data transmission from the peripheral to the central should be achieved through the notification. The received data on the central side should be the same to the sent data on the peripheral side (compare the output from the two terminals).

