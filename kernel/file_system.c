/*
 * kernel.c
 * Quantum SMS OS Kernel Core
 * Fully integrated with Blockchain, Filesystem, and Runtime Modules
 */

#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Blockchain headers
#include "blockchain.c"

// Constants
#define MAX_FILE_SIZE 1024
#define MAX_FILES 128

// In-memory file structure
typedef struct {
    char path[256];
    char data[MAX_FILE_SIZE];
} VirtualFile;

VirtualFile file_system[MAX_FILES];
int file_count = 0;

// File System Init
void init_file_system() {
    file_count = 0;
    printf("🗂️ Initializing Quantum SMS Virtual File System...\n");
}

// File Writing
void write_file(const char *path, const char *data) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_system[i].path, path) == 0) {
            strncpy(file_system[i].data, data, MAX_FILE_SIZE);
            printf("✍️ Updated file: %s\n", path);
            return;
        }
    }

    if (file_count < MAX_FILES) {
        strncpy(file_system[file_count].path, path, sizeof(file_system[file_count].path));
        strncpy(file_system[file_count].data, data, MAX_FILE_SIZE);
        file_count++;
        printf("📝 Created new file: %s\n", path);
    } else {
        printf("❌ File system full. Cannot create: %s\n", path);
    }
}

// File Reading
void read_file(const char *path, char *buffer) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_system[i].path, path) == 0) {
            strncpy(buffer, file_system[i].data, MAX_FILE_SIZE);
            printf("📖 Read file: %s\n", path);
            return;
        }
    }
    strcpy(buffer, "");
    printf("⚠️ File not found: %s\n", path);
}

// File-Based Transaction Trigger (smart contract simulation)
void trigger_transaction_from_file(const char *filepath) {
    char content[MAX_FILE_SIZE];
    read_file(filepath, content);

    if (strlen(content) > 0) {
        char sender[64], recipient[64], message[256];
        double amount = 0;

        sscanf(content, "%63s %63s %lf %[^\n]", sender, recipient, &amount, message);

        if (add_transaction(chain->latest, sender, recipient, amount, message)) {
            printf("💸 Blockchain transaction added from file!\n");
            mine_block();
        } else {
            printf("❌ Failed to add transaction from file.\n");
        }
    }
}

// Kernel Boot Sequence
void boot_kernel() {
    apply_theme("dark");
    print_theme_banner();

    init_file_system();
    chain = init_blockchain();

    printf("🧠 Quantum SMS Kernel Boot Complete.\n");
}
