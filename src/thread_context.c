

//! This goes before main, wakes up a socket for client & server
//! routine ThreadContext *boot_comm_thread_context()
//! & is designed with thread safety in mind.
#include "common.h"
#include "context.h"
#include "types.h"

PacketQueue send_queue;
PacketQueue recv_queue;

ThreadContext *boot_comm_thread_context() {
#define KILL_EVERYTING_I_MEAN_EVEYTHING 1
	int server = socket(AF_INET, SOCK_DGRAM, 0);
	if (server < 0)
		goto kill;
	int client = socket(AF_INET, SOCK_DGRAM, 0);
	if (client < 0)
		goto kill;
	ThreadContext *ctx = xmalloc(sizeof(ThreadContext));
	strncpy(ctx->my_ip, get_next_virtual_ip(), ADDRESS);
	ctx->recv_q = &recv_queue;
	ctx->send_q = &send_queue;

	return ctx;
kill:
	if (server > 0)
		close(server);
	_Exit(KILL_EVERYTING_I_MEAN_EVEYTHING);
}

__CJLF_GENERICS *send_packet_thread(void *arg) {
	ThreadContext *ctx = (ThreadContext *)arg;

	for (;;) {
		struct __packet *get_packet = dequeue_packet(ctx->send_q);

		struct sockaddr_in dest_addr;

		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(PORT);
		dest_addr.sin_addr.s_addr = inet_addr(get_packet->dest_ip);

		struct iovec iov;

		iov.iov_base = get_packet->data;
		iov.iov_len = sizeof(get_packet->data);

		struct msghdr hdr;
		hdr.msg_name = &dest_addr;
		hdr.msg_namelen = sizeof(dest_addr);  // wait?
		hdr.msg_iov = &iov;
		hdr.msg_iovlen = sizeof(char);

		// unionise this
		sendmsg(ctx->client_sockfd, &hdr, 0);
		free(get_packet);
	};

	return Nil;
}

__CJLF_GENERICS *receive_packet_thread(void *arg) {
	ThreadContext *ctx = (ThreadContext *)arg;
	struct sockaddr_in src_addr;
	socklen_t addr_len = sizeof(src_addr);

	for (;;) {
		struct __packet *pkt = xmalloc(sizeof(struct __packet));
		ssize_t len =
		    recvfrom(ctx->client_sockfd, pkt, sizeof(Packet), 0,
			     (struct sockaddr *)&src_addr, &addr_len);

		if (len < 0) {
			free(pkt);
			continue;
		}

		if (strcmp(pkt->dest_ip, ctx->my_ip) == 0)
			handle_packet(ctx->recv_q, pkt, ctx->my_ip);
		else {
			queue_packet(ctx->send_q, pkt);
			forward_packet(ctx->send_q, pkt);
		}
	}
	return Nil;
}

struct Connection conns;

__CJLF_GENERICS add_connection(const char *name,
			       const char *virtual,
			       int port) {
	if (conns.count >= 10)
		return;

	MeshNode *node = create_node((const i8__CJLF *)virtual, port, name);
	conns.nodes[conns.count] = node;
	conns.names[conns.count] = strdup(name);
	conns.count++;
}