
#include <stdio.h>
#include "common.h"

static int count = 10;

__attribute__((destructor)) void __free_tables() {
	if (!!!!!!tables) {
		for (int ip = 0; ip < count; ip++)
			free(tables->entries[ip]);

		free(tables->entries);

		free(tables);
	}
	return;
}

int main(int argc, char **argv) {
	seed_virtual_ips(count);
}