#include <stdio.h>
#include <string.h>
#include "xtimer.h"
#include "thread.h"

#include "net/gnrc/netif.h"
#include "net/gnrc/ipv6.h"
#include "net/gnrc/rpl.h"
#include "net/gnrc/rpl/dodag.h"
#include "net/gnrc/udp.h"

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define MAX_NODES 16

typedef struct {
    ipv6_addr_t key;
    ipv6_addr_t value;
    int used;
} ip_map_entry_t;

ip_map_entry_t map[MAX_NODES];

// Add to ip_map_entry_t map
int map_add(const ipv6_addr_t *key, const ipv6_addr_t *value)
{
    int free_index = -1;

    for (int i = 0; i < MAX_NODES; i++) {
        if (map[i].used) {
            /* Update existing entry */
            if (memcmp(&map[i].key, key, sizeof(ipv6_addr_t)) == 0) {
                map[i].value = *value;
                return 0;   // updated
            }
        }
        else if (free_index < 0) {
            /* remember first free slot */
            free_index = i;
        }
    }

    /* Insert new entry if space available */
    if (free_index >= 0) {
        map[free_index].key = *key;
        map[free_index].value = *value;
        map[free_index].used = 1;
        return 0;   // inserted
    }

    /* Map full */
    return -1;
}


int parse_ip_pair(const char *input,
                  ipv6_addr_t *key,
                  ipv6_addr_t *value)
{
    char ip1[IPV6_ADDR_MAX_STR_LEN];
    char ip2[IPV6_ADDR_MAX_STR_LEN];

    /* Find space separator */
    const char *space = strchr(input, ' ');
    if (!space) {
        return -1;
    }

    size_t len1 = space - input;
    size_t len2 = strlen(space + 1);

    if (len1 == 0 || len2 == 0 ||
        len1 >= sizeof(ip1) ||
        len2 >= sizeof(ip2)) {
        return -1;
    }

    /* Copy substrings */
    memcpy(ip1, input, len1);
    ip1[len1] = '\0';
    strcpy(ip2, space + 1);

    /* Convert strings → ipv6_addr_t */
    if (!ipv6_addr_from_str(key, ip1)) {
        return -1;
    }
    if (!ipv6_addr_from_str(value, ip2)) {
        return -1;
    }

    return 0;
}

/* ================= IPv6 sending ================= */
int send_ipv6(ipv6_addr_t *dst, const char *msg1, const char *msg2)
{
    puts("SEND");
    netif_t *netif = &gnrc_netif_iter(NULL)->netif;
    
    size_t total_len = strlen(msg1) + 1 + strlen(msg2);

    char msg[total_len + 1]; // +1 for null terminator
    snprintf(msg, sizeof(msg), "%s %s", msg1, msg2); // concatenate with space
    
    /* Construct payload */
    size_t payload_size = strlen(msg);
    gnrc_pktsnip_t *payload = gnrc_pktbuf_add(NULL, &msg, payload_size, GNRC_NETTYPE_UNDEF);
    if (!payload) {
        puts("Failed to allocate payload");
        return 1;
    }

    /* Build IPv6 header */
    gnrc_pktsnip_t *ip = gnrc_ipv6_hdr_build(payload, NULL, dst);
    if (!ip) {
        puts("Failed to build IPv6 header");
        gnrc_pktbuf_release(payload);
        return 1;
    }

    ipv6_hdr_t *ip_hdr = ip->data;
    ip_hdr->nh = 253; /* Experimental protocol */

    if (netif != NULL) {
        gnrc_pktsnip_t *link_hdr = gnrc_netif_hdr_build(NULL, 0, NULL, 0);
        if (!link_hdr) {
            puts("Failed to allocate link-layer header");
            gnrc_pktbuf_release(ip);
            return 1;
        }
        gnrc_netif_hdr_set_netif(link_hdr->data,
                                 container_of(netif, gnrc_netif_t, netif));
        ip = gnrc_pkt_prepend(ip, link_hdr);
    }

    if (!gnrc_netapi_dispatch_send(GNRC_NETTYPE_IPV6, GNRC_NETREG_DEMUX_CTX_ALL, ip)) {
        puts("Failed to send packet");
        gnrc_pktbuf_release(ip);
        return 1;
    }

    return 0;
}

