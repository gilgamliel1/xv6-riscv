#include "kernel/types.h"
#include "user/user.h"

int
main() {
  printf("Initial memsize: %d bytes\n", memsize());

  // Allocate 20,000 bytes (20 KB)
  void *ptr = malloc(20000);

  printf("After malloc(20k): %d bytes\n", memsize());

  // Free the allocated memory
  free(ptr);

  printf("After free(): %d bytes\n", memsize());

  exit(0 , "");
}
