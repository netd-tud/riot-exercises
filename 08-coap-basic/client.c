/*
 * Copyright (c) 2015-2017 Ken Bannister. All rights reserved.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       gcoap CLI support
 *
 * @author      Ken Bannister <kb2ma@runbox.com>
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "fmt.h"
#include "net/gcoap.h"
#include "net/utils.h"
#include "od.h"

#include "gcoap_example.h"

uint16_t req_count = 0;

/*
 * Response callback.
 */
static void _resp_handler(const gcoap_request_memo_t *memo, coap_pkt_t* pdu,
                          const sock_udp_ep_t *remote)
{
    (void)remote;       /* not interested in the source currently */

    if (memo->state == GCOAP_MEMO_TIMEOUT) {
        printf("gcoap: timeout for msg ID %02u\n", coap_get_id(pdu));
        return;
    }
    else if (memo->state != GCOAP_MEMO_RESP) {
        printf("gcoap: error in response\n");
        printf("state: %d\n", memo->state);
        return;
    }

    char *class_str = (coap_get_code_class(pdu) == COAP_CLASS_SUCCESS) ? "Success" : "Error";

    printf("gcoap: response %s, code %1u.%02u", class_str,
                                                coap_get_code_class(pdu),
                                                coap_get_code_detail(pdu));
    if (pdu->payload_len) {
        unsigned content_type = coap_get_content_type(pdu);
        if (content_type == COAP_FORMAT_TEXT
                || content_type == COAP_FORMAT_LINK
                || coap_get_code_class(pdu) == COAP_CLASS_CLIENT_FAILURE
                || coap_get_code_class(pdu) == COAP_CLASS_SERVER_FAILURE) {
            /* Expecting diagnostic payload in failure cases */
            printf(", %u bytes\n%.*s\n", pdu->payload_len, pdu->payload_len,
                                                          (char *)pdu->payload);
        }
        else {
            printf(", %u bytes\n", pdu->payload_len);
            od_hex_dump(pdu->payload, pdu->payload_len, OD_WIDTH_DEFAULT);
        }
    }
    else {
        printf(", empty payload\n");
    }
}

static bool _parse_endpoint(sock_udp_ep_t *remote,
                            const char *addr_str, const char *port_str)
{
    netif_t *netif;

    /* parse hostname */
    if (netutils_get_ipv6((ipv6_addr_t *)&remote->addr, &netif, addr_str) < 0) {
        puts("gcoap_cli: unable to parse destination address");
        return false;
    }
    remote->netif = netif ? netif_get_id(netif) : SOCK_ADDR_ANY_NETIF;
    remote->family = AF_INET6;

    /* parse port */
    remote->port = atoi(port_str);
    if (remote->port == 0) {
        puts("gcoap_cli: unable to parse destination port");
        return false;
    }

    return true;
}

static size_t _send(uint8_t *buf, size_t len, char *addr_str, char *port_str)
{
    size_t bytes_sent;
    sock_udp_ep_t remote;

    if (!_parse_endpoint(&remote, addr_str, port_str)) {
        return 0;
    }

    bytes_sent = gcoap_req_send(buf, len, &remote, NULL, _resp_handler, NULL, GCOAP_SOCKET_TYPE_UDP);
    if (bytes_sent > 0) {
        req_count++;
    }
    return bytes_sent;
}

static int _print_usage(char **argv)
{
    printf("usage: %s info\n", argv[0]);
    printf("       %s <get|post|put|delete> <addr>[%%iface] <port> <path> [data]\n",argv[0]);
    return 1;
}

static int _coap_info_cmd(void)
{
    uint8_t open_reqs = gcoap_op_state();

    printf("CoAP server is listening on port %u\n", CONFIG_GCOAP_PORT);
    printf(" CLI requests sent: %u\n", req_count);
    printf("CoAP open requests: %u\n", open_reqs);
    return 0;
}

/* map a string to a coap method code */
int _method_str_to_code(const char *method)
{
    if (!strcmp(method, "get")) {
        return COAP_METHOD_GET;
    }
    else if (!strcmp(method, "post")) {
        return COAP_METHOD_POST;
    }
    else if (!strcmp(method, "put")) {
        return COAP_METHOD_PUT;
    }
    else if (!strcmp(method, "delete")) {
        return COAP_METHOD_DELETE;
    }

    printf("Unknown method: %s\n", method);
    return -1;
}

int gcoap_cli_cmd(int argc, char **argv)
{

    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    coap_pkt_t pdu;
    size_t len;
    int position = 1;

    /* parse user input */
    if (argc == 1) {
        /* invalid number of arguments, show help for the command */
        return _print_usage(argv);
    }

    if (strcmp(argv[position], "info") == 0) {
        return _coap_info_cmd();
    }

    if ((argc != 5) && (argc != 6)) {
        /* invalid number of arguments, show help for the command */
        return _print_usage(argv);
    }

    /* determine which method to use */
    int code = _method_str_to_code(argv[position]);
    if (code < 0) {
        /* unknown method, show help */
        return _print_usage(argv);
    }

    position ++;

    char *addr = argv[position++];
    char *port = argv[position++];
    char *path = argv[position++];

    /* simple check for the path */
    if (!path || path[0] != '/') {
        return _print_usage(argv);
    }

    char *data = NULL;
    size_t data_len = 0;
    if (argc == 6) {
        data = argv[position++];
        data_len = strlen(data);
    }
    printf("%s\n", path);
    /* initialize the CoAP request */
    gcoap_req_init(&pdu, buf, CONFIG_GCOAP_PDU_BUF_SIZE, code, path);

    /* send a confirmable message */
    coap_hdr_set_type(pdu.hdr, COAP_TYPE_CON);

    /* if there is data, we specify the plain text format and copy the payload */
    if (data_len) {
        coap_opt_add_format(&pdu, COAP_FORMAT_TEXT);

        len = coap_opt_finish(&pdu, COAP_OPT_FINISH_PAYLOAD);
        if (pdu.payload_len >= data_len) {
            memcpy(pdu.payload, data, data_len);
            len += data_len;
        }
        else {
            puts("The buffer is too small, reduce the message length");
            return 1;
        }
    } else {
        len = coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);
    }

    printf("gcoap_cli: sending msg ID %u, %u bytes\n", coap_get_id(&pdu), (unsigned) len);
    if (!_send(buf, len, addr, port)) {
        puts("gcoap_cli: msg send failed");
        return -1;
    }
    return 0;
}

/* define CoAP shell command */
SHELL_COMMAND(coap, "Perform CoAP operations", gcoap_cli_cmd);
