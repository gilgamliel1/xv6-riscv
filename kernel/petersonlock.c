// Mutual exclusion spin locks.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "petersonlock.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"

#define MAX_PETERSON_LOCKS 15
struct petersonlock peterson_locks[MAX_PETERSON_LOCKS];

// --- Internal helper: enter critical section using Peterson's algorithm
static void
enter_region(int lock_id, int role) {
  peterson_locks[lock_id].interested[role] = 1;
  peterson_locks[lock_id].barrier = role;
  while (peterson_locks[lock_id].barrier == role &&
         peterson_locks[lock_id].interested[1 - role]) {
    yield(); // Assignment requires CPU yield instead of busy-waiting
  }
}

// --- Internal helper: exit critical section
static void
leave_region(int lock_id, int role) {
  peterson_locks[lock_id].interested[role] = 0;
}

// --- API: Create a new Peterson lock
int
peterson_create(void) {
  for (int i = 0; i < MAX_PETERSON_LOCKS; i++) {
    if (!peterson_locks[i].active) {
      peterson_locks[i].interested[0] = 0;
      peterson_locks[i].interested[1] = 0;
      peterson_locks[i].barrier = 0;
      peterson_locks[i].active = 1;
      peterson_locks[i].cpu = 0;
      return i;  // Return index as lock ID
    }
  }
  return -1;  // All locks in use
}

// --- API: Acquire the lock
int 
peterson_acquire(int lock_id, int role) {
  if (lock_id < 0 || lock_id >= MAX_PETERSON_LOCKS || 
      peterson_locks[lock_id].active == 0 || 
      (role != 0 && role != 1)) {
    return -1; // Invalid lock ID or role
  }
  enter_region(lock_id, role);
  return 0;    
}

// --- API: Release the lock
int 
peterson_release(int lock_id, int role) {
  if (lock_id < 0 || lock_id >= MAX_PETERSON_LOCKS || 
      peterson_locks[lock_id].active == 0 || 
      (role != 0 && role != 1)) {
    return -1; // Invalid lock ID or role
  } 
  leave_region(lock_id, role);
  return 0;
}

// --- API: Destroy the lock
int 
peterson_destroy(int lock_id) {
  if (lock_id < 0 || lock_id >= MAX_PETERSON_LOCKS || 
      peterson_locks[lock_id].active == 0) {
    return -1; // Invalid lock ID
  } 
  peterson_locks[lock_id].active = 0;
  return 0; 
}
