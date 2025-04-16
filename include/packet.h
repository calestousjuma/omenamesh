#ifndef PACKET_H
#define PACKET_H

#include <netinet/in.h>
#include "types.h"
#define MAX_DATA_SIZE 1024 /* MTU? */
#define ADDRESS INET_ADDRSTRLEN

/*TODO, read about linkage w/ static*/
static __CJLF_GENERICS set_nonblocking(int sock) {
	int flags = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flags | O_NONBLOCK);
}

//! TODO handle EADDRINUSE
#define PORT 8000
#define INCASE_SOMEONE_IS_USING_THE_PORT

i8__CJLF start_server_t();
int connect_to_peer(const char *peer_ip);

#define PACKET_MASK 0x7
typedef enum {
// Normal data packet
#define PACKET_TYPE_DATA 0x01
	NORMAL = PACKET_TYPE_DATA,
// Initial handshake packet
#define PACKET_TYPE_HANDSHAKE 0x02
	HANDSHAKE,
// Acknowledgment packet
#define PACKET_TYPE_ACK 0x03
	ACK,
// Node discovery packet
#define PACKET_TYPE_HELLO 0x04
	HELLO,
// Authentication request packet
#define PACKET_TYPE_AUTH 0x05
	AUTH,
// Error notification packet
#define PACKET_TYPE_ERROR 0x06
	PACKET_ERROR
} Packet_TYPE;

// SOCKET SOCKET SOCKET SOCKET SOCKET SOCKET
typedef struct __packet {
	/* Packet type */
	u8__CJLF type;
	/* Source IP*/
	char src_ip[ADDRESS];
	/* Destination IP*/
	char dest_ip[ADDRESS];
	/* Sequence number (for ordering)*/
	u16__CJLF seq_num;
	/* Time-to-Live (hops before discard)*/
	u8__CJLF ttl;
	/*Actual data length */
	u16__CJLF payload_size;
	/*payload*/
	char data[MAX_DATA_SIZE];
	/*queue the next packet*/
	struct __packet *next;
} Packet;

/*Manage*/
__CJLF_GENERICS deserialize_packet(const char *buffer, Packet *pkt);
__CJLF_GENERICS serialize_packet(const Packet *pkt, char *buffer);

/*queue packet*/
typedef struct {
	Packet *front;
	Packet *rear;
	int size;
} PacketQueue;

/*packet queue*/
OMENAMESH_API __CJLF_GENERICS queue_packet(PacketQueue *queue, Packet *pkt);
OMENAMESH_API Packet *dequeue_packet(PacketQueue *queue);
OMENAMESH_API __CJLF_GENERICS forward_packet(PacketQueue *queue, Packet *pkt);
OMENAMESH_API __CJLF_GENERICS handle_packet(PacketQueue *queue,
					    Packet *pkt,
					    char *my_ip);

typedef struct RoutingTable {
	char dest_ip[16];
	char next_hop[16];
	int hops;
} RoutingTable;

#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>

/* Process the contents of a normal data packet */
OMENAMESH_API_T __CJLF_GENERICS process_data(char *data);

/* Perform handshake operations for establishing a connection */
OMENAMESH_API_T __CJLF_GENERICS perform_handshake(PacketQueue *queue,
						  Packet *pkt,
						  char *my_ip);

/* Handle acknowledgment of previously sent packets */
OMENAMESH_API_T __CJLF_GENERICS handle_acknowledgment(Packet *pkt);

/* Discover or register a new node from a HELLO packet */
OMENAMESH_API_T __CJLF_GENERICS discover_node(PacketQueue *queue,
					      Packet *pkt,
					      char *my_ip);

/* Process an authentication request from another node */
OMENAMESH_API_T __CJLF_GENERICS authenticate_node(PacketQueue *queue,
						  Packet *pkt);

/* Handle an error reported in a PACKET_ERROR packet */
OMENAMESH_API_T __CJLF_GENERICS handle_error(char *err);

/* Process packets of unknown or unsupported types */
OMENAMESH_API_T __CJLF_GENERICS handle_unknown_packet(Packet *pkt);

#endif /*   !packet.h*/