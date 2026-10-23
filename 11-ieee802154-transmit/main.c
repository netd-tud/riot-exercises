/*
 * Copyright (C) 2026 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       IEEE802154_transmit
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "ztimer.h"
#include "net/gnrc.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netif/hdr.h"
#include "net/gnrc/netapi.h"
#include "net/netopt.h"
#include "net/ieee802154.h"



/* [TASK 1: Necessary configurations. They should match with the receiver.] */




gnrc_netif_t *iface = NULL;

int transmit(const char *data, size_t len)
{
    gnrc_pktsnip_t *header, *payload, *packet;



    /* [TASK 2: Form an IEEE 802.15.4 packet (header + payload = packet).] */



    
    /* Send the packet */
    gnrc_netapi_send(iface->pid, packet);

    return 0;
}

int main(void)
{
    /* Find an interface */
    iface = gnrc_netif_iter(NULL);
    /* Set up PAN ID and channel for communication */
    uint16_t pan  = APP_PAN_ID;
    uint16_t chan = APP_CHANNEL;
    gnrc_netapi_set(iface->pid, NETOPT_NID,     0, &pan,  sizeof(pan));
    gnrc_netapi_set(iface->pid, NETOPT_CHANNEL, 0, &chan, sizeof(chan));

    /* Send a message periodically */
    char buf[32];
    uint16_t seq = 0;
    while (1) {
        int len = snprintf(buf, sizeof(buf), "HELLO %d", seq);
        transmit(buf, len);



        /* [TASK 3: Print the transmitted data.] */




        seq++;

        ztimer_sleep(ZTIMER_MSEC, APP_SEND_INTERVAL);
    }

    return 0;
}
