#include "common.h"



int main(void) {
	char *me[5] = {
		"hello", "me", "us", "hello", "me",
	};

	for (int q = 0; q < 5; q++)
		printf("%d\n", hash_insert_virtual_ips(me[q]));
}