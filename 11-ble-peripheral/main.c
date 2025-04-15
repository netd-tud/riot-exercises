/*
 * Copyright (C) 2025 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       BLE_peripheral
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include "random.h"
#include "ztimer.h"
#include "host/util/util.h"
#include "host/ble_gap.h"
#include "services/gap/ble_svc_gap.h"

/* Define UUIDs. */
#define BLE_SVC_CUSTOM_UUID                         0xff00
#define BLE_CHR_CUSTOM_NOTIFY_UUID                  0xee00
/* Define variables that will be used later. */
const char *device_name = "RIOT-BLE-peripheral";
uint8_t addr_type;
uint8_t conn_state;
uint16_t conn_handle;
uint8_t notify_state;
/* Define characteristic value handle. */
uint16_t custom_notify_data_val_handle;

void advertise(void);

/*
 * Define characteristic access callback handler.
 * This function is called when a client accesses the characteristic.
 * It is used to handle read/write requests from the client.
 * In this case, we are only interested in the notification characteristic.
 * But we still keep it in case we want to add more functionality in the future.
 */
int custom_notify_data_handler(uint16_t conn_handle, uint16_t attr_handle,
    struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle;
    (void)attr_handle;
    (void)arg;
    if (ble_uuid_u16(ctxt->chr->uuid) != BLE_CHR_CUSTOM_NOTIFY_UUID) {
        return BLE_ATT_ERR_UNLIKELY;
    }
    return 0;
}

/* Define our custom service and characteristic. */
const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        /* Custom Service */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(BLE_SVC_CUSTOM_UUID),
        .characteristics = (struct ble_gatt_chr_def[]) { {
            /* Custom Notify Characteristic */
            .uuid = BLE_UUID16_DECLARE(BLE_CHR_CUSTOM_NOTIFY_UUID),
            .access_cb = custom_notify_data_handler,
            .val_handle = &custom_notify_data_val_handle,
            .flags = BLE_GATT_CHR_F_NOTIFY,
        },  {
            0, /* No more characteristics in this service */
        }, }
    },
    {
        0, /* No more services */
    },
};

int peripheral_conn_event(struct ble_gap_event *event, void *arg)
{
    (void)arg;

    switch (event->type) {
    case BLE_GAP_EVENT_ADV_COMPLETE:
        advertise();
        return 0;
    case BLE_GAP_EVENT_CONNECT:
        conn_state = 1;
        conn_handle = event->connect.conn_handle;
        return 0;
    case BLE_GAP_EVENT_DISCONNECT:
        conn_state = 0;
        advertise();
        return 0;

    /* [TASK 3: Check if someone subscribed to our custom characteristic] */


    case BLE_GAP_EVENT_NOTIFY_TX:
        printf("Peripheral: Connected and sending notification\n");
        return 0;
    }
    return 0;
}

void advertise(void)
{
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;

    /* Fill all fields and parameters with zeros */
    memset(&adv_params, 0, sizeof(adv_params));
    memset(&fields, 0, sizeof(fields));

    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    fields.flags = BLE_HS_ADV_F_DISC_GEN;
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;

    /* Set UUID for the custom service */
    fields.uuids16 = (ble_uuid16_t[]) {
        BLE_UUID16_INIT(BLE_SVC_CUSTOM_UUID)
    };
    fields.num_uuids16 = 1;
    fields.uuids16_is_complete = 1;

    /* [TASK 1: Start advertising here] */

}

int main(void)
{
    /* Set device name */
    int rc;
    rc = ble_svc_gap_device_name_set(device_name);
    assert(rc == 0);

    /* [TASK 2: Add our custom service, and reload the GATT server] */


    /* addr_type will store type of address we use */
    rc = ble_hs_util_ensure_addr(0);
    assert(rc == 0);
    rc = ble_hs_id_infer_auto(0, &addr_type);
    assert(rc == 0);
    /* Begin advertising. */
    advertise();

    while (1) {
        if (conn_state == 0) {
            printf("Peripheral: Not connected\n");
        } else {
            if (notify_state == 1) {



                /* [TASK 4: Achieve the notification here] */




            } else {
                printf("Peripheral: Connected but not sending notification\n");
            }
        }
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}
