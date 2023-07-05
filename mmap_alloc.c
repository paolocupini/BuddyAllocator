#include "mmap_alloc.h"
#include <math.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdio.h>


void* mmap_malloc(uint64_t size){
    // 16 bytes overhead 
    char* map=(char*) mmap(0, size+16,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
    if(map == MAP_FAILED){
      printf("\n MAP FAIL \n");
      return 0;
    };
    printf("\nMAPPED CORRECTLY\n");

    //salvo puntatore per unmap
    char** pointer=(char**)map;
    *pointer=map;
    
    uint64_t* backup=(uint64_t*)(map+8);
    *backup=size+16;

    printf("\nALLOCAZIONE BLOCCO DA %ld BYTES\n",size+16);

    return map+16;


}
void mmap_free(void* p) {
    //free(NULL) 
    if (!p)
      return;
    
        printf("\n LIBERO BLOCCO IN POSIZIONE:  %p\n",p);
        char* p1=(char*)p-16;
        char** saved=(char**)p1;

      //confronto saved pointer 
        if(*saved!=p1){
          printf("\nWRONG POINTER\n");
          return;
        }
    
        uint64_t size=*((uint64_t*)(p1+8));
        int res=munmap(p1,size);
        assert(!res && "error munmap");

        printf("\n UNMAPPED CORRECTLY \n");

        
    
}