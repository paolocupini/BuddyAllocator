#pragma once
#include "bitmap.h"

typedef struct {
    int num_levels;
    char *buffer; //memory area to be managed
    int buffer_size;
    int min_bucket_size; // the minimum page of RAM that can be returned

} BuddyAllocator;

int BuddyAllocator_init(
    BuddyAllocator* alloc,
    int num_levels,
    char *buffer,
    int buffer_size,
    int min_bucket_size;

);
