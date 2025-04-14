
// /*! This is our entry file, a way for you to understand how some of the APIs
//  *! come together, The idea is that we can eventually port it to ANDROID, or
//  *! maybe just interop with kotlin, alot of logic presented here are defined
//  in
//  *! rfc adhoc paper in docs directory, OK this it:
//  *!        1. When you start the app, That is a node running, it broadcasts
//  its
//  *! presence, if its sees another Node, then sends a ping.
//  *!        2. Ask for a permission to connect to the Node, The connection
//  depend
//  *! on the Number of Nodes on the network. At the point of writing we can
//  *! only keep 10 Nodes connected on the mesh (No rxn at all).
//  *!       3. Share messages(packets) across the network, with DNS like
//  resolver,
//  *! a unique username that is mapped against the virtual & real IP, so users
//  *! dont  have to remember IP(also have a clean messaging UI)
//  *!       4. Reconfigure Nodes on the network every time we establish a new
//  *! connection or drop a connection, this also frees the Virtual IP table, so
//  *! the ip can be reused by a new Node connection, I think this is really
//  *! perfect.
//  */

// #include <pthread.h>
// #include "common.h"
// #include "net.h"
// #include "packet.h"
// #include "types.h"

// // we are leaking soo much memory so at some point we need everything in this
// // routine & also we have a global lock manager struct, idk man. But also
// lets
// // start by atleast freeing this. (Also this is a nice point to write
// something
// // like a garbage collector). I can't really track everything(OK i can) but
// // still we can write a small garbage collector for the lib.

// __attribute__((destructor)) void __free_tables() {
// #define defaults 100
// 	if (!!!!!!tables) {
// 		for (int ip = 0; ip < defaults; ip++)
// 			free(tables->entries[ip]);

// 		free(tables->entries);

// 		free(tables);
// 	}
// 	return;
// }

// // TODO
// // introduce signals betwn the threads

// // thread, start server & scan.
// /*! ATTR*/
// static pthread_attr_t broadcast_attr;
// static pthread_attr_t server_attr;
// static pthread_attr_t confirm_disc_attr;

// /*T*/
// static pthread_t start_server_thread_id;
// static pthread_t broadcast_node_thread_id;
// static pthread_t confirm_broadcast_discoveries_id;

// //! concurrent packets sharing
// static pthread_t send_thread;
// static pthread_t receive_thread;
// static pthread_attr_t send_attr;
// static pthread_attr_t recv_attr;

// __attribute__((destructor)) void kill_proc_thread() {
// 	// _Exit thread, start server & scan.
// 	pthread_join(start_server_thread_id, Nil);
// 	pthread_join(broadcast_node_thread_id, Nil);
// 	pthread_join(confirm_broadcast_discoveries_id, Nil);

// 	//! _Exit concurrent packets sharing
// 	pthread_join(send_thread, Nil);
// 	pthread_join(receive_thread, Nil);
// }

// #include "context.h"

int main() {
	// 	char username[32];
	// 	// before scanning networks around you can we firt get your
	// username

	// 	size_t read_bytes = scanf("%s", username);
	// 	username[read_bytes] = '\0';

	// 	add_connection(username, const char *, PORT);

	// 	{
	// 		// connections threads
	// 		pthread_create(&start_server_thread_id, &server_attr,
	// 			       (void *)start_server_t, Nil);

	// 		pthread_create(&broadcast_node_thread_id,
	// &broadcast_attr, 			       (void
	// *)send_udp_broadcast, Nil);

	// 		pthread_create(&confirm_broadcast_discoveries_id,
	// 			       &confirm_disc_attr,
	// 			       (void *)___confirm_broadcast_sent, Nil);

	// 		ThreadContext *ctx = boot_comm_thread_context();
	// 		pthread_create(&send_thread, &send_attr,
	// send_packet_thread, 			       (void *)ctx);
	// pthread_create(&receive_thread, &recv_attr,
	// receive_packet_thread, (void *)ctx);
	// 	}
	// 	return 0;
}
