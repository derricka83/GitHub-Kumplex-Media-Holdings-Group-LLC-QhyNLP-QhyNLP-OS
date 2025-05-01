/*
 * process_manager.c
 * Quantum SMS OS Process Manager
 * Includes basic process lifecycle management and blockchain-integrated auditing
 */

#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PROCESSES 64

typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_WAITING,
    PROCESS_TERMINATED
} ProcessState;

typedef struct {
    int pid;
    char name[32];
    ProcessState state;
    time_t start_time;
} Process;

static Process process_table[MAX_PROCESSES];
static int current_pid = 1;
static int running_index = -1;

// Initialize process table
void init_process_manager() {
    memset(process_table, 0, sizeof(process_table));
    printf("🧬 Initializing Quantum Process Manager...\n");
}

// Create a new process
int create_process(const char* name) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].state == PROCESS_TERMINATED || process_table[i].pid == 0) {
            process_table[i].pid = current_pid++;
            strncpy(process_table[i].name, name, sizeof(process_table[i].name));
            process_table[i].state = PROCESS_READY;
            process_table[i].start_time = time(NULL);
            printf("✅ Created process [%s] with PID %d\n", name, process_table[i].pid);

            // Blockchain audit
            char msg[128];
            snprintf(msg, sizeof(msg), "Process %s (PID %d) created", name, process_table[i].pid);
            add_transaction(chain->latest, "proc.qsms", "audit.qsms", 0.0, msg);
            return process_table[i].pid;
        }
    }

    printf("❌ Process table full. Failed to create process.\n");
    return -1;
}

// Terminate a process
void terminate_process(int pid) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (process_table[i].pid == pid && process_table[i].state != PROCESS_TERMINATED) {
            process_table[i].state = PROCESS_TERMINATED;
            printf("🛑 Terminated process PID %d (%s)\n", pid, process_table[i].name);

            // Blockchain audit
            char msg[128];
            snprintf(msg, sizeof(msg), "Process %s (PID %d) terminated", process_table[i].name, pid);
            add_transaction(chain->latest, "proc.qsms", "audit.qsms", 0.0, msg);
            return;
        }
    }

    printf("⚠️ Process PID %d not found or already terminated\n", pid);
}

// Simple round-robin scheduler
void schedule_processes() {
    printf("🕒 Scheduling processes...\n");

    for (int i = 0; i < MAX_PROCESSES; i++) {
        int index = (running_index + 1 + i) % MAX_PROCESSES;

        if (process_table[index].state == PROCESS_READY) {
            if (running_index >= 0 && process_table[running_index].state == PROCESS_RUNNING)
                process_table[running_index].state = PROCESS_READY;

            process_table[index].state = PROCESS_RUNNING;
            running_index = index;

            printf("▶️ Running process PID %d (%s)\n", process_table[index].pid, process_table[index].name);
            return;
        }
    }

    printf("⏸️ No ready processes to schedule.\n");
}
