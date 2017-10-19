#include <stdio.h>
#include "mem.h"

int main() {
    printf("heap after malloc:\n");
    heap_init(1);
    void* first = _malloc(4076);
    void* second = _malloc(100000000);
    void* third = _malloc(4096);
    void* fourth = _malloc(10);
    memalloc_debug_heap(stdout,HEAP_START);
    printf("heap after free:\n");
    _free(third);
    _free(first);
    _free(fourth);
    _free(second);
    memalloc_debug_heap(stdout,HEAP_START);
    return 0;
}