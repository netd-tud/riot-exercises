/*
 * Copyright (C) 2026 TU Dresden
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 * @brief       IEEE802154_receive
 * @author      Bozheng Pang <bozheng.pang@tu-dresden.de>
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "thread.h"
#include "msg.h"
#include "net/gnrc.h"
#include "net/gnrc/netif.h"
#include "net/gnrc/netif/hdr.h"
#include "net/gnrc/netapi.h"
#include "net/gnrc/netreg.h"
#include "net/netopt.h"
#include "net/ieee802154.h"



/* [TASK 1: Necessary configurations. They should match with the transmitter.] */




msg_t main_msg_queue[APP_MSG_QUEUE_SIZE];
gnrc_netif_t *iface = NULL;
uint16_t rx_count = 0;

void receive(gnrc_pktsnip_t *pkt)
{



    /* [TASK 2: Print the received data.] */



    
    /* release the packet from the buffer */
    gnrc_pktbuf_release(pkt);
}

int main(void)
{
    /* Open a message queue for receiving packets */
    msg_init_queue(main_msg_queue, APP_MSG_QUEUE_SIZE);
    /* Find an interface */
    iface = gnrc_netif_iter(NULL);
    /* Set up PAN ID and channel for communication */
    uint16_t pan  = APP_PAN_ID;
    uint16_t chan = APP_CHANNEL;
    gnrc_netapi_set(iface->pid, NETOPT_NID,     0, &pan,  sizeof(pan));
    gnrc_netapi_set(iface->pid, NETOPT_CHANNEL, 0, &chan, sizeof(chan));
    /* Register a thread for receiving packets */
    gnrc_netreg_entry_t entry = 
        GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL, thread_getpid());
    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &entry);
    /* Handle packets in a while loop */
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        if (msg.type == GNRC_NETAPI_MSG_TYPE_RCV) {
            receive(msg.content.ptr);
        }
    }

    return 0;
}
