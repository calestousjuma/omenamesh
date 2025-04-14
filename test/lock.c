#include "lock.h"
#include <pthread.h>

void *get_sum(void *arg) {
	long sum = *(long *)arg;

	for (long i = 1; i <= 1e6; ++i) {
		acquire_lock(global_lock_manager, 0, 0);
		sum += i;
		release_lock(global_lock_manager,
			     global_lock_manager->no_acquired_locks);
	}

	return arg;
}

int main() {
	long sum = 0;
	// pthread_t thread1;
	// pthread_attr_t attr;

	// pthread_attr_init(&attr);
	// pthread_create(&thread1, &attr, get_sum, &sum);

	return 0;
}
