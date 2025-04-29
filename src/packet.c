#include "packet.h"
#include "lock.h"

// TODO!  MACRO THIS
i8__CJLF is_packet_type(Packet_TYPE pType) {
	return pType & PACKET_MASK;
}

/* Mutex for synchronization (locking) */
/* queue packet - Add a packet to the queue & keep the writing as atomic as
 * possible, Locking to prevent simultaneous packet handling */

__CJLF_GENERICS queue_packet(PacketQueue *queue, Packet *pkt) {
	acquire_lock(global_lock_manager, 0, 0);

	if (queue->rear == Nil) {
		queue->front = queue->rear = pkt;
	} else {
		queue->rear->next = pkt;
		queue->rear = pkt;
	}
	release_lock(global_lock_manager, queue->size);
	queue->size++;
}

/* Remove and return the front packet of the queue */
Packet *dequeue_packet(PacketQueue *queue) {
	acquire_lock(global_lock_manager, 0, 0);

	if (queue->front == Nil)
		/* No packets in queue */
		goto defer;

	Packet *pkt = queue->front;
	queue->front = queue->front->next;
	if (queue->front == Nil)
		queue->rear = Nil;

	release_lock(global_lock_manager, queue->size);
	queue->size--;
	return pkt;

defer:
	release_lock(global_lock_manager, queue->size);
	return 0;
}

#include "common.h"

/* Forward packet - This function simulates sending/forwarding a packet
 */
__CJLF_GENERICS forward_packet(PacketQueue *queue, Packet *pkt) {
	//! TODO
	//! THIS IS STUPID, WE CANT BE STARTING A NEW SOCKET EVERYTIME,
	//! JUST ADD THIS AS PACKET METADATA SO WHEN ONE SOCKET OPENS
	//! THAT IS USED FOR EVERYTHING, ALSO SOFT RLIMIT FOR NOFILE IS
	//! 1024, EVERRY ROUTINE WAKING UP A SOCKET IS NOT COOL, SO
	//! PACKET SHOULD HAVE MORE METADATA
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		goto defer;
	/* use  virtual → real IP resolver */
	char *ip = hash_lookup(pkt->dest_ip);
	if (ip == Nil)
		goto defer;

	struct sockaddr_in dest;

	dest.sin_family = AF_INET;
	dest.sin_port = htons(PORT);

	inet_pton(AF_INET, ip, &dest.sin_addr);

	ssize_t sent = sendto(sockfd, pkt, sizeof(Packet), 0,
			      (struct sockaddr *)&dest, sizeof(dest));
	if (sent < 0)
		goto defer;

defer:
	if (sockfd > 0)
		close(sockfd);
	return;
}

/* Handle packet - Process received packets */
__CJLF_GENERICS handle_packet(PacketQueue *queue, Packet *pkt, char *my_ip) {
	switch (pkt->type) {
		// case NORMAL:
		// case HANDSHAKE:
		// 	perform_handshake(queue, pkt, my_ip);
		// 	break;
		// case ACK:
		// 	handle_acknowledgment(pkt);
		// 	break;
		// case HELLO:
		// 	discover_node(queue, pkt, my_ip);
		// 	break;
		// case AUTH:
		// 	authenticate_node(queue, pkt);
		// 	break;
		// case PACKET_ERROR:
		// 	handle_error(pkt->data);
		// 	break;
		// default:
		// 	handle_unknown_packet(pkt);
		// 	break;
	}
}