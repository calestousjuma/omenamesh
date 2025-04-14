
#include "common.h"

/*! better error handling*/

void *xrealloc(void *oldptr, size_t new_size) {
	void *ptr;
	if ((ptr = realloc(oldptr, new_size)) == 0)
		goto defer;
	return ptr;
defer:
	perror("realloc");
	return ptr;
}

void *xcalloc(size_t members, size_t block) {
	void *ptr;
	if ((ptr = calloc(members, block)) == 0)
		goto defer;
	return ptr;
defer:
	perror("calloc");
	return ptr;
}

void *xmalloc(size_t block) {
	void *ptr;
	if ((ptr = malloc(block)) == 0)
		goto defer;
	return ptr;
defer:
	perror("malloc");
	return ptr;
}