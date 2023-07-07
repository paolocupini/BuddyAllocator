#include "buddy_allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include "mmap_alloc.h"
#include "math.h"
#include <unistd.h>


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
        int buddy_init = BuddyAllocator_init(
            &alloc,
            levels,
            buffer,      // buf alloc
            buf_size,    // dim buf
            memory,      // buf bitmap
            MEMORY_SIZE, // dim bitmap
            min_bucket);


        if (buddy_init == 0)
        {
            printf("error initializing buddy_allocator\n");
            return 0;
        }

        printf("\n\033[0;35mTESTING BUDDY ALLOCATION\033[0m\n\n");

        
        void *a1=BuddyAllocator_malloc(&alloc,124);

        void *a2=BuddyAllocator_malloc(&alloc,124);
        void *a3=BuddyAllocator_malloc(&alloc,252);
        void *a5=BuddyAllocator_malloc(&alloc,508);
        BuddyAllocator_free(&alloc,a5);
        BuddyAllocator_free(&alloc,a3);
        BuddyAllocator_free(&alloc,a2);
        BuddyAllocator_free(&alloc,a1);
        void *a4=BuddyAllocator_malloc(&alloc,1020);



        //!ERRORI MALLOC
        sleep(2);
        printf("\n\033[0;35mTENTO DI ALLOCARE UNA VOLTA TERMINATI I BLOCCHI\033[0m\n\n");

        void *a6=BuddyAllocator_malloc(&alloc,10);
        BuddyAllocator_free(&alloc,a4);
        sleep(2);
        printf("\n\033[0;35mTENTO DI ALLOCARE PIU MEMORIA DI QUELLA DISPONIBILE\033[0m\n\n");

        void *a7=BuddyAllocator_malloc(&alloc,30000);

        //!ERRORI FREE
        sleep(2);
        printf("\n\033[0;35mTENTO DI LIBERARE BLOCCO NON ALLOCATO\033[0m\n\n");
        BuddyAllocator_free(&alloc, a6);

        printf("\n\033[0;35mTENTO DI LIBERARE BLOCCO GIA LIBERATO\033[0m\n\n");
        BuddyAllocator_free(&alloc,a4);



        return 0;
    }
    if (size==-2){
        
        printf("\n\033[0;35mTESTING MMAP ALLOCATION\033[0m\n\n");
        
        void *p1 = mmap_malloc(1000);
        void *p2 = mmap_malloc(100000);
        printf("\n");
        mmap_free(p1);
        mmap_free(p2);
        //!TO DO : TEST ERRORI MMAP FREE
        printf("\n\033[0;35mFACCIO FALLIRE\033[0m\n\n");
        

        return 0;

    }
    //!SMALL REQ
    else if (size + 4 <= BUFFER_SIZE)
    {
        printf("\n\033[0;35mSMALL REQUEST\033[0m\n\n");
        int buddy_init = BuddyAllocator_init(
            &alloc,
            levels,
            buffer,      // buf alloc
            buf_size,    // dim buf
            memory,      // buf bitmap
            MEMORY_SIZE, // dim bitmap
            min_bucket);


        if (buddy_init == 0)
        {
            printf("error initializing buddy_allocator\n");
            return 0;
        }
        void *p1 = BuddyAllocator_malloc(&alloc, size);
        sleep(2);
        BuddyAllocator_free(&alloc, p1);
    }
    //! LARGE REQ

    else
    {
       printf("\n\033[0;35mLARGE REQUEST\033[0m\n\n");

        void *p1 = mmap_malloc(size);
        mmap_free(p1);
    }

    return 0;
}