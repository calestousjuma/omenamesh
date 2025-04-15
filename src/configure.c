//! TODO
#include "packet.h"

/* Process the contents of a normal data packet */
OMENAMESH_API_T __CJLF_GENERICS process_data(char *data) {}

/* Perform handshake operations for establishing a connection */
OMENAMESH_API_T __CJLF_GENERICS perform_handshake(PacketQueue *queue,
						  Packet *pkt,
						  char *my_ip) {}

/* Handle acknowledgment of previously sent packets */
OMENAMESH_API_T __CJLF_GENERICS handle_acknowledgment(Packet *pkt) {}

/* Discover or register a new node from a HELLO packet */
OMENAMESH_API_T __CJLF_GENERICS discover_node(PacketQueue *queue,
					      Packet *pkt,
					      char *my_ip) {}

/* Process an authentication request from another node */
OMENAMESH_API_T __CJLF_GENERICS authenticate_node(PacketQueue *queue,
						  Packet *pkt) {}

/* Handle an error reported in a PACKET_ERROR packet */
OMENAMESH_API_T __CJLF_GENERICS handle_error(char *err) {}

/* Process packets of unknown or unsupported types */
OMENAMESH_API_T __CJLF_GENERICS handle_unknown_packet(Packet *pkt) {}