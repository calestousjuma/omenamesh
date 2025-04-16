
#include "common.h"
#include "net.h"

static i32__CJLF virtual_ip_table_index = 0;

__CJLF_GENERICS send_udp_broadcast() {
	i32__CJLF sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	if (sockfd < 0 && errno)
		// instead of all these gotos, we can move to non
		// local jumps (setjmp?) maybe in the future when
		// we refactor some of thi.
		goto defer;

	int broadcastEnable = 1;

	// make this a macro its heavily used
	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcastEnable,
		       sizeof(broadcastEnable)) < 0)
		goto defer;

	struct sockaddr_in broadcast_addr;

	broadcast_addr.sin_family = AF_INET;
	broadcast_addr.sin_port = htons(8000);
	broadcast_addr.sin_addr.s_addr = INADDR_BROADCAST;

	char real_ip[ADDRESS];
	bool status = false;
	get_local_ip(real_ip, sizeof(real_ip), &status);

	char message[128];

	//! TODO
	printf("OMENA_MESH_API_STARTED\n");

	snprintf(
	    message, sizeof(message),
	    "[virtual IP <%s:%d> mapped to real IP <%s:%d> ]",
	    tables->entries[virtual_ip_table_index++] /*CAN THIS OVERFLOW?*/,
	    PORT, real_ip, PORT);

	if (sendto(sockfd, message, strlen(message), 0,
		   (struct sockaddr *)&broadcast_addr,
		   sizeof(broadcast_addr)) < 0)
		goto defer;

	return;

defer:
	perror("Server Error");
	if (sockfd > 0)
		close(sockfd);
	/*! wait this goes to the kernel & doesnt clean up, No?
	 *! incase we decide to have __attribute__((destructor)) please come
	 *back to this*/
	_Exit(EXIT_SUCCESS);
}

// wake up, so we can broadcast. Multithread this
i8__CJLF start_server_t() {
	i32__CJLF server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
		goto defer;
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) <
	    0)
		goto defer;

	struct sockaddr_in server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8000);

	if (bind(server_fd, (struct sockaddr *)&server_addr,
		 sizeof(server_addr)) < 0)
		goto defer;

	if (listen(server_fd, 5) < 0)
		goto defer;

	set_nonblocking(server_fd);

	/*! Alot of questions here... */

	printf("[SERVER] Listening on port %d\n", PORT);
	return server_fd;

defer:
	perror("Init Server Error");
	if (server_fd > 0)
		close(server_fd);
	_Exit(EXIT_FAILURE);
}
