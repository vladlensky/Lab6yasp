#ifndef _MEM_H_
#define _MEM_H_
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <sys/mman.h>

#define HEAP_START ((void*)0x04040000)

#pragma pack(push, 1)
typedef struct _mem_t {
    struct _mem_t* next;
    size_t capacity;
    int is_free;
} mem_t;
#pragma pack(pop)

void* _malloc (size_t query);
void  _free   (void*  mem);
void* heap_init(size_t initial_size );

#define DEBUG_FIRST_BYTES 4

void memalloc_debug_struct_info(FILE* f, const mem_t* const address);
void memalloc_debug_heap(FILE* f, const mem_t* ptr);

#endif