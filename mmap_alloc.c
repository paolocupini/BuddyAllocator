#include "mmap_alloc.h"
#include <math.h>
#include <sys/mman.h>
#include <assert.h>
#include <stdio.h>

   #include <sys/types.h>
   #include <sys/stat.h>
  #include <fcntl.h>


void* mmap_malloc(uint64_t size){
    // 16 bytes overhead 
    /* PORTABLE VERSION
    int fd = open("/dev/zero", O_RDWR);
    map = mmap(0, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
    */
    //Anonymous mappings are better for large allocations because they do not pin down the heap with chunks of memory, which would make fragmentation more likely. -> thx stackoverflow
    char* map=(char*) mmap(0, size+16,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
   
    if(map == MAP_FAILED){
      printf("\n MAP FAIL \n");
      return 0;
    };
   
    printf("\033[0;34mALLOCAZIONE BLOCCO DA:%ld BYTES\033[0m",size+16);

    //salvo puntatore per unmap
    char** pointer=(char**)map;
    *pointer=map;
    
    uint64_t* backup=(uint64_t*)(map+8);
    *backup=size+16;

    printf("\033[32;1m...MAPPED CORRECTLY...\033[0m");

    printf( "\033[0;33m:%p\033[0m\n",map+16);


    return map+16;


}
void mmap_free(void* p) {
    //free(NULL) 
   if(!p){
    return;
   }
   
    printf("\033[0;34mLIBERO BLOCCO INDIRIZZO: \033[0m");
    printf( "\033[0;33m:%p\033[0m",p);
    
        char* p1=(char*)p-16;
        char** saved=(char**)p1;

      //confronto saved pointer 
        if(*saved!=p1){
    printf("\n\033[0;31m*************************ERROR: WRONG POINTER************************\033[0m\n");
          return;
        }

        //abbiamo il puntatore giusto possiamo deallocare
        uint64_t size=*((uint64_t*)(p1+8));
        int res=munmap(p1,size);
        assert(!res && "error munmap");

        printf("\033[32;1m...UNMAPPED CORECTLY...\033[0m\n");



        
    
}