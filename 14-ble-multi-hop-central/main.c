/*
 * Copyright (C) 2026 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       BLE_multihop_central
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include "ztimer.h"
#include "host/util/util.h"
#include "host/ble_gap.h"
#include "services/gap/ble_svc_gap.h"
#include "ble_hs_conn_priv.h"

/* Define UUIDs. */
#define CUSTOM_SVC_UUID                         0xff00
#define CUSTOM_CHR_NOTIFY_UUID                  0xee00
#define CUSTOM_CHR_WRITE_UUID                   0xee01
/* Convert UUIDs to ble_uuid16_t. */
ble_uuid16_t custom_svc_uuid = BLE_UUID16_INIT(CUSTOM_SVC_UUID);
ble_uuid16_t custom_chr_notify_uuid = BLE_UUID16_INIT(CUSTOM_CHR_NOTIFY_UUID);
ble_uuid16_t custom_chr_write_uuid = BLE_UUID16_INIT(CUSTOM_CHR_WRITE_UUID);
/* Define variables that will be used later. */
uint8_t addr_type;
uint8_t conn_state;
/* Define a variable to store the characteristic value handle */
uint16_t custom_chr_val_handle;
/* Define a variable to manually set Client Characteristic Configuration (CCC) descriptor handle */
uint16_t custom_chr_ccc_handle;
/* Define a variable to store the write characteristic value handle */
uint16_t custom_write_data_val_handle;

void scan(void);

int discover_chr_cb(uint16_t conn_handle, const struct ble_gatt_error *error,
    const struct ble_gatt_chr *chr, void *arg)
{
    (void)arg;
    (void)error;

    if (chr != NULL) {
        /* Check if this is the notify characteristic */
        if (ble_uuid_cmp(&chr->uuid.u, &custom_chr_notify_uuid.u) == 0) {
            custom_chr_val_handle = chr->val_handle;
            custom_chr_ccc_handle = chr->val_handle + 1;
            /* Enable notifications by writing 0x0001 to the CCC handle */
            uint16_t value[1] = {0x0001};
            ble_gattc_write_flat(conn_handle, custom_chr_ccc_handle,
                                value, sizeof(value), NULL, NULL);
        }



        /* [TASK 2: Discover the write characteristic from the peripheral device.] */




    }

    return 0;
}

int discover_svc_cb(uint16_t conn_handle, const struct ble_gatt_error *error,
    const struct ble_gatt_svc *service, void *arg)
{
    (void)arg;
    (void)error;

    if (service != NULL) {
        /* Discover characteristics */
        ble_gattc_disc_all_chrs(conn_handle,
                                service->start_handle,
                                service->end_handle,
                                discover_chr_cb, NULL);
    }

    return 0;
}

int central_conn_event(struct ble_gap_event *event, void *arg)
{
    (void)arg;

    switch (event->type) {
    case BLE_GAP_EVENT_CONNECT:
        conn_state = 1;
        /* Achieve service discovery. If failed, terminate the connection and re-scan */
        uint16_t conn_handle = event->connect.conn_handle;
        int rc = ble_gattc_disc_svc_by_uuid(conn_handle, &custom_svc_uuid.u, 
                                        discover_svc_cb, NULL);
        if (rc != 0) {
            ble_gap_terminate(event->connect.conn_handle, BLE_ERR_REM_USER_CONN_TERM);
            conn_state = 0;
            scan();
        } else {
            /* If this device is not connected to two other devices, keep scanning */
            int idx = 0;
            struct ble_hs_conn *c;
            while ((c = ble_hs_conn_find_by_idx(idx)) != NULL) {
                idx++;
            }
            if (idx < 2) {
                scan();
            }
        }
        return 0;
    case BLE_GAP_EVENT_DISCONNECT:
        conn_state = 0;
        scan();
        return 0;

    /* Print and forward the received data */
    case BLE_GAP_EVENT_NOTIFY_RX:
        printf("Central: received data --> ");
        for (uint8_t i = 0; i < event->notify_rx.om->om_len; i++) {
            printf("%02x ", event->notify_rx.om->om_data[i]);
        }
        printf("\n");

        int idx = 0;
        struct ble_hs_conn *c;
        while ((c = ble_hs_conn_find_by_idx(idx)) != NULL) {
            if (c->bhc_handle != event->notify_rx.conn_handle) {



                /* [TASK 3: Forward the received data to the other connected peripheral device using the write characteristic.] */




            }
            idx++;
        }

        return 0;

    }
    return 0;
}

int central_scan_event(struct ble_gap_event *event, void *arg)
{
    (void)arg;

    struct ble_hs_adv_fields parsed_fields;
    memset(&parsed_fields, 0, sizeof(parsed_fields));

    switch (event->type) {
    case BLE_GAP_EVENT_DISC_COMPLETE:
        scan();
        return 0;
    case BLE_GAP_EVENT_DISC:
        ble_hs_adv_parse_fields(&parsed_fields, event->disc.data, event->disc.length_data);
        if (ble_uuid_cmp(&custom_svc_uuid.u, &parsed_fields.uuids16->u) == 0) {
            ble_gap_disc_cancel();
            ble_gap_connect(addr_type, &(event->disc.addr), 100,
                            NULL, central_conn_event, NULL);
        }
        return 0;
    }
    return 0;
}

void scan(void)
{
    /* Set scan parameters, and perform discovery procedure */
    const struct ble_gap_disc_params scan_params = {10000, 200, 0, 0, 0, 1};
    int rc = ble_gap_disc(addr_type, 100, &scan_params, central_scan_event, NULL);
    assert(rc == 0);
}

int main(void)
{
    int rc;

    /* addr_type will store type of address we use */
    rc = ble_hs_util_ensure_addr(0);
    assert(rc == 0);
    rc = ble_hs_id_infer_auto(0, &addr_type);
    assert(rc == 0);
    /* Begin scanning. */
    scan();

    while (1) {
        if (conn_state == 0) {
            printf("Central: Not connected\n");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
