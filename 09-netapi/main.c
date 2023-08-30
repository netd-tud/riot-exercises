/*
 * Copyright (C) 2015 Inria
 * Copyright (C) 2023 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @{
 *
 * @file
 * @author  Oliver Hahm <oliver.hahm@inria.fr>
 * @author  Martine Lenders <martine.lenders@tu-dresden.de>
 */

#include <stdio.h>

#include "msg.h"
#include "net/ipv6/addr.h"
#include "net/gnrc.h"
#include "net/gnrc/ipv6/hdr.h"
#include "net/utils.h"
#include "net/netif.h"
#include "od.h"
#include "shell.h"
#include "thread.h"

#define MSG_QUEUE_SIZE  8

/* [TASK 2: Move reception code to thread handler here] */

/* [TASK 2: Add shell command handler for sending here] */

/* [TASK 2: register your shell command handler here] */

int main(void)
{
    puts("NETAPI example.\n");

    /* buffer to read commands */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    /* get interfaces and print their addresses */
    gnrc_netif_t *netif = NULL;
    while ((netif = gnrc_netif_iter(netif))) {
        printf("Iface %d:\n", (int)netif->pid);

        ipv6_addr_t ipv6_addrs[CONFIG_GNRC_NETIF_IPV6_ADDRS_NUMOF];
        int res = gnrc_netapi_get(netif->pid, NETOPT_IPV6_ADDR, 0, ipv6_addrs,
                                  sizeof(ipv6_addrs));

        if (res < 0) {
            puts(" {}");
            continue;
        }
        for (unsigned i = 0; i < (unsigned)(res / sizeof(ipv6_addr_t)); i++) {
            char ipv6_addr[IPV6_ADDR_MAX_STR_LEN];

            ipv6_addr_to_str(ipv6_addr, &ipv6_addrs[i], IPV6_ADDR_MAX_STR_LEN);
            printf("- %s\n", ipv6_addr);
        }
    }

    /* [TASK 1: include message queue initialization here] */

    /* [TASK 1: register for IPv6 protocol number 253 here] */

    /* [TASK 1: receive packet here] */

    /* [TASK 1: unregister from IPv6 protocol number 253 here] */

    /* run the shell, this will block the thread waiting for incoming commands */
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    /* main thread exits */
    return 0;
}

/** @} */
