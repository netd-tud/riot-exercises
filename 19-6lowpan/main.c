/*
 * Copyright (C) 2026 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       6LowPAN
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include <stdio.h>
#include <string.h>
#include "ztimer.h"
#include "random.h"
#include "net/sock/udp.h"

/* Define a common port number for all the nodes. */
#define PORT        (1234)
#define INTERVAL    (5000)   /* Data sending interval in milliseconds */

/* Multicast address ff02::1 for all the nodes */
ipv6_addr_t mcast_addr = IPV6_ADDR_ALL_NODES_LINK_LOCAL;

/* Define a variable to store the socket object. */
sock_udp_t sock;

/* Define two stacks. One to transmit data and the other one to receive data. */
char tx_stack[THREAD_STACKSIZE_DEFAULT];
char rx_stack[THREAD_STACKSIZE_DEFAULT];

/* Transmit stack: generate, send and print UDP data. */
void *tx_thread(void *arg)
{
    (void)arg;
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    remote.port = PORT;
    remote.netif = SOCK_ADDR_ANY_NETIF;
    memcpy(&remote.addr.ipv6, &mcast_addr, sizeof(mcast_addr));

    while (1) {



        /* [TASK 2: Generate, send and print UDP data.] */
        uint8_t payload[5];
        uint8_t random_data = random_uint32_range(0, 256);
        for (uint8_t i = 0; i < sizeof(payload); i++) {
            payload[i] = random_data;
        }
        sock_udp_send(&sock, payload, sizeof(payload), &remote);
        printf("TX: ");
        for (unsigned i = 0; i < sizeof(payload); i++) {
            printf("%02x ", payload[i]);
        }
        printf("\n");




        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return NULL;
}

/* Receive stack: receive and print UDP data. */
void *rx_thread(void *arg)
{
    (void)arg;
    uint8_t buf[16];

    while (1) {



        /* [TASK 3: Receive and print UDP data.] */
        int len = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, NULL);
        if (len > 0) {
            printf("RX: ");
            for (int i = 0; i < len; i++) {
                printf("%02x ", buf[i]);
            }
            printf("\n");
        }



    }

    return NULL;
}

int main(void)
{



    /* [TASK 1: Creat a socket object and bind to the PORT (1234).] */
    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    local.port = PORT;
    sock_udp_create(&sock, &local, NULL, 0);




    /* Create two threads to take care of transmit and receive, respectively. */
    thread_create(tx_stack, sizeof(tx_stack), THREAD_PRIORITY_MAIN - 1, 0, tx_thread, NULL, "tx");
    thread_create(rx_stack, sizeof(rx_stack), THREAD_PRIORITY_MAIN - 1, 0, rx_thread, NULL, "rx");

    return 0;
}
