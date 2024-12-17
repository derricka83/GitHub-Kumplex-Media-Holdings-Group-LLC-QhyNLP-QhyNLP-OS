#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

// Process control block
typedef struct {
    int pid;          // Process ID
    int priority;     // Process priority
    int state;        // Process state (e.g., running, waiting)
    void (*process_func)(void);  // Pointer to the process function
} Process;

// Global process table
static Process process_table[MAX_PROCESSES];
static int process_count = 0;

// Initialize process manager
void init_process_manager() {
    printf("Initializing Process Manager...\n");
    // Initialize process table with some dummy processes for now
    for (int i = 0; i < MAX_PROCESSES; i++) {
        process_table[i].pid = -1;  // Mark unused slots
    }
}

// Add a process to the process table
int add_process(void (*func)(void), int priority) {
    if (process_count < MAX_PROCESSES) {
        int pid = process_count;
        process_table[pid].pid = pid;
        process_table[pid].priority = priority;
        process_table[pid].state = 0; // Process state - running
        process_table[pid].process_func = func;
        process_count++;
        return pid;
    }
    return -1;  // No space for new process
}

// Schedule processes (round-robin for simplicity)
void schedule_processes() {
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].pid != -1) {
            // Execute the process function
            printf("Running process %d...\n", process_table[i].pid);
            process_table[i].process_func();
        }
    }
}

// Example of a process function
void example_process() {
    printf("Example process running...\n");
}
