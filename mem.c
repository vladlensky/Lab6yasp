#include <signal.h>
#include "mem.h"
#define PAGE_SIZE 4096

void* heap_init(size_t initial_size) {
    void* chunk = mmap(HEAP_START, (initial_size)*PAGE_SIZE,
                       PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    mem_t* head = HEAP_START;
    head->next     = NULL;
    head->capacity = PAGE_SIZE*(initial_size) - sizeof(mem_t);
    head->is_free  = 1;
    return chunk;
}

mem_t* new_chunk(mem_t* last, size_t query) {
    size_t temp = query/PAGE_SIZE;
    int8_t * address = (int8_t*)last+last->capacity+sizeof(mem_t);
    size_t size = query+sizeof(mem_t);
    mem_t* chunk = mmap(address, (temp+1)*PAGE_SIZE,
                       PROT_READ | PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if(chunk==MAP_FAILED)
        chunk = mmap(NULL, (temp+1)*PAGE_SIZE,
                     PROT_READ | PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
    if(chunk==MAP_FAILED)
        return NULL;
    mem_t* next_chunk = NULL;
    if((temp+1)*4096>=size+sizeof(mem_t)){
        next_chunk = (mem_t*)(address + size);
        next_chunk->next = NULL;
        next_chunk->is_free = 1;
        next_chunk->capacity = (temp+1)*4096-query-2*sizeof(mem_t);
    }
    chunk->capacity=query;
    chunk->is_free=0;
    chunk->next = next_chunk;
    return chunk;
}
mem_t* has_chunk(mem_t* start,size_t query){
    mem_t* chunk = start;
    while(chunk){
        if(chunk->is_free&&chunk->capacity>=query)
            return chunk;
        chunk = chunk->next;
    }
    return NULL;
}
mem_t* last_chunk(mem_t* start){
    mem_t* chunk = start;
    while(chunk->next){
        chunk = chunk->next;
    }
    return chunk;
}

mem_t* find_previous_chunk(mem_t* chunk,void * chunk_to_find){
    while (chunk->next){
        mem_t * mem = chunk->next+1;
        if(chunk->next+1==(mem_t*)chunk_to_find)
            return chunk;
        chunk= chunk->next;
    }
    return NULL;
}

void* _malloc(size_t query) {
    mem_t* chunk = has_chunk(HEAP_START,query);
    mem_t* new = NULL;
    if(chunk) {
        if (chunk->capacity == query ||
            (chunk->capacity - query - 2*sizeof(mem_t)) < 0) {
            chunk->is_free = 0;
            return chunk + 1;
        }
        new = (mem_t*)((uint8_t*)chunk + sizeof(mem_t) + query);
        new->capacity   = chunk->capacity - query - sizeof(mem_t);
        new->is_free    = 1;
        new->next       = chunk->next;
        chunk->next     = new;
        chunk->capacity = query;
        chunk->is_free  = 0;
    } else {
        new = new_chunk(last_chunk(HEAP_START), query);
        last_chunk(HEAP_START)->next = new;
        return new+1;
    }
    return (int8_t*)chunk + sizeof(mem_t);
}

void _free(void* mem) {
    mem_t* prevChunk = find_previous_chunk(HEAP_START, mem);
    mem_t* chunk = HEAP_START;
    if(prevChunk)
        chunk  = prevChunk->next;
    if (chunk) {
        chunk->is_free = 1;
        if (chunk->next)
            if(chunk->next->is_free == 1) {
                chunk->capacity += chunk->next->capacity + sizeof(mem_t);
                chunk->next = chunk->next->next;
            }
        if(prevChunk)
            if(prevChunk->is_free == 1) {
                prevChunk->capacity += chunk->capacity + sizeof(mem_t);
                prevChunk->next = chunk->next;
            }
    }
}