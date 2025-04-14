#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"

#define HEADER_SIZE sizeof(struct __virtual_header)

void *send_packet(void *arg) {
	VirtualPacket *packet = (VirtualPacket *)arg;
	// Logic for sending the packet across a socket (e.g., using sendto() or
	// sendmsg)
	printf("Sending packet with toAddr: %llu and payload size: %d\n",
	       packet->header->toAddr, packet->header->payloadSize);
	return NULL;
}

void *receive_packet(void *arg) {
	VirtualPacket *packet = arg;

	printf("Received packet fromAddr: %llu and hop count: %d\n",
	       packet->header->fromAddr, packet->header->hopCount);
	return NULL;
}

int main() {
	// VirtualPacket packet;

	// struct __virtual_header header = {
	// 	.toAddr = 12345,
	// 	.toPort = 8080,
	// 	.fromAddr = 54321,
	// 	.fromPort = 9090,
	// 	.lastHopAddr = 12345,
	// 	.hopCount = 1,
	// 	.maxHops = 5,
	// 	.payloadSize = 512, /*strict MTU*/
	// };

	// packet.header = &header;
	// packet._IO_buf_base = (char *)xmalloc(HEADER_SIZE +
	// header.payloadSize);

	// // Allocate space for the payload data (this is a simple example,
	// you'd
	// // handle payload appropriately)
	// packet.data = packet._IO_buf_base + HEADER_SIZE;

	// pthread_t sender_thread, receiver_thread;

	// // Create a sender thread to simulate sending a packet
	// pthread_create(&sender_thread, NULL, send_packet, (void *)&packet);

	// // Create a receiver thread to simulate receiving a packet
	// pthread_create(&receiver_thread, NULL, receive_packet, (void
	// *)&packet);

	// // Wait for threads to finish
	// pthread_join(sender_thread, NULL);
	// pthread_join(receiver_thread, NULL);

	// free(packet._IO_buf_base);

	return 0;
}
