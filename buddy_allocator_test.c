#include "buddy_allocator.h"
#include <stdio.h>


#define BUFFER_SIZE 260
#define BUDDY_LEVELS 5
#define MEMORY_SIZE 128
#define MIN_BUCKET_SIZE (BUFFER_SIZE>>(BUDDY_LEVELS))

char memory[MEMORY_SIZE];

BuddyAllocator alloc;

int main(int argc, char** argv) {
    int buf_size=BUFFER_SIZE;
    int levels=BUDDY_LEVELS;
    int min_bucket=buf_size>>(levels);
    char buffer[BUFFER_SIZE];
    
    //! TO DO: check size of the request and handle two different scenarios
    //!TEST DA IMPLEMENTARE: 
    //! 1. small request (< 1/4 page size) -> buddy allocator
    //! 2. large request (> 1/4 page size) -> mmap
    //! 3. errori: blocco troppo grande, memoria terminata ecc
    
    printf("init...\n");

    int buddy_init= BuddyAllocator_init(
        &alloc,
        levels,
        buffer,     //buf alloc
        buf_size,   //dim buf
        memory,     //buf bitmap
        MEMORY_SIZE,// dim bitmap
        min_bucket);

    printf("done..\n");

    if(buddy_init==0) {
        printf("error initializing buddy_allocator\n");
        return 0;
    }
      void *p1 = BuddyAllocator_malloc(&alloc, 4);
      BuddyAllocator_free(&alloc,p1);


    //! TO DO: try some allocation 

    return 0;

}