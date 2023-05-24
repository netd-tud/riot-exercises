/*
 * Copyright (c) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "net/gcoap.h"
#include "net/utils.h"
#include "od.h"

#include "saul_reg.h"
#include "phydat.h"
#include "assert.h"

#include "gcoap_example.h"

static ssize_t _sensor_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, coap_request_ctx_t *ctx);

/* CoAP resources. Must be sorted by path (ASCII order). */
#if defined(TASK_4)
static saul_reg_t *temp_device = NULL;
static saul_reg_t *hum_device = NULL;

static coap_resource_t _resources[] = {
    { "/sense/hum", COAP_GET, _sensor_handler, &hum_device },
    { "/sense/temp", COAP_GET, _sensor_handler, &temp_device },
};
#elif defined(TASK_5)
static saul_reg_t *press_device = NULL;
static saul_reg_t *mag_device = NULL;

static coap_resource_t _resources[] = {
    { "/sense/mag", COAP_GET, _sensor_handler, &mag_device },
    { "/sense/press", COAP_GET, _sensor_handler, &press_device },
};
#else
#error "Set either TASK_4 or TASK_5 CFLAGS"
#endif

static gcoap_listener_t _listener = {
    _resources,
    ARRAY_SIZE(_resources),
    GCOAP_SOCKET_TYPE_UDP,
    NULL,
    NULL,
    NULL
};

void server_init(void)
{
    gcoap_register_listener(&_listener);

    /* find sensors */
#if defined(TASK_4)
    temp_device = saul_reg_find_type(SAUL_SENSE_TEMP);
    hum_device = saul_reg_find_type(SAUL_SENSE_HUM);
    if (!temp_device || !hum_device) {
        puts("Error, could not find the devices");
        assert(0);
    }
#elif defined(TASK_5)
    press_device = saul_reg_find_type(SAUL_SENSE_TEMP);
    mag_device = saul_reg_find_type(SAUL_SENSE_HUM);
    if (!press_device || !mag_device) {
        puts("Error, could not find the devices");
        assert(0);
    }
#endif
}

/*
 * GET: Returns the current sensor value as plain text
 */
static ssize_t _sensor_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, coap_request_ctx_t *ctx)
{
    const coap_resource_t *resource = ctx->resource;
    saul_reg_t *device = *(saul_reg_t**)resource->context;

    /* initialize a new coap response */
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);

    /* first we set all the options */
    /* set the format option to "plain text" */
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);

    /* finish the options sections */
    /* it is important to keep track of the amount of used bytes (resp_len) */
    size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

    phydat_t result;
    int dimensions = saul_reg_read(device, &result);
    if (dimensions < 0) {
        puts("Error reading sensor");
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }

    /* check that we have enough space */
    if (fmt_s16_dec(NULL, result.val[0]) > pdu->payload_len) {
        puts("Error buffer too small");
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }

    resp_len += fmt_s16_dec((char *)pdu->payload, result.val[0]);
    return resp_len;
}
