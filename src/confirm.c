
#include "packet.h"

void ___confirm_broadcast_sent() {
	int sockfd;
	struct sockaddr_in addr, sender;
	socklen_t sender_len = sizeof(sender);
#define BUF_SIZE 1024
	char buffer[BUF_SIZE];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		goto defer;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		goto defer;

	printf("Listening for UDP broadcasts on port %d...\n", PORT);

	for (;;) {
		ssize_t len = recvfrom(sockfd, buffer, BUF_SIZE - 1, 0,
				       (struct sockaddr *)&sender, &sender_len);
		if (len < 0) {
			perror("recvfrom");
			continue;
		}

		buffer[len] = '\0'; /*! clean string*/
		printf("Received from %s:%d -> %s\n",
		       inet_ntoa(sender.sin_addr), ntohs(sender.sin_port),
		       buffer);
	}

	close(sockfd);
	return;

defer:
	perror("confirm_broadcast");
	if (sockfd > 0)
		close(sockfd);
	_Exit(EXIT_FAILURE);
}