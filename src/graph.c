
#include "common.h"
#include "net.h"

__CJLF_GENERICS init_graph_network(MeshGraph *graph, i8__CJLF vertixxx) {
	graph->num_nodes = vertixxx;
	for (int q = 0; q < vertixxx; vertixxx++)
		graph->nodes[q] = Nil;
}
MeshNode *create_node(const i8__CJLF *ip, int port, const char *username) {
	MeshNode *node = xmalloc(sizeof(*node));

	strncpy((char *)node->ip, (char *)ip, ADDRESS);
	node->port = port;
	node->connected = false;
	node->node_packets = Nil;
	node->name = xmalloc(strlen(username));

	return node;
}

__CJLF_GENERICS add_node(MeshGraph *graph, MeshNode *node) {
	if (graph->num_nodes >= MAX_NODES)
		return;
	graph->nodes[graph->num_nodes++] = node;
}

__CJLF_GENERICS remove_node(MeshGraph *graph, const char *ip) {
	for (int i = 0; i < graph->num_nodes; i++) {
		if (strcmp((char *)graph->nodes[i]->ip, ip) == 0) {
			free(graph->nodes[i]);
			graph->nodes[i] = graph->nodes[--graph->num_nodes];
			return;
		}
	}
}

__CJLF_GENERICS connect_nodes(MeshNode *a, MeshNode *b) {
	if (a->neighbor_count >= MAX_NEIGHBORS ||
	    b->neighbor_count >= MAX_NEIGHBORS)
		return; /*! BRO ITS ENOUGH, should we keep a node in queue until
			   another node leaves the mesh?*/

	a->neighbors[a->neighbor_count++] = b;
	b->neighbors[b->neighbor_count++] = a;
}

__CJLF_GENERICS disconnect_nodes(MeshNode *a, MeshNode *b) {
	for (int i = 0; i < a->neighbor_count; i++) {
		if (a->neighbors[i] == b) {
			a->neighbors[i] = a->neighbors[--a->neighbor_count];
			break;
		}
	}
	for (int i = 0; i < b->neighbor_count; i++) {
		if (b->neighbors[i] == a) {
			b->neighbors[i] = b->neighbors[--b->neighbor_count];
			break;
		}
	}
}

bool send_message(MeshNode *src,
		  MeshNode *dest,
		  const char *msg,
		  uint8_t type) {
	if (!src || !dest)
		return false;

	Packet *pkt = (Packet *)xmalloc(sizeof(Packet));

	pkt->type = type;
	strcpy((char *)pkt->src_ip, (const char *)src->ip);
	strcpy((char *)pkt->dest_ip, (const char *)dest->ip);
	pkt->payload_size = strlen(msg);
	strncpy(pkt->data, msg, MAX_DATA_SIZE);

	printf("Message sent from %s to %s: %s\n", src->ip, dest->ip, msg);
	return true;
}

// HUHHHHH ?
__CJLF_GENERICS receive_message(MeshNode *node, Packet *pkt) {
	if (!node || !pkt)
		return;
	printf("Message received at %s: %s\n", node->ip, pkt->data);
}

#include <fcntl.h>

void send_file(MeshNode *src,
	       MeshNode *dest,
	       const char *filepath,
	       bool *status) {
	/*! UNIX VFS*/
	int fd = open(filepath, O_RDONLY);
	if (fd < 0)
		*status = false;

	Packet *pkt = (Packet *)xmalloc(sizeof(Packet));

	pkt->type = PACKET_TYPE_DATA;
	strcpy((char *)pkt->src_ip, (char *)src->ip);
	strcpy((char *)pkt->dest_ip, (char *)dest->ip);

	int bytes_read;
	while ((bytes_read = read(fd, pkt->data, MAX_DATA_SIZE)) > 0) {
		pkt->payload_size = bytes_read;
		printf("Sending %d bytes from %s to %s\n", bytes_read, src->ip,
		       dest->ip);
	}

	close(fd);
	*status = true;
}

/*remember to free this*/
i8__CJLF *save_packet_to_file(const char *src_ip,
			      int seq_num,
			      const char *data,
			      size_t len) {
	char filename[128];

	char ip_sanitized[32];
	snprintf(ip_sanitized, sizeof(ip_sanitized), "%s", src_ip);

	/*make filename from the node ip & seq_num, this can give use unique
	 * filenames, considering we have different IP for all the nodes*/
	for (char *p = ip_sanitized; *p; p++)
		if (*p == '.')
			*p = '_';

	snprintf(filename, sizeof(filename), "packet_%s_%d.bin", ip_sanitized,
		 seq_num);

	FILE *file = fopen(filename, "wb");
	if (file == 0)
		return 0;
	fwrite(data, 1, len, file);
	fclose(file);

	char *random_file_name = strdup(filename);
	return (i8__CJLF *)random_file_name;
}

bool receive_file(MeshNode *node, Packet *pkt) {
	if (!node || !pkt)
		return false;

	i8__CJLF *path = save_packet_to_file((char *)node->ip, pkt->seq_num,
					     pkt->data, pkt->payload_size);

	FILE *file = fopen((const char *)path, "wb" /*PORTABLE*/);
	if (file == 0)
		return false;

	/*THE PAYLOAD, sometimes might be bigger than our actual buf
	 * size(1024), */
	fwrite(pkt->data, 1, pkt->payload_size, file);
	fclose(file);

	printf("File receivedat %s\n", node->ip);
	return true;
}

// TODO , this is not a daemon
__CJLF_GENERICS *mesh_daemon(void *arg) {
	MeshNode *node = (MeshNode *)arg;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		return Nil;

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(node->port);

	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
	    0)
		return Nil;

	(__CJLF_GENERICS) listen(sockfd, 5);

	while (1) {
		int client_sock = accept(sockfd, Nil, Nil);
		if (client_sock > 0) {
			Packet pkt;
			recv(client_sock, &pkt, sizeof(pkt), 0);
			receive_message(node, &pkt);
		}
		close(client_sock);
	}
}
