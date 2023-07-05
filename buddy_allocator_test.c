#include "buddy_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include "mmap_alloc.h"
#include "math.h"

#define BUFFER_SIZE 1024
#define MEMORY_SIZE 256
#define BUDDY_LEVELS 5
#define MIN_BUCKET_SIZE (BUFFER_SIZE >>(BUDDY_LEVELS))
char memory[MEMORY_SIZE];

BuddyAllocator alloc;

int main(int argc, char **argv)
{
    int buf_size=BUFFER_SIZE;
    int levels=BUDDY_LEVELS;
    int min_bucket=buf_size>>(levels);
    char buffer[BUFFER_SIZE];
    


    int size;
    if (argc > 1)
    {
        size = atoi(argv[1]);
        if (size < -2 || size==0)
        {
            printf("\nERROR\nINSERISCI SIZE BLOCCO DA ALLOCARE > 0 \n INSERISCI -1 PER ESEGUIRE TEST BUDDY_ALLOC\n INSERISCI -2 PER ESEGUIRE TEST MMAP_ALLOC\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("\nERROR\n INSERISCI SIZE BLOCCO DA ALLOCARE > 0 \n INSERISCI -1 PER ESEGUIRE TEST BUDDY_ALLOC\n INSERISCI -2 PER ESEGUIRE TEST MMAP_ALLOC\n");
        exit(EXIT_FAILURE);
    }
    if (size==-1) {

        printf("\nTEST BUDDY_ALLOC\n");
        return 0;
    }
    if (size==-2){
        
        printf("TEST MMAP_ALLOC");
        return 0;

    }
    else if (size + 4 < BUFFER_SIZE / 4)
    {

        printf("init...\n");
        int buddy_init = BuddyAllocator_init(
            &alloc,
            levels,
            buffer,      // buf alloc
            buf_size,    // dim buf
            memory,      // buf bitmap
            MEMORY_SIZE, // dim bitmap
            min_bucket);

        printf("done..\n");

        if (buddy_init == 0)
        {
            printf("error initializing buddy_allocator\n");
            return 0;
        }
        void *p1 = BuddyAllocator_malloc(&alloc, size);
        BuddyAllocator_free(&alloc, p1);
    }

    else
    {
        void *p1 = mmap_malloc(size);
        mmap_free(p1);
    }

    return 0;
}