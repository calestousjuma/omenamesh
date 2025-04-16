
/*! LICENCED UNDER CJLF 2025(c)
 *!
 *! This file has hashmap for ip(real) value ip(virtual) key, the reason for the
 *! hashmap is because the ip are generated in random fashion & yeah the map
 *! helps to keep everything unique so we dont have ip conflict later in the
 *! network, OK that is not actually the reason, the thing is different networks
 *! have almost similar IP addresses, the idea is we map actual real IP
 *! behind a virtual IP, when you share something across the network, the
 *! virtual then maps to the real IP. IDK if that makes sense but that is
 *! the whole idea behind our virtual IP addressing.
 *!
 *! OK this will make sense, y'know how we have virtual memory which is
 *! translated to physical memory, blah blah TLB MMU PDE all that?. Yeah thats
 *! it. You can do anything with the virtual memory (dereference, map files
 *! ...etc except its not what the computer will see, Just an illusion for your
 *! user process). If u got this far in the comment you're beautiful & deserve
 *! the world :)
 */

#include "common.h"

#define STRING (ADDRESS_LEN * 2)

struct __table *tables;

__CJLF_GENERICS initialize_table(i64__CJLF cap, i64__CJLF block) {
	tables = xmalloc(sizeof(*tables));
	tables->entries = xcalloc(cap, block);
	/*begin, FLAGS __UNSUED*/
	tables->cursor = _UNUSED;
	tables->flags = xcalloc(cap, sizeof(VirtualIpState));
	return;
}

__CJLF_GENERICS seed_virtual_ips() {
#define DEFAULT 100
	initialize_table(DEFAULT, STRING);
	/*! rxn for hashmap*/
	for (int elements = 0; elements < DEFAULT; elements++) {
		tables->ip = xmalloc(STRING);
		snprintf(tables->ip, STRING, "%d:%d:%d:%d", rand() % 255,
			 rand() % 255, rand() % 255, rand() % 255);
		if (hash_insert_virtual_ips(tables->ip)) {
			tables->entries[elements] = tables->ip;
			tables->flags[elements] = _UNUSED;
		} else {
			/*! 30 for 30, sza*/
			elements--; /*one more time to make up for that*/
		}
	}

	return;
}

char *get_next_virtual_ip() {
#define DEFAULTS 100
	while (tables->cursor < DEFAULTS) {
		if (tables->flags[tables->cursor] == _UNUSED) {
			tables->flags[tables->cursor] = _USED;
			return tables->entries[tables->cursor++];
		}
		tables->cursor++;
	}
	return Nil;
}

typedef struct __entries_t {
	char *ip;
	struct __entries_t *next;
} pde_t;

pde_t *hashmap[HASH_SIZE];

unsigned int hash_virtual_ip(const char *ip) {
	unsigned hash = 5381; /*prime*/
	while (*ip)
		hash = ((hash << 5) + hash) + *ip++;

	return hash % HASH_SIZE;
}

bool hash_insert_virtual_ips(char *ip) {
	bool status = false;
	unsigned index = hash_virtual_ip(ip);

	pde_t *cur = hashmap[index];
	for (pde_t *k = cur; k; k = k->next)  // BUG
		if (strcmp(k->ip, ip) == 0)
			return status;

	status = true;
	pde_t *new = xmalloc(sizeof *new);
	new->ip = ip;
	new->next = hashmap[index];
	hashmap[index] = new;

	return status;
}

char *hash_lookup(const char *virtual_ip) {
	unsigned index = hash_virtual_ip(virtual_ip);
	pde_t *entry = hashmap[index];

	for (; entry; entry = entry->next)
		if (strcmp(entry->ip, virtual_ip) == 0)
			return entry->ip;

	return Nil;
}

#include "net.h"
// this is what is mapped against the virtual IP hash Table
void get_local_ip(char *buffer, i64__CJLF len, bool *status) {
	struct ifaddrs *ifaddr, *ifa;
	*status = false;

	if (getifaddrs(&ifaddr) == -1)
		return;

	for (ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL || ifa->ifa_name == NULL)
			continue;

		if (ifa->ifa_addr->sa_family != AF_INET)
			continue;

		/*! Skip loopback */
		if (strcmp(ifa->ifa_name, "lo") == 0)
			continue;

		if (getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
				buffer, len, NULL, 0, NI_NUMERICHOST) == 0) {
			if (strncmp(buffer, "127.", 4) == 0)
				continue;

			printf("[IP Discovery] Interface: %s, IP: %s\n",
			       ifa->ifa_name, buffer);
			*status = true;
			break;
		}
	}

	freeifaddrs(ifaddr);
}
