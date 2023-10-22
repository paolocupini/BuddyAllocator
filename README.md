# SO-project

Progetto Sistemi Operativi 2022/23, Prof. Giorgio Grisetti


## 1. Project's Purpose

The purpose of the project is to implement a memory allocator that does not use malloc. Specifically, memory allocation is managed in two ways: through a BuddyAllocator for small allocations and through MMAP. The BuddyAllocator divides memory recursively, with each recursion splitting a parent block into two "buddy" children until further recursion would make the block smaller than the required memory. We can visualize the result of this recursive division as a binary tree, and we can leverage the properties of the node relationships.

``` c
//level of node idx
int levelIdx(size_t idx){
  return (int)floor(log2(idx+1));
};
//index of the buddy with idx 
int buddyIdx(int idx){ 
  if (idx==0) return 0;
  else if (idx &0x1) return idx+1;
  return idx-1;
}

int parentIdx(int idx){
    return (idx-1)/2;
}
// ritorna indice primo nodo sul livello i , corrisponde a 2^i 
int firstIdx(int lev){
  return (1 << lev)-1; //2^level -1
}

// ritorna distanza dal primo elemento sul livello dato
int offsetIdx(int idx){
    return (idx-(firstIdx(levelIdx(idx))));
}

```


## 2. Implementation Choices and Explanation of Main Functions:

The project builds upon the code covered in the lecture, with the key difference being the use of a bitmap to store the state of the memory allocation. Each block is associated with a bit (1 or 0) to indicate whether it is allocated or free.

The BitMap struct holds a memory buffer with a corresponding size and the number of bits it can contain, matching the number of nodes in the tree.

You have already learned about the functions implemented in the 'bit_map.c' file, which are used to initialize the bitmap and query/set the bit values.

The BuddyAllocator struct stores information for managing the bitmap, including the number of tree levels, the minimum block size, and details about the amount of memory that can be managed.

1. BuddyAllocator_malloc Function:

Purpose: This function checks if there is sufficient available memory to satisfy the allocation request. It searches for the level at which to allocate memory, starting from the lowest level and iterating through the blocks at that level to find a free block. Once a free block is located, it changes the bit state of the selected block and its parent nodes up to the root using the Set_parent and Set_child functions.
Explanation: BuddyAllocator_malloc efficiently manages memory allocation by dividing memory into buddy blocks and recursively searching for an appropriate free block. When a suitable block is found, it updates the bitmap and maintains the tree structure.

2. BuddyAllocator_free Function:

Purpose: This function checks whether the provided address corresponds to a previously allocated block. If it does, the function changes the corresponding bit in the bitmap to mark the block as free.
Explanation: BuddyAllocator_free ensures that only valid allocated memory is released, and it updates the bitmap to reflect the freed block.

3. Bitmap_merge Function:

Purpose: This function compacts memory blocks by recursively merging two buddy blocks with their parent node until reaching the root.
Explanation: Bitmap_merge recursively checks and merges buddy blocks and their parent nodes. This operation helps maintain memory efficiency and prevents fragmentation.

## 3. How to run

```console
paolocupini@def:~$ git clone https://github.com/paolocupini/SO-project.git
paolocupini@def:~$ make
paolocupini@def:~$ ./buddy_allocator_test "x"


```
Where the variable x can take the following values:

1. x=-1 -> It runs all tests related to allocations using BuddyAllocator.
2. x=-2 -> It runs all tests related to allocations using mmap.
3. x>0 -> It manages allocation in two ways: through BuddyAllocator if x is less than 1024, otherwise using mmap.


## 4. Explanation of Testing:
To better visualize allocations and deallocations, a BuddyAllocator with 5 levels is tested. In this case, the minimum size of a bucket will be 1024/2^5 = 32, which, of course, leads to fragmentation. To reduce the block size to 2, you can simply change the number of levels from 6 to 9 in the 'buddy_allocator_test.c' file.




