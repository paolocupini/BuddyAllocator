#include "buddy_allocator.h"
#include <stdio.h>


#define BUFFER_SIZE 1024
#define BUDDY_LEVELS 7
#define MEMORY_SIZE 128
#define MIN_BUCKET_SIZE (BUFFER_SIZE>>(BUDDY_LEVELS))

char memory[MEMORY_SIZE];
char buffer[BUFFER_SIZE];

BuddyAllocator alloc;
int main(int argc, char** argv) {
    int buf_size=BUFFER_SIZE;
    int min_bucket=MIN_BUCKET_SIZE;
    int levels=BUDDY_LEVELS;

    //! TO DO: check size of the request and handle two different scenarios

    printf("init...\n");

    int buddy_init= BuddyAllocator_init(&alloc,levels,buffer,BUFFER_SIZE,memory,min_bucket);

    printf("done..\n")

    if(init==0) {
        printf("error initializing buddy_allocator\n");
        return 0;
    }

    //! TO DO: try some allocation 

    return 0;

}