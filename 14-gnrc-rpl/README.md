# Generic (GNRC) network stack RPL Example

## Introduction

In this tutorial we build a **multi-hop IPv6 network using RPL** (Routing Protocol for Low-Power and Lossy Networks) on RIOT OS.
One node acts as the **RPL DODAG root**, while all other nodes automatically join the network and select parents.

The goal is to **visualize the RPL topology** as a tree by:

* Having each node periodically send its **parent IPv6 address** and **own IPv6 address**
* Collecting this information at the root
* Building a **parent → child mapping** in a simple tree-like data structure

This approach demonstrates:

* Raw IPv6 packet handling in GNRC
* RPL parent selection and multi-hop forwarding
* Custom application-layer protocols on top of IPv6
* Simple topology reconstruction at the root

---

## How to Run

Flash **one node as the root** and the others as regular nodes:

```bash
# Root node
make all flash term PORT=/dev/ttyACM0 ROOT=1

# Non-root nodes
make all flash term PORT=/dev/ttyACM1 ROOT=0
make all flash term PORT=/dev/ttyACM2 ROOT=0
...
```

All nodes must be within radio range to form an RPL topology.

If there are multiple Root Nodes change the Instance ID in these:

```
gnrc_rpl_root_init(1, &dodag_id_a, false, false);
gnrc_rpl_instance_t *inst = gnrc_rpl_instance_get(1);
```

---

## RPL Overview (Conceptual Background)

RPL builds a **Destination-Oriented Directed Acyclic Graph (DODAG)** rooted at a single node.

### Key Concepts

* **DODAG Root**
  The root advertises itself as the destination for all traffic.

* **DIO (DODAG Information Object)**
  Periodic control messages sent by nodes to announce:

  * DODAG ID
  * Rank (distance to root)
  * Routing metrics

* **Rank**
  A scalar value indicating distance to the root.
  Lower rank = closer to the root.

* **Parent Selection**
  Each node selects one or more parents based on rank and link quality.

* **Upward Routing**
  Packets addressed to the root automatically traverse multiple hops via parents.

In RIOT OS, **GNRC RPL handles all of this automatically** once initialized.

---

## Code Structure Overview

Before diving into the tasks, let’s briefly explain the main helper functions and data structures.

---

### Data Structure: Parent Mapping

The topology-map is stored as a fixed-size array:

```c
#define MAX_NODES 16
ip_map_entry_t map[MAX_NODES];
```

---

### `map_add()`

```c
int map_add(const ipv6_addr_t *key, const ipv6_addr_t *value) { ... }
```
* Inserts or updates a `(parent → child)` entry.

* This keeps the topology consistent as nodes periodically resend information.

---

### `parse_ip_pair()`

```c
int parse_ip_pair(const char *input, ipv6_addr_t *key, ipv6_addr_t *value)
```
* Convert packet payload into sender and sender_parent ip address.

---

### `send_ipv6()`

```c
int send_ipv6(ipv6_addr_t *dst, const char *msg1, const char *msg2)
```

Concatenates two strings into one payload
Wraps it in:
  * Custom payload (`GNRC_NETTYPE_UNDEF`)
  * IPv6 header
  * Link-layer header
Sends it using GNRC’s IPv6 stack

This function demonstrates **manual IPv6 packet construction**.

---

### `process_incoming_data()`

```c
void process_incoming_data(char *data)
```

* Called only on the root node
* Parses incoming `(parent, child)` information
* Inserts it into the map
* Prints the current network topology

---

## TASK 1 – Initialize RPL and Join the Network

### Objective

Enable RPL on all nodes and register a custom IPv6 protocol.

### Key Code

```c
gnrc_rpl_init(netif->pid);
```

* Starts RPL on the given network interface
* Non-root nodes automatically listen for DIOs
* Parent selection is handled internally

```c
gnrc_netreg_entry_t server =
    GNRC_NETREG_ENTRY_INIT_PID(253, thread_getpid());

int reg_result = gnrc_netreg_register(GNRC_NETTYPE_IPV6, &server);
```

* Registers a custom IPv6 protocol number (`253`)
* All IPv6 packets with nh = 253 are delivered to this thread

---

## TASK 2 – Create the RPL Root and Send IPv6 Data

### Objective

Turn one node into the DODAG root and enable multi-hop communication.

### Root Initialization

```c
ipv6_addr_from_str(&dodag_id, "2001:db8::1");
gnrc_netif_ipv6_addr_add(netif, &dodag_id, 64, 0);

gnrc_rpl_instance_t *inst = gnrc_rpl_root_init(1, &dodag_id, false, false);
```

* Assigns a global IPv6 address
* Starts advertising the DODAG
* Other nodes automatically attach

---

### Non-Root Behavior

Each non-root node periodically sends:

```
(parent_ip, own_ip)
```

```c
ipv6_addr_to_str(src, &inst->dodag.parents->addr, sizeof(src));
ipv6_addr_to_str(src2, &netif->ipv6.addrs[0], sizeof(src2));

send_ipv6(&root_addr, src, src2);
```

RPL ensures packets reach the root **via multi-hop forwarding**.

---

## TASK 3 – Visualize the Network Topology

### Objective

Reconstruct the RPL tree at the root.

### How It Works

1. Root receives IPv6 packets
2. Payload is parsed into `(parent, child)`
3. Mapping is stored in `map[]`
4. Topology can be printed or further processed

This forms a tree representation of the RPL DODAG.