//Visualize the Map
void process_incoming_data(char* data){
    ipv6_addr_t key;
    ipv6_addr_t value;
    
    //TASK 3 Parse Paket Payload data
    parse_ip_pair(data, &key, &value);
    map_add(&key, &value);
    
    //TASK 3 Visualize the Network
    puts("Network Topology");
    
	// YOUR CODE
    
}

/* ================= Main ================= */
int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    printf("\n=== RPL Node Starting ===\n");
    xtimer_sleep(2);

    /* Get first network interface */
    gnrc_netif_t *netif = gnrc_netif_iter(NULL);
    if (!netif) {
        puts("No network interface found");
        return 1;
    }

    printf("Network interface PID: %d\n", netif->pid);

    // TASK 1 initialize RPL (auto init is included in Makefile)
    
    xtimer_sleep(2);
    
    /* Register for IPv6 experimental protocol 253 */
    // TASK 1 register IPV6 with Custom Protocol
    gnrc_netreg_entry_t server;
    
    
    int reg_result = -1;
    if (reg_result < 0) {
        printf("ERROR: IPv6 registration failed: %d\n", reg_result);
        return 1;
    }
    
    printf("Root thread registered for IPv6 packets\n");

#if ROOT
    printf("=== Initializing as DODAG Root ===\n");

  
    ipv6_addr_t dodag_id;
	// TASK 2 Create DODAG Root with ip and initialize Root Node

    gnrc_rpl_instance_t *inst;
    if (!inst) {
        puts("RPL Root init failed");
        return 1;
    }
    
    while (1) {
        msg_t msg;
        puts("Waiting for packet...");
        msg_receive(&msg);
        
        gnrc_pktsnip_t *pkt = msg.content.ptr;
        if (msg.type == GNRC_NETAPI_MSG_TYPE_RCV) {
            //Extract ipv6 and custom protocol header seperately
            gnrc_pktsnip_t *payload =
                gnrc_pktsnip_search_type(pkt, GNRC_NETTYPE_UNDEF);
            gnrc_pktsnip_t *ipv6 =
                gnrc_pktsnip_search_type(pkt, GNRC_NETTYPE_IPV6);

            if (payload && ipv6) {
                // Get payload data
                char buf[payload->size + 1];
                memcpy(buf, payload->data, payload->size);
                buf[payload->size] = 0;
                
                printf("Received Message: %s\n", buf);
                
                // TASK 3 Visualize the network
                process_incoming_data(buf);

            }

        }
        gnrc_pktbuf_release(pkt);
    }

#else
    /* Non-root node: send IPv6 packets to root */
    ipv6_addr_t root_addr;
    ipv6_addr_from_str(&root_addr, "2001:db8::1");

    while (1) {
        gnrc_rpl_instance_t *inst = gnrc_rpl_instance_get(1);
        

        if (!inst || !inst->dodag.parents) {
            xtimer_sleep(1);
            continue;
        }
        
        char src2[IPV6_ADDR_MAX_STR_LEN];
        ipv6_addr_to_str(src2, &netif->ipv6.addrs[0], sizeof(src2));
                
        char src[IPV6_ADDR_MAX_STR_LEN];
        ipv6_addr_to_str(src, &inst->dodag.parents->addr, sizeof(src));

        printf("Parent IP: %s My IP %s\n", src, src2);
    
        send_ipv6(&root_addr, src, src2);
        xtimer_sleep(5);
    }
#endif

    gnrc_netreg_unregister(GNRC_NETTYPE_IPV6, &server);

    return 0;
}
