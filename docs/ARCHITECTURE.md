<b><u> model</u> </b> Application(primarily written in kotlin) & mesh lib(written in C)

<u> > Mesh Lib(C)</u>

1. complete mesh network.
2. offline & secure messaging.
3. Sharing files & other multimedia data.
4. Broadcast Message across connected Nodes.
5. Access to the internet.
6. Idea of a server.

<u> > Application(kotlin)</u>

1. C mesh lib bindings with kotlin Cintrop(Native & tailored to android).
2. Simple UI.

NOTE: "internet" might be used, mostly to mean peer to peer(P2P).

Any phone can send [HTTP requests](#model) even if it's not directly connected to the internet (ad-hoc, a decentralized system, no phone(node) controls the traffic).

The mesh network routes and queues packets until they reach the gateway phone.
If a phone disconnects, queued packets wait until a new path is found.

Packet Routing Between Nodes

-   If Phone A is connected to the internet but Phone C is not, Phone B (in between) must forward packets from C → B → A.
    Each node checks if it’s the final destination or forwards the packet.

-   Queueing & Forwarding Requests
    Devices that can't reach the internet queue outgoing packets and forward them to the next best node.
    If the connection drops, packets stay in the queue until reconnected.

-   Maintaining a Routing Table

    Each phone remembers which device is connected to the internet.
    If a device forwards packets through multiple nodes, it finds the shortest path (like in AODV routing).

<a href="#model"> Steps to Implement Mesh Packet Forwarding </a>

1. Packet Queueing: Store packets if the internet node is unreachable.
2. Route Discovery: Nodes broadcast "HELLO" messages to learn neighbors.
3. Forwarding Logic: If a node receives a packet:

-   If it's the destination, process it.
-   If it knows a path, forward it.
-   If not, flood it (send to all neighbors).

4. NAT or VPN Proxy: If a phone has internet, it acts as a proxy for all others.
5. Handling Failures: If a route is broken, requeue packets and find another path.
