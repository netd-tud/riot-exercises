#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

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

// make all flash term PORT=/dev/ttyACM1 SENDER=1
//SENDER == 0 -> ROLE: RECEIVER
//SENDER == 1 -> ROLE: SENDER


#define CONFIG_NETDEV_DEFAULT_TX_POWER 0

/* Message queue for the main thread */
static msg_t msg_queue[MSG_QUEUE_SIZE];


int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    puts("NETAPI example.\n");
    xtimer_sleep(2);
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);

    printf("Iface %d:\n", (int)netif->pid);

    uint8_t hwaddr[8];
    int res = gnrc_netapi_get(netif->pid, NETOPT_ADDRESS_LONG, 0, hwaddr, sizeof(hwaddr));

    if (res > 0) {
        printf("HW Address: ");
        for (int i = 0; i < res; i++) {
            printf("%02x", hwaddr[i]);
            if (i < res - 1) printf(":");
        }
        printf("\n");
    }
    
	
	//Task 1 Set Pan ID and Channel
    int16_t panID = 0x1234;
	int16_t channel = 26;
    
    int CUSTOM_PROTO_TYPE = 253;
    

    gnrc_netreg_entry_t server = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL, thread_getpid());
	
	//Task 1 Register Nettype with UNDEF (so all is passed to layer 2)
    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &server);

    printf("Registered for protocol type 0x%04X\n", GNRC_NETREG_DEMUX_CTX_ALL);

    
    if(SENDER){
        
        char *pld = "Payload";
        
        puts("Sender mode - sending packets every 5 seconds...");
        
        
        //uint8_t dst_addr[8] = {0x12,0xcf,0xcf,0xcf,0xcf,0xcf,0xcf};
        //size_t dst_len = sizeof(dst_addr);
        while (1) {
            gnrc_pktsnip_t *payload = gnrc_pktbuf_add(NULL, pld, strlen(pld), CUSTOM_PROTO_TYPE);
            if (!payload) {
                puts("Failed to allocate payload");
                return 1;
            }
            gnrc_pktsnip_t *netif_hdr = gnrc_netif_hdr_build(NULL, 0, NULL, 0);
            if (!netif_hdr) {
                puts("Failed to allocate link-layer header");
                gnrc_pktbuf_release(payload);
                return 1;
            }
			
			//Task 2 Send packet
			
			puts("Packet sent");
            
            xtimer_sleep(5);
        }
        
        
    }else{
        puts("Receiver mode - waiting for packets...");
		while (1) {
			msg_t msg;
            msg_receive(&msg);

            if (msg.type == GNRC_NETAPI_MSG_TYPE_RCV) {
				//Task 3 Calculate Distance (assume tx=-60dBm)
				
			}
    
			gnrc_pktbuf_release(pkt);
		}
        
    }
    
    /* Unregister when done (never reached in this example) */
    gnrc_netreg_unregister(GNRC_NETTYPE_NETIF, &server);

    return 0;
}