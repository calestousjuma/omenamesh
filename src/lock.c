#include "lock.h"
#include "common.h"

LockManager *global_lock_manager = 0;

BEFORE __CJLF_GENERICS lock_init() {
	global_lock_manager = xmalloc(sizeof(*global_lock_manager));
	for (int i = 0; i < MAX_LOCKS; i++)
		atomic_store(&global_lock_manager->locks[i].locked, false);

	global_lock_manager->request_queue.front = Nil;
	global_lock_manager->request_queue.rear = Nil;
	global_lock_manager->no_acquired_locks = Nil;

	atomic_store(&global_lock_manager->request_queue.size, Nil);
}

#include <assert.h>
/* Acquire the lock with a callback and argument for queued requests */
__CJLF_GENERICS acquire_lock(LockManager *manager,
			     __CJLF_GENERICS (*callback)(__CJLF_GENERICS *arg),
			     __CJLF_GENERICS *arg) {
	if (manager->no_acquired_locks >= MAX_LOCKS)
		queue_lock_request(manager, callback, arg);

	bool acquired = false; /*atomic?*/
	/*spin & try to acquire a lock*/
	for (int i = 0; i < MAX_LOCKS; i++) {
		bool expected = false;
		if (!atomic_load(&manager->locks[i].locked) &&
		    atomic_compare_exchange_strong(&manager->locks[i].locked,
						   &expected, true)) {
			acquired = true;
			manager->no_acquired_locks++;
			return;
		}
	}

	/*If no lock is available, queue the request */
	if (!acquired)
		queue_lock_request(manager, callback, arg);
}

/* Release a specific lock and process any pending requests in the queue */
__CJLF_GENERICS release_lock(LockManager *manager, int lock_id) {
	if (lock_id >= 0 && lock_id < MAX_LOCKS) {
		atomic_store(&manager->locks[lock_id].locked, false);
		process_lock_queue(manager);
	}
}

/* Check if a lock is currently held */
bool is_locked(LockManager *manager, int lock_id) {
	/*assert with no_acquired_locks - 1*/
	if (lock_id >= 0 && lock_id < MAX_LOCKS)
		return atomic_load(&manager->locks[lock_id].locked);
	return false;
}

__CJLF_GENERICS queue_lock_request(
    LockManager *manager,
    __CJLF_GENERICS (*callback)(__CJLF_GENERICS *arg),
    __CJLF_GENERICS *arg) {
	LockRequestNode *new_request =
	    (LockRequestNode *)xmalloc(sizeof(LockRequestNode));
	new_request->callback = callback;
	new_request->arg = arg;
	new_request->next = Nil;

	/* Add the new request to the end of the queue */
	if (atomic_load(&manager->request_queue.size) == 0)
		manager->request_queue.front = new_request;
	else
		manager->request_queue.rear->next = new_request;

	manager->request_queue.rear = new_request;
	atomic_fetch_add(&manager->request_queue.size, 1);
}

/*Dequeue a lock request from the queue */
LockRequestNode *dequeue_lock_request(LockManager *manager) {
	if (atomic_load(&manager->request_queue.size) == 0)
		return Nil;

	LockRequestNode *front_request = manager->request_queue.front;
	manager->request_queue.front = front_request->next;
	atomic_fetch_sub(&manager->request_queue.size, 1);

	return front_request;
}

__CJLF_GENERICS process_lock_queue(LockManager *manager) {
	if (atomic_load(&manager->request_queue.size) > 0) {
		LockRequestNode *request = dequeue_lock_request(manager);
		if (request != Nil) {
			request->callback(request->arg);
			free(request);
		}
	}
}
