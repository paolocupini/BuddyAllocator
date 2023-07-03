#pragma once
#include "bit_map.h"
#define MAX_LEVELS 16


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
    char *bit_buf,
    int bit_buf_size,
    int min_bucket_size
);

//allocates memory
void* BuddyAllocator_malloc(BuddyAllocator* alloc, int size);

//releases allocated memory
void BuddyAllocator_free(BuddyAllocator* alloc, void* mem);

void Set_parent(BitMap *bitmap, int status, int numBit);

void Set_child(BitMap *bitmap, int status, int numBit);