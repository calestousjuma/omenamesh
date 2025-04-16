//! step into chaos
//!
//!
//! CJLF 2025(c)

//!
#ifndef COMMON_H

#define COMMON_H

#include "types.h"
typedef struct __queue_t {
	struct __queue_t *front;
	struct __queue_t *rear;
	i64__CJLF size;
} Queue;

typedef struct __array {
	i64__CJLF *members;
	i64__CJLF cap;
	i64__CJLF len;
} ArrayList;

/*!
 *! A VirtualPacket represents a packet being sent over the virtual mesh
 *! network. The packet data includes a header (which contains the virtual
 *! to/from address and port, hop count, max hops and payload size) followed
 *! by a payload of a given length in bytes as specified within the header.
 *!
 *!
 *! @param data ByteArray to be used to store data
 *! @param _IO_buf_base the offset to the start of the data (the header data
 *! begins at offset, payload data begins at
 *! dataOffset+VirtualPacketHeader.HEADER_SIZE)
 *! @param header if the header is supplied -
 *! it will be written into the data. If the header is not supplied,
 *! the header data MUST be in data and it will be read when constructed */

typedef struct __virtual_packet {
	char *_IO_buf_base; /* Start of reserve area. */
	struct __virtual_header *header;
	char *data; /* Flexible array for payload */
} VirtualPacket;

/**
 * Virtual packet structure
 * Header:
 * @param toAddr - the Virtual Node address this packet wants to reach. 0 = send
 * over local link only.
 * @param port the Virtual Port on the destination Vitual Noe that this packet
 * wants to reach
 * @param fromAddr the Virtual Node that originally sent this packet
 * @param fromPort the virtual port that this packet was sent from
 * @param lastHopAddr the virtual node address of the most recent hop. E.g.
 * where the packet is sent from node A to node B, then node B to node C, the
 * lastHop would be A when the packet is sent from A to B, and then B when the
 * packet is sent from B to C.
 * @param hopCount the total number of hops this packet has taken. Starts at 1
 * when first sent and is incremented on each hop.
 * @param maxHops the maximum number of hops that this packet should live for.
 * If exceeded, packet is dropped
 * @param payloadSize the size of the payload data
 *
 */
struct __virtual_header {
	/* Upper 16 bits for metadata, lower 48 bits for payload data */
	u64__CJLF metadatau64__CJLF;
	u64__CJLF toAddr : 48;
	u64__CJLF toPort : 16;
	u64__CJLF fromAddr : 48;
	u64__CJLF fromPort : 16;
	u64__CJLF lastHopAddr : 48;
	u64__CJLF hopCount : 8;
	u64__CJLF maxHops : 8;
	u64__CJLF payloadSize : 16;
};

/*page table entries virtual & real IP*/
#define HASH_SIZE 4096
extern struct __entries_t *hashmap[HASH_SIZE];

#define VOLATILE_API __attribute__((visibility("hidden")))  // DONT

extern unsigned int hash_virtual_ip(const char *ip);
extern bool hash_insert_virtual_ips(char *ip);
#define UNUSED(x) (void)(x)

/*cursor for our seed table*/
typedef enum {
	_UNUSED = 0,
	_USED,
	_SPOILT,
} VirtualIpState;

typedef struct __table {
	char **entries;
	char *ip;
	i64__CJLF cursor;
	VirtualIpState *flags;
} Table;

OMENAMESH_API char *get_next_virtual_ip();
OMENAMESH_API char *hash_lookup(const char *virtual_ip);

OMENAMESH_API struct __table *tables;

#define FF (unsigned char)~0

/*! FF:FF:FF */
#define BROADCAST_IP(ip)                                                 \
	do {                                                             \
		snprintf(ip, sizeof(ip), "%d.%d.%d.%d", FF, FF, FF, FF); \
	} while (0)

#define SEED 10
#define ADDRESS_LEN 16

/*!mem.c*/
extern void *xcalloc(size_t members, size_t block);
extern void *xmalloc(size_t block);
void *xrealloc(void *oldptr, size_t new_size);

#define SWAP(T, a, b)    \
	do {             \
		T t = a; \
		a = b;   \
		b = t;   \
	} while (0)

#define UNIMPLEMENTED(...)                                                \
	do {                                                              \
		printf("%s:%d: UNIMPLEMENTED: %s \n", __FILE__, __LINE__, \
		       __VA_ARGS__);                                      \
		exit(1);                                                  \
	} while (0)

/*! before everything else, can we just have 100 virual IPs ready? */
BEFORE OMENAMESH_API __CJLF_GENERICS seed_virtual_ips();

/*Make Local? */
OMENAMESH_API __CJLF_GENERICS initialize_table(i64__CJLF cap, i64__CJLF block);

/*TODO file snif.c, find a good name for the file*/
OMENAMESH_API __CJLF_GENERICS ___confirm_broadcast_sent();

#endif
