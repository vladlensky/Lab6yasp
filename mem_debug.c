#include "mem.h"

void memalloc_debug_struct_info(FILE* f, const mem_t* const address) {
    size_t i;
    fprintf(f, "start: %p\nsize: %lu\nis_free: %d\n",
            (void*)address,
            address->capacity,
            address->is_free);

    for ( i = 0; i < DEBUG_FIRST_BYTES && i < address->capacity; i++ ) {
        fprintf(f, "%X", ((uint8_t*)address)[sizeof(mem_t) + i]);
        putc('\n', f);
    }
}

void memalloc_debug_heap(FILE* f, const mem_t* ptr) {
    for(;ptr; ptr = ptr->next ) {
        memalloc_debug_struct_info(f, ptr);
    }
}