
#include "buddy_allocator.h"

void* mmap_malloc(uint64_t size);
void mmap_free(void* p);