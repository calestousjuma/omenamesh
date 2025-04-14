#ifndef LOCK_H
#define LOCK_H

/*
 * simple lightweight lock implementation
 * ----------------------------------------------------------
 * This lock system provides a simple, atomic-based locking mechanism that:
 *
 * 1. **Limits Concurrent Locks**:
 *    - Only a maximum of 10 locks can be acquired at a time.
 *    - If all locks are in use, new requests are added to a queue.
 *
 * 2. **Queue-Based Lock Handling**:
 *    - When a lock is released, pending requests in the queue are processed.
 *    - Requests are executed in FIFO order.
 *
 * 3. **Atomic and Non-Blocking**:
 *    - Uses atomic operations (`atomic_compare_exchange_strong`) to prevent
 * race conditions.
 *    - Avoids busy-waiting by using a request queue instead of aggressive
 * polling.
 *
 * 4. **Callback Mechanism**:
 *    - Lock requests specify a callback function that is executed when the lock
 * is acquired.
 *    - This allows asynchronous handling of queued requests.
 *
 * 5. **Locking Behavior**:
 *    - If a lock is available, it is immediately granted.
 *    - If no lock is available, the request is queued and processed later.
 *    - When a lock is released, the next request in the queue is granted
 * access.
 *
 * 6. **Potential Improvements**:
 *    - Implement a timeout feature for queued requests to avoid indefinite
 * waiting.
 *    - Convert to a fairer scheduling policy, such as priority-based lock
 * handling.
 *    - Optimize performance for multi-core systems with cache-aware techniques.
 *
 * This lock system is designed for use in scenarios where `pthread` is not
 * desirable, ensuring thread/process-safe access to shared resources while
 * maintaining efficiency.
 */

#include <stdatomic.h>
#include "types.h"

#define MAX_LOCKS 10

typedef struct {
	atomic_bool locked;
} Lock;

/* Queue node for holding lock requests */
typedef struct LockRequestNode {
	/* The callback function for the request */
	__CJLF_GENERICS (*callback)(__CJLF_GENERICS *arg);
	/*  Arguments for the callback function */
	__CJLF_GENERICS *arg;
	/*Pointer to the next request in the queue */
	struct LockRequestNode *next;
} LockRequestNode;

/* Lock queue structure to hold waiting requests */
typedef struct {
	LockRequestNode *front;
	LockRequestNode *rear;
	/*Current number of requests in the queue */
	atomic_int size;
} LockRequestQueue;

/* Lock manager structure to manage locks and request queue */
typedef struct {
	Lock locks[MAX_LOCKS];
	LockRequestQueue request_queue;
	i8__CJLF no_acquired_locks;
} LockManager;

extern LockManager *global_lock_manager;

/*make the lock manager global, uhhhhh? Look.*/
BEFORE OMENAMESH_API __CJLF_GENERICS lock_init();

__attribute__((__destructor__)) __CJLF_GENERICS destroy_lock();

OMENAMESH_API __CJLF_GENERICS
acquire_lock(LockManager *manager,
	     __CJLF_GENERICS (*callback)(__CJLF_GENERICS *arg),
	     __CJLF_GENERICS *arg);
OMENAMESH_API __CJLF_GENERICS release_lock(LockManager *manager,
					   i32__CJLF lock_id);
OMENAMESH_API bool is_locked(LockManager *manager, i32__CJLF lock_id);

OMENAMESH_API __CJLF_GENERICS
queue_lock_request(LockManager *manager,
		   __CJLF_GENERICS (*callback)(__CJLF_GENERICS *arg),
		   __CJLF_GENERICS *arg);

OMENAMESH_API LockRequestNode *dequeue_lock_request(LockManager *manager);

__CJLF_GENERICS process_lock_queue(LockManager *manager);

#endif /*! LOCK_H */
