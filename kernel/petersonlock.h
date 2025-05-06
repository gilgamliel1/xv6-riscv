#include "types.h"
// Mutual exclusion lock.
struct petersonlock {
  uint barrier;       // Is the lock held?
  uint interested[2];     // array of 2 flags for each thread

  // For debugging:
  uint active;        
  struct cpu *cpu;   // The cpu holding the lock.
};

