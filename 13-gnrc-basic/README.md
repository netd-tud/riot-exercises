# Generic (GNRC) network stack basic (PHY and MAC Layer)

### Overview

This project demonstrates direct Layer-2 communication between two RIOT OS devices using the GNRC network stack.
One device acts as a sender, the other as a receiver. Packets are exchanged without IP, directly on the IEEE 802.15.4 PHY/MAC layer.

In addition to sending data, the receiver estimates the distance between the devices using the RSSI value of the received packet.

## Task 1

Modify the communication and set the panID and the channel.

```
int16_t panID = 0x1234;

if (gnrc_netapi_set(netif->pid, NETOPT_NID, 0, &panID, sizeof(panID)) < 0) {
    puts("Failed to set Pan ID!");
}
```

Experiment with different settings like csma  (https://doc.riot-os.org/group__net__netopt.html#ga19e30424c1ab107c9c84dc0cb29d9906).


## Task 2

Send a valid L2 packet.

### Sender

- Builds a custom L2 packet using GNRC
- Attaches a link-layer header
- Sends packets:
  - Either as broadcast
  - Or directly to a specific MAC address
- Packets are sent every 5 seconds

No IPv6 or higher-layer protocol is used — GNRC handles pure L2 forwarding.

```
/* Set the network interface */
gnrc_netif_hdr_set_netif(netif_hdr->data, netif);

gnrc_netif_hdr_t *neth = (gnrc_netif_hdr_t *)netif_hdr->data;
neth->flags |= GNRC_NETIF_HDR_FLAGS_BROADCAST;

/* Prepend header to payload */
gnrc_pktsnip_t *pkt = gnrc_pkt_prepend(payload, netif_hdr);

/* Send packet to network interface - GNRC will handle L2 forwarding */
if (!gnrc_netif_send(netif, pkt)) {
	puts("Failed to send packet");
	gnrc_pktbuf_release(pkt);
	return 1;
}
```

Check if you receive the Broadcast in your receiver device.

Change the code to only send to your receiver via mac (https://doc.riot-os.org/group__net__gnrc__netif__hdr.html#ga20cd0f0071d5efac972f7a5954438b52).

## Task 3

Read the received packet data.

### Receiver

- Registers for GNRC_NETTYPE_UNDEF to receive raw L2 packets
- Extracts:
  - Payload data
  - RSSI value from the network interface header
- Prints the received message

```
gnrc_pktsnip_t *pkt = msg.content.ptr;
    
if (pkt->next) {
	//Calculate DISTANCE from rssi and txpower
	
	gnrc_netif_hdr_t *hdr = pkt->next->data;
	int rssi_raw = (int)hdr->rssi;

	/* Calculate the DISTANCE */
	
	printf("Distance: %dcm\n", distance);

}
```

This is the formula for calculating the distance between the two devices:

d=10^((Tx−RSSI)/(10⋅n))
	​

**Short explanation**


* **(d)** = estimated distance (usually in meters)
* **(Tx)** = transmit power at 1 meter (in dBm)
* **(RSSI)** = received signal strength (in dBm)
* **(n)** = path-loss exponent (environment factor)

Note: Tx typically ranges from -60dBm to +8dBm for the feather sense nrf52480.

The idea: radio signals lose strength logarithmically with distance.
The parameter **(n)** models the environment:

* Free space: (n \approx 2)
* Indoors: (n \approx 2.7–4)
* Urban/obstructed: (n \ge 4)

⚠️ This gives only a **rough estimate**—walls, reflections, antenna orientation, and interference can cause large errors.
