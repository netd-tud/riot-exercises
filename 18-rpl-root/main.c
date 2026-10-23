/*
 * Copyright (C) 2026 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       RPL Root
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include <stdio.h>
#include <string.h>
#include "net/gnrc/netif.h"
#include "net/gnrc/rpl.h"
#include "net/gnrc/ipv6/nib/ft.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "ztimer.h"

/* Define root IP */
#define ROOT_IP "fd00::1"

void print_rpl_tree(void)
{
    void *state = NULL;
    gnrc_ipv6_nib_ft_t entry;

    printf("\n--- RPL Tree ---\n");
    while (gnrc_ipv6_nib_ft_iter(NULL, 0, &state, &entry)) {
        char dst_str[IPV6_ADDR_MAX_STR_LEN];
        char hop_str[IPV6_ADDR_MAX_STR_LEN];

        /* We are only interested in the nodes with the prefix fd00 */
        if (entry.dst.u8[0] == 0xfd && entry.dst.u8[1] == 0x00) {



            /* [TASK 1: Print the RPL tree.] */
            ipv6_addr_to_str(dst_str, &entry.dst, sizeof(dst_str));
            /* If the last 8 bytes of the target IP and the last 8 bytes of the next hop physical IP are the same, it means there is no intermediate node */
            if (memcmp(&entry.dst.u8[8], &entry.next_hop.u8[8], 8) == 0) {
                printf("Root <- %s\n", dst_str);
            } else {
                /* If they are different, it means there is an intermediate node */
                ipv6_addr_to_str(hop_str, &entry.next_hop, sizeof(hop_str));
                printf("%s <- %s\n", hop_str, dst_str);
            }



        }
    }
    printf("----------------\n\n");
}

int main(void)
{
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    ipv6_addr_t root_addr;
    ipv6_addr_from_str(&root_addr, ROOT_IP);
    gnrc_netif_ipv6_addr_add(netif, &root_addr, 64, GNRC_NETIF_IPV6_ADDRS_FLAGS_STATE_VALID);
    gnrc_rpl_root_init(0, &root_addr, true, true);
    sock_udp_ep_t local = { .family = AF_INET6, .port = 8080 };
    sock_udp_t sock;
    sock_udp_create(&sock, &local, NULL, 0);

    uint8_t buf[32];
    while (1) {
        /* Declare a remote endpoint */
        sock_udp_ep_t remote;

        /* Receive data */
        int len = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT, &remote);
        if (len > 0) {
            /* Define a buffer to store the IP address as a string */
            char remote_ip[IPV6_ADDR_MAX_STR_LEN];
            /* Convert the binary IPv6 address in remote.addr.ipv6 to a string */
            ipv6_addr_to_str(remote_ip, (ipv6_addr_t *)&remote.addr.ipv6, sizeof(remote_ip));
            /* Print the message, its source, and the RPL tree */
            printf("[Rx] from %s: %.*s\n", remote_ip, (int)len, (char *)buf);
            print_rpl_tree();
        }
    }

    return 0;
}
