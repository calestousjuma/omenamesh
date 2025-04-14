

#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "common.h"

#define PORT 9999
#define MSG "hello from device"
#define BUF_SIZE 1024
#define INTERVAL 5000  // ms

int main() {
	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock < 0)
		return 0;

	char ip[ADDRESS_LEN];

	bool broadcast = true;

	setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast,
		   sizeof(broadcast));

	BROADCAST_IP(ip);

	struct sockaddr_in bcast = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT),
		.sin_addr.s_addr = inet_addr(ip),
	};

	struct sockaddr_in local = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT),
		.sin_addr.s_addr = INADDR_ANY,
	};
	if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
		return 0;

	struct pollfd fds[1] = {
		{
		    .fd = sock,
		    .events = POLLIN,
		},
	};

	char buf[HASH_SIZE / 4];
	struct sockaddr_in peer;
	socklen_t len = sizeof(peer);
	uint64_t last_sent = 0;

	do {
		uint64_t now = time(NULL) * 1000;
		if (now - last_sent >= INTERVAL) {
			sendto(sock, MSG, strlen(MSG), 0,
			       (struct sockaddr *)&bcast, sizeof(bcast));
			last_sent = now;
		}

		int ret = poll(fds, 1, 100);  // 100ms wait
		if (ret > 0 && (fds[0].revents & POLLIN)) {
			ssize_t n = recvfrom(sock, buf, BUF_SIZE - 1, 0,
					     (struct sockaddr *)&peer, &len);
			if (n > 0) {
				buf[n] = 0;
				printf("Got: %s from %s\n", buf,
				       inet_ntoa(peer.sin_addr));
			}
		}

		sleep(1);
	} while (0);

	return 0;
}
