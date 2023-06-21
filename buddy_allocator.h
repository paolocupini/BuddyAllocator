#pragma once
#include "bit_map.h"

typedef struct {
    
    int num_levels;
    char *buffer; //memory area to be managed
    int buffer_size;
    int min_bucket_size; // the minimum page of RAM that can be returned
    BitMap bitmap;
} BuddyAllocator;

int BuddyAllocator_init(
    BuddyAllocator* alloc,
    int num_levels,
    char *buffer,
    int buffer_size,
    int min_bucket_size;

);

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);
