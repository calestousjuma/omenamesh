
#ifndef KOTLIN_BINDINGS
#define KOTLIN_BINDINGS

#include "jni.h"

#ifdef JNICALL

struct __mesh_graph;
struct __mesh_node;

//! UI
JNIEXPORT void seed_virtual_ips(JNIEnv *env);
JNIEXPORT jbyte *get_next_virtual_ip(JNIEnv *env);
JNIEXPORT jbyte *hash_lookup(JNIEnv *env, jbyte *virtual_ip);
JNIEXPORT void initialize_table(JNIEnv *env, jlong cap, jlong block);
JNIEXPORT void ___confirm_broadcast_sent(JNIEnv *env);
JNIEXPORT jint hash_virtual_ip(jbyte *ip);
JNIEXPORT jboolean hash_insert_virtual_ips(jbyte *ip);

#define UNDEFINED __attribute__((visibility("hidden")))

#ifdef EXPERIMENTAL
/* Process the contents of a normal data packet */
UNDEFINED void process_data(char *data);

/* Perform handshake operations for establishing a connection */
UNDEFINED void perform_handshake(PacketQueue *queue, Packet *pkt, char *my_ip);

/* Handle acknowledgment of previously sent packets */
UNDEFINED void handle_acknowledgment(Packet *pkt);

/* Discover or register a new node from a HELLO packet */
UNDEFINED void discover_node(PacketQueue *queue, Packet *pkt, char *my_ip);

/* Process an authentication request from another node */
UNDEFINED void authenticate_node(PacketQueue *queue, Packet *pkt);

/* Handle an error reported in a PACKET_ERROR packet */
UNDEFINED void handle_error(char *err);

/* Process packets of unknown or unsupported types */
UNDEFINED void handle_unknown_packet(Packet *pkt);

#endif
typedef struct __mesh_graph MeshGraph;
typedef struct __mesh_node MeshNode;

JNIEXPORT void init_graph_network(JNIEnv *env,
				  MeshGraph *graph,
				  jbyte vertixxx);

JNIEXPORT void handle_new_connection(JNIEnv *env, int server_fd);
JNIEXPORT void handle_user_input(JNIEnv *env);
JNIEXPORT void handle_client_data(JNIEnv *env, int client_fd, int index);
/**
 * Creates a new mesh node with the given IP and port.
 * @param ip - The IP address of the node.
 * @param port - The port number for the node.
 * @return Pointer to the created MeshNode.
 */
JNIEXPORT MeshNode *create_node(JNIEnv *env, jbyte *ip, int port, const char *);

/**
 * Adds a node to the mesh graph.
 * @param graph - The mesh graph to which the node will be added.
 * @param node - The node to be added.
 */
JNIEXPORT void add_node(JNIEnv *env, MeshGraph *graph, MeshNode *node);

/**
 * Removes a node from the mesh graph based on its IP address.
 * @param graph - The mesh graph from which the node will be removed.
 * @param ip - The IP address of the node to remove.
 */
JNIEXPORT void remove_node(JNIEnv *env, MeshGraph *graph, const char *ip);

/**
 * Establishes a direct connection between two mesh nodes.
 * @param a - First node.
 * @param b - Second node.
 */
JNIEXPORT void connect_nodes(JNIEnv *env, MeshNode *a, MeshNode *b);

/**
 * Disconnects two previously connected mesh nodes.
 * @param a - First node.
 * @param b - Second node.
 */
JNIEXPORT void disconnect_nodes(JNIEnv *env, MeshNode *a, MeshNode *b);

struct __packet;

typedef struct __packet Packet;
/**
 * Sends a message from one node to another.
 * @param src - The sender node.
 * @param dest - The recipient node.
 * @param msg - The message to be sent.
 * @param type - The packet type (e.g., data, handshake, ACK).
 * @return true if the message was successfully sent, false otherwise.
 */
JNIEXPORT jboolean send_message(
    JNIEnv *env, MeshNode *src, MeshNode *dest, jbyte *msg, jbyte type);

/**
 * Processes an incoming message for a node.
 * @param node - The node receiving the message.
 * @param pkt - The received packet.
 */
JNIEXPORT void receive_message(JNIEnv *env, MeshNode *node, Packet *pkt);

/**
 * Sends a file from one node to another.
 * @param src - The sender node.
 * @param dest - The recipient node.
 * @param filepath - Path to the file to be sent.
 * @param status - status
 * @return true if the file transfer started successfully, false otherwise.
 */
JNIEXPORT void send_file(JNIEnv *env,
			 MeshNode *src,
			 MeshNode *dest,
			 const char *filepath,
			 jboolean *status);

/**
 * Handles receiving a file at a given node.
 * @param node - The node receiving the file.
 * @param pkt - The received packet containing file data.
 * @return true if the file was successfully received, false otherwise.
 */
JNIEXPORT jboolean receive_file(JNIEnv *env, MeshNode *node, Packet *pkt);

/**
 * The main daemon responsible for managing the mesh network.
 * This function runs in the background to handle packet routing,
 * node discovery, and data transmission.
 * @param arg - Argument for the daemon (typically a MeshGraph pointer).
 * @return A pointer to the result (if needed).
 */
JNIEXPORT void *mesh_daemon(JNIEnv *env, void *arg);
#endif

// ! threading

JNIEXPORT void add_connection(JNIEnv *env, jbyte *, jbyte *, jint);
JNIEXPORT void *send_packet_thread(JNIEnv *env, void *arg);
JNIEXPORT void *receive_packet_thread(JNIEnv *env, void *arg);

#endif  // BINDINGS