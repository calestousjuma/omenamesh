

#ifndef THREAD_CONTEXT
#define THREAD_CONTEXT

#include "net.h"
#include "packet.h"
#include "types.h"

OMENAMESH_API PacketQueue send_queue;
OMENAMESH_API PacketQueue recv_queue;

typedef PacketQueue ThreadContextPacket;

typedef struct {
	// union {
	// int client_sockfd;
	// int serv_sockfd;
	// } fd;
	i32__CJLF client_sockfd;
	i32__CJLF serv_sockfd;
	char my_ip[ADDRESS];
	ThreadContextPacket *recv_q;
	ThreadContextPacket *send_q;
} ThreadContext;

BEFORE OMENAMESH_API ThreadContext *boot_comm_thread_context();

typedef struct Connection {
	MeshNode *nodes[MAX_NODES];
	/*this is the dns thingy i was talking about in net.h*/
	char *names[MAX_NODES];
	i32__CJLF client_sockfd;
	i32__CJLF server_sockfd;
	i64__CJLF count;
} Connection;

OMENAMESH_API Connection conns;

OMENAMESH_API __CJLF_GENERICS add_connection(const char *, const char *, int);

OMENAMESH_API __CJLF_GENERICS *send_packet_thread(void *arg);
OMENAMESH_API __CJLF_GENERICS *receive_packet_thread(void *arg);

#endif /*! THREAD_CONTEXT*/