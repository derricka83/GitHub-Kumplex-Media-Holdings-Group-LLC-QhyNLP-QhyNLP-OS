#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_POOL_SIZE 1024  // Size of the memory pool in bytes
static char memory_pool[MEMORY_POOL_SIZE];  // Simulated memory pool
static int memory_usage = 0;  // Tracks the current memory usage

// Initialize memory manager
void init_memory_manager() {
    printf("Initializing Memory Manager...\n");
    memory_usage = 0;  // Reset memory usage
}

// Allocate memory from the pool
void* allocate_memory(size_t size) {
    if (memory_usage + size <= MEMORY_POOL_SIZE) {
        void* ptr = &memory_pool[memory_usage];
        memory_usage += size;
        return ptr;
    }
    return NULL;  // Not enough memory
}

// Free memory (not actually needed in this simplified model)
void free_memory(void* ptr) {
    // No-op: we don't handle freeing memory in this basic model.
    // In a more advanced implementation, you could add memory defragmentation.
}

// Memory information
void print_memory_info() {
    printf("Memory usage: %d/%d bytes\n", memory_usage, MEMORY_POOL_SIZE);
}
