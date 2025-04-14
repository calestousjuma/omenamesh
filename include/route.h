#ifndef ROUTE_H
#define ROUTE_H

#include "net.h"
#include "packet.h"

struct __routes {
	char *ip;
	i32__CJLF socket_fd;
} Routes[MAX_NODES];

/*! MACRO THIS*/
static inline void initialize_route_table(struct __routes *route) {
	route->ip = Nil;
	route->socket_fd = 0;
}

// extern struct __routes routes;

// extern i32__CJLF get_local_ip(char *buffer, i64__CJLF len);
// extern void add_route(const char *ip, int socket_fd);

#endif /*! ROUTE_H*/
