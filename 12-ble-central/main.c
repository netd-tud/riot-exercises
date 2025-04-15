/*
 * Copyright (C) 2025 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       BLE_central
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include "ztimer.h"
#include "host/util/util.h"
#include "host/ble_gap.h"
#include "services/gap/ble_svc_gap.h"

/* Define UUIDs. */
#define BLE_SVC_CUSTOM_UUID                         0xff00
#define BLE_CHR_CUSTOM_NOTIFY_UUID                  0xee00
/* Convert UUIDs to ble_uuid16_t. */
ble_uuid16_t custom_svc_uuid = BLE_UUID16_INIT(BLE_SVC_CUSTOM_UUID);
ble_uuid16_t custom_chr_uuid = BLE_UUID16_INIT(BLE_CHR_CUSTOM_NOTIFY_UUID);
/* Define variables that will be used later. */
uint8_t addr_type;
uint8_t conn_state;
uint8_t notify_state;
/* Define a variable to store the characteristic value handle */
uint16_t custom_chr_val_handle;
/* Define a variable to manually set Client Characteristic Configuration (CCC) descriptor handle */
uint16_t custom_chr_ccc_handle;

void scan(void);

int discover_chr_cb(uint16_t conn_handle, const struct ble_gatt_error *error,
    const struct ble_gatt_chr *chr, void *arg)
{
    (void)arg;
    (void)error;

    if (chr != NULL) {
        /* Check if this is our desired characteristic */
        if (ble_uuid_cmp(&chr->uuid.u, &custom_chr_uuid.u) == 0) {
            custom_chr_val_handle = chr->val_handle;
            custom_chr_ccc_handle = chr->val_handle + 1;

            /* Enable notifications by writing 0x0001 to the CCC handle */
            uint16_t value[1] = {0x0001};
            ble_gattc_write_flat(conn_handle, custom_chr_ccc_handle,
                                value, sizeof(value), NULL, NULL);
        }
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
        /* [TASK 2: Achieve service discovery. If failed, terminate the connection and re-scan] */

        return 0;
    case BLE_GAP_EVENT_DISCONNECT:
        conn_state = 0;
        scan();
        return 0;

    /* [TASK 3: Print out the received data] */


    }
    return 0;
}

int central_scan_event(struct ble_gap_event *event, void *arg)
{
    (void)arg;

    const ble_uuid16_t predef_uuid = custom_svc_uuid;

    struct ble_hs_adv_fields parsed_fields;
    int uuid_cmp_result;

    memset(&parsed_fields, 0, sizeof(parsed_fields));

    switch (event->type) {
    case BLE_GAP_EVENT_DISC_COMPLETE:
        scan();
        return 0;
    case BLE_GAP_EVENT_DISC:
        ble_hs_adv_parse_fields(&parsed_fields, event->disc.data, event->disc.length_data);
        uuid_cmp_result = ble_uuid_cmp(&predef_uuid.u, &parsed_fields.uuids16->u);
        if (uuid_cmp_result == 0) {
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
    /* [TASK 1: Set scan parameters, and perform discovery procedure] */

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
        } else {
            if (notify_state == 0) {
                printf("Central: Connected but not receiving notification\n");
            } else {
                printf("Central: Connected and receiving notification\n");
            }
        }
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}
