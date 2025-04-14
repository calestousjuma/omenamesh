#ifndef NET__H
#define NET__H

/*arr of found cap 10 */

#include "types.h"
#define MAX_NODES 10
#include "packet.h"

#define PING_TIMEOUT /*under 10 */ MAX_NODES
#define MAX_NETWORKS PING_TIMEOUT

typedef struct {
	i8__CJLF ip[ADDRESS];
} DiscoveredNetwork;

extern i32__CJLF network_count;
extern DiscoveredNetwork discovered[MAX_NODES];
extern i32__CJLF clients[MAX_NODES];

#define MAX_NEIGHBORS 10
#define MAX_DATA_SIZE 1024

/*POSIX*/
OMENAMESH_API i32__CJLF dicsover_networks_around_me();

OMENAMESH_API __CJLF_GENERICS send_udp_broadcast();

typedef struct __mesh_node {
	// established connection
	bool connected;
	// IP address (IPv6 support)/ this is src ip
	i8__CJLF ip[ADDRESS];
	// Node send/rcvs packets
	Packet *node_packets;
	// Port for server (could be dynamic)
	i32__CJLF port;
	// List of neighboring nodes
	struct __mesh_node *neighbors[MAX_NEIGHBORS];
	// Pointer to the next node in the mesh
	struct __mesh_node *next;

	/*TODO */
	i8__CJLF neighbor_count;

	// ok hear me out DNS for Node, Nobody is seriously going to remember
	// IP, atleast we can figure out how to get messages sent over specific
	// username, i will figure this out.
	char *name;
} MeshNode;

/* Mesh network graph holding all nodes in the mesh */
typedef struct {
	// Graph adjacency list
	MeshNode *nodes[MAX_NODES];
	// max out at MAX_NODES
	int num_nodes;
} MeshGraph;

/*EXPERIMENTAL*/

OMENAMESH_API __CJLF_GENERICS init_graph_network(MeshGraph *graph,
						 i8__CJLF vertixxx);

OMENAMESH_API __CJLF_GENERICS handle_new_connection(int server_fd);
OMENAMESH_API_T __CJLF_GENERICS handle_user_input();
OMENAMESH_API __CJLF_GENERICS handle_client_data(int client_fd, int index);
/**
 * Creates a new mesh node with the given IP and port.
 * @param ip - The IP address of the node.
 * @param port - The port number for the node.
 * @return Pointer to the created MeshNode.
 */
OMENAMESH_API MeshNode *create_node(const i8__CJLF *ip, int port, const char *);

/**
 * Adds a node to the mesh graph.
 * @param graph - The mesh graph to which the node will be added.
 * @param node - The node to be added.
 */
OMENAMESH_API __CJLF_GENERICS add_node(MeshGraph *graph, MeshNode *node);

/**
 * Removes a node from the mesh graph based on its IP address.
 * @param graph - The mesh graph from which the node will be removed.
 * @param ip - The IP address of the node to remove.
 */
OMENAMESH_API __CJLF_GENERICS remove_node(MeshGraph *graph, const char *ip);

/**
 * Establishes a direct connection between two mesh nodes.
 * @param a - First node.
 * @param b - Second node.
 */
OMENAMESH_API __CJLF_GENERICS connect_nodes(MeshNode *a, MeshNode *b);

/**
 * Disconnects two previously connected mesh nodes.
 * @param a - First node.
 * @param b - Second node.
 */
OMENAMESH_API __CJLF_GENERICS disconnect_nodes(MeshNode *a, MeshNode *b);

/**
 * Sends a message from one node to another.
 * @param src - The sender node.
 * @param dest - The recipient node.
 * @param msg - The message to be sent.
 * @param type - The packet type (e.g., data, handshake, ACK).
 * @return true if the message was successfully sent, false otherwise.
 */
OMENAMESH_API bool send_message(MeshNode *src,
				MeshNode *dest,
				const char *msg,
				uint8_t type);

/**
 * Processes an incoming message for a node.
 * @param node - The node receiving the message.
 * @param pkt - The received packet.
 */
OMENAMESH_API __CJLF_GENERICS receive_message(MeshNode *node, Packet *pkt);

/**
 * Sends a file from one node to another.
 * @param src - The sender node.
 * @param dest - The recipient node.
 * @param filepath - Path to the file to be sent.
 * @param status - status
 * @return true if the file transfer started successfully, false otherwise.
 */
OMENAMESH_API void send_file(MeshNode *src,
			     MeshNode *dest,
			     const char *filepath,
			     bool *status);

/**
 * Handles receiving a file at a given node.
 * @param node - The node receiving the file.
 * @param pkt - The received packet containing file data.
 * @return true if the file was successfully received, false otherwise.
 */
OMENAMESH_API bool receive_file(MeshNode *node, Packet *pkt);

/**
 * The main daemon responsible for managing the mesh network.
 * This function runs in the background to handle packet routing,
 * node discovery, and data transmission.
 * @param arg - Argument for the daemon (typically a MeshGraph pointer).
 * @return A pointer to the result (if needed).
 */
OMENAMESH_API __CJLF_GENERICS *mesh_daemon(__CJLF_GENERICS *arg);

#include <ifaddrs.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

/*WIFI DIRECT*/
__CJLF_GENERICS get_local_ip(char *buffer, i64__CJLF len, bool *status);

#endif /* NET__H*/