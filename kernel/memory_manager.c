/*
 * memory_manager.c
 * Quantum SMS OS Memory Management System
 * Includes memory pool simulation, diagnostics, and blockchain logging
 */

#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MEMORY_POOL_SIZE 1024 * 1024  // 1 MB
#define MAX_ALLOCATIONS 256

typedef struct {
    void* address;
    size_t size;
    bool active;
} AllocationRecord;

static uint8_t memory_pool[MEMORY_POOL_SIZE];
static size_t memory_offset = 0;
static AllocationRecord allocations[MAX_ALLOCATIONS];

// Initialize memory manager
void init_memory_manager() {
    memory_offset = 0;
    memset(allocations, 0, sizeof(allocations));
    printf("🧠 Initializing Quantum Memory Manager (%d KB)...\n", MEMORY_POOL_SIZE / 1024);
}

// Allocate memory from memory pool
void* allocate_memory(size_t size) {
    if (memory_offset + size > MEMORY_POOL_SIZE) {
        printf("❌ Allocation failed: Out of memory.\n");
        return NULL;
    }

    void* ptr = &memory_pool[memory_offset];
    memory_offset += size;

    // Track allocation
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (!allocations[i].active) {
            allocations[i].address = ptr;
            allocations[i].size = size;
            allocations[i].active = true;
            printf("✅ Allocated %zu bytes at %p\n", size, ptr);
            return ptr;
        }
    }

    printf("⚠️ Allocation table full. Allocation not tracked.\n");
    return ptr;
}

// Free simulated memory (manual tracking)
void free_memory(void* ptr) {
    for (int i = 0; i < MAX_ALLOCATIONS; i++) {
        if (allocations[i].active && allocations[i].address == ptr) {
            allocations[i].active = false;
            printf("🧹 Freed memory at %p (%zu bytes)\n", ptr, allocations[i].size);

            // Blockchain logging for audit
            char msg[256];
            snprintf(msg, sizeof(msg), "Memory freed at %p (%zu bytes)", ptr, allocations[i].size);
            add_transaction(chain->latest, "kernel.qsms", "mem.audit.qsms", 0.0, msg);
            return;
        }
    }

    printf("⚠️ Attempted to free untracked memory at %p\n", ptr);
}
