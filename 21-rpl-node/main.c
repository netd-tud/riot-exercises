/*
 * Copyright (C) 2026 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       RPL Node
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include <stdio.h>
#include <string.h>
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "ztimer.h"

/* Define root IP, port number, and sending interval */
#define ROOT_IP "fd00::1"
#define PORT 8080
#define SEND_INTERVAL_MS (5000)

int main(void)
{
    /* Set up target node (root) for sending, and prepare a message to send */
    sock_udp_ep_t remote = { .family = AF_INET6, .port = PORT };
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr.ipv6, ROOT_IP);
    const char *msg = "Hello Root!";

    while (1) {
        /* Send and print the message */
        int len = sock_udp_send(NULL, msg, strlen(msg), &remote);
        if (len > 0) {
            printf("[TX] %s\n", msg);
        }

        ztimer_sleep(ZTIMER_MSEC, SEND_INTERVAL_MS);
    }

    return 0;
}
