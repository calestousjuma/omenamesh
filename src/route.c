
#include "route.h"
#include "packet.h"
#include "types.h"

__CJLF_GENERICS add_route(const char *ip, i32__CJLF socket_fd) {
	for (i64__CJLF i = 0; i < MAX_NODES; i++) {
		if (Routes[i].socket_fd == 0) {
			strcpy(Routes[i].ip, ip);
			Routes[i].socket_fd = socket_fd;
			break;
		}
	}
}

#define IP_ADDR_LEN ADDRESS

typedef struct {
	char ip[IP_ADDR_LEN];
	bool acknowledged;
	bool blacklisted;
} Node;

static Node routing_table[MAX_NODES];
static int node_count = 0;

static Node *find_node(const char *ip) {
	for (int i = 0; i < node_count; ++i) {
		if (strncmp(routing_table[i].ip, ip, IP_ADDR_LEN) == 0) {
			return &routing_table[i];
		}
	}
	return NULL;
}

bool is_known_node(const char *ip) {
	return find_node(ip) != NULL;
}

void add_to_routing_table(const char *ip) {
	if (is_known_node(ip) || node_count >= MAX_NODES)
		return;
	strncpy(routing_table[node_count].ip, ip, IP_ADDR_LEN);
	routing_table[node_count].acknowledged = false;
	routing_table[node_count].blacklisted = false;
	node_count++;
}

void mark_packet_acknowledged(const char *ip) {
	Node *n = find_node(ip);
	if (n)
		n->acknowledged = true;
}

void remove_pending_ack(const char *ip) {
	Node *n = find_node(ip);
	if (n)
		n->acknowledged = false;
}

void blacklist_node(const char *ip) {
	Node *n = find_node(ip);
	if (n)
		n->blacklisted = true;
}
