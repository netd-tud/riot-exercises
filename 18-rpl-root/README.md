# RPL Root

Routing Protocol for Low-Power and Lossy Networks (RPL) is a routing protocol for wireless networks with low power consumption and generally susceptible to packet loss. It operates on IEEE 802.15.4. RPL creates a tree topology. Each node within the network has an assigned rank, which increases as the node moves away from the root node.

In this example, we quickly show how to implement a RPL root node. RIOT provides RPL modeles, which can help set up a RPL network. With `auto_init_gnrc_rpl` module, RIOT will automatically initialize the RPL stack. By default, every device operates as a standard routing node, unless explicitly configured as a root node. Through `gnrc_rpl_root_init()` function, we can designate a node as the root node.

To change to this directory from a different exercise, use the following command in the terminal.

```sh
$ cd ../20-rpl-root
```

## Task 1
**Print the RPL tree.**

An important concept in RPL is the RPL tree. It represents the topology of the network. 

The following code helps to print the RPL tree.

```C
ipv6_addr_to_str(dst_str, &entry.dst, sizeof(dst_str));
/* If the last 8 bytes of the target IP and the last 8 bytes of the next hop physical IP are the same, it means there is no intermediate node */
if (memcmp(&entry.dst.u8[8], &entry.next_hop.u8[8], 8) == 0) {
    printf("Root <- %s\n", dst_str);
} else {
    /* If they are different, it means there is an intermediate node */
    ipv6_addr_to_str(hop_str, &entry.next_hop, sizeof(hop_str));
    printf("%s <- %s\n", hop_str, dst_str);
}
```

There is only one root node in a RPL network. Hence, this example should only be flashed to one board. The later example [21-rpl-node](../21-rpl-node/README.md) is to realize a regular routing node, and can be flashed to multiple boards. After flashing both examples to multiple boards, the RPL network should be formed, and the topology of the network can be seen through the `print_rpl_tree()` function in [20-rpl-root](../20-rpl-root/README.md).
