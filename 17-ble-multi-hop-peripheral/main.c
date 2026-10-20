/*
 * Copyright (C) 2026 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       BLE_multihop_peripheral
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include "random.h"
#include "ztimer.h"
#include "host/util/util.h"
#include "host/ble_gap.h"
#include "services/gap/ble_svc_gap.h"

/* Define UUIDs. */
#define CUSTOM_SVC_UUID                         0xff00
#define CUSTOM_CHR_NOTIFY_UUID                  0xee00



/* [TASK 1: Define a UUID for a write characteristic] */




/* Define variables that will be used later. */
const char *device_name = "multi-hop-p";
uint8_t addr_type;
uint8_t conn_state;
uint16_t conn_handle;
uint8_t notify_state;
/* Define characteristic value handle. */
uint16_t custom_notify_data_val_handle;
uint16_t custom_write_data_val_handle;

void advertise(void);

/*
 * Define characteristic access callback.
 * These functions are called when a the characteristics are accessed.
 */

/*
 * The notify access callback is not doing anything, 
 * but is necessary to be defined for the service and characteristic definitions later.
 */
int custom_notify_access_cb(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle;
    (void)attr_handle;
    (void)arg;
    if (ble_uuid_u16(ctxt->chr->uuid) != CUSTOM_CHR_NOTIFY_UUID) {
        return BLE_ATT_ERR_UNLIKELY;
    }
    return 0;
}

/* The write access callback, */
int custom_write_access_cb(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    (void)conn_handle;
    (void)attr_handle;
    (void)arg;

    if (ble_uuid_u16(ctxt->chr->uuid) != CUSTOM_CHR_WRITE_UUID) {
        return BLE_ATT_ERR_UNLIKELY;
    }



    /* [TASK 3: Print the received data from central if any] */




    return 0;
}

/* Define our custom service and characteristic. */
const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        /* Custom Service */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(CUSTOM_SVC_UUID),
        .characteristics = (struct ble_gatt_chr_def[]) { {
            /* Custom Notify Characteristic */
            .uuid = BLE_UUID16_DECLARE(CUSTOM_CHR_NOTIFY_UUID),
            .access_cb = custom_notify_access_cb,
            .val_handle = &custom_notify_data_val_handle,
            .flags = BLE_GATT_CHR_F_NOTIFY,
        },



        /* [TASK 2: Define the write characteristic] */




        {
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
    case BLE_GAP_EVENT_SUBSCRIBE:
        notify_state = event->subscribe.cur_notify;
        return 0;
    case BLE_GAP_EVENT_NOTIFY_TX:
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
        BLE_UUID16_INIT(CUSTOM_SVC_UUID)
    };
    fields.num_uuids16 = 1;
    fields.uuids16_is_complete = 1;

    /* Start advertising */
    int rc;
    rc = ble_gap_adv_set_fields(&fields);
    assert(rc == 0);
    rc = ble_gap_adv_start(addr_type, NULL, 100, &adv_params, peripheral_conn_event, NULL);
    assert(rc == 0);
}

int main(void)
{
    /* Set device name */
    int rc;
    rc = ble_svc_gap_device_name_set(device_name);
    assert(rc == 0);

    /* Add our custom service, and reload the GATT server */
    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    assert(rc == 0);
    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    assert(rc == 0);

    /* Reload the GATT server to link our added services */
    rc = ble_gatts_start();
    assert(rc == 0);

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
                /* Achieve the notification here */
                uint8_t custom_notify_data[5];
                uint8_t random_data = random_uint32_range(0, 256);
                struct os_mbuf *om = ble_hs_mbuf_from_flat(custom_notify_data, sizeof(custom_notify_data));
                ble_gatts_notify_custom(conn_handle, custom_notify_data_val_handle, om);
                printf("Peripheral: sent data --> ");
                for (uint8_t i = 0; i < sizeof(custom_notify_data); i++) {
                    custom_notify_data[i] = random_data;
                    printf("%02x ", custom_notify_data[i]);
                }
                printf("\n");
            }
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}
