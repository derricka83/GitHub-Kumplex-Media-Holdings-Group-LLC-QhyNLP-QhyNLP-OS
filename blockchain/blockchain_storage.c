/*
 * blockchain_storage.c
 * Handles persistent storage and loading of the blockchain
 * Supports file-based saving/loading with JSON-style serialization
 */

#include "blockchain_storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STORAGE_FILE "data/blockchain.dat"

// Save a block to a file stream
static void write_block(FILE* file, const Block* block) {
    fprintf(file, "BLOCK %d\n", block->index);
    fprintf(file, "TIMESTAMP %ld\n", block->timestamp);
    fprintf(file, "PREV_HASH %s\n", block->previous_hash);
    fprintf(file, "HASH %s\n", block->hash);
    fprintf(file, "TX_COUNT %d\n", block->transaction_count);

    for (int i = 0; i < block->transaction_count; i++) {
        const Transaction* tx = &block->transactions[i];
        fprintf(file, "TX %s %s %.8f %s\n", tx->sender, tx->recipient, tx->amount, tx->message);
    }

    fprintf(file, "END_BLOCK\n\n");
}

// Load a single block from a file stream
static Block* read_block(FILE* file) {
    Block* block = (Block*)malloc(sizeof(Block));
    memset(block, 0, sizeof(Block));
    char line[1024];
    int tx_index = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "BLOCK", 5) == 0) {
            sscanf(line, "BLOCK %d", &block->index);
        } else if (strncmp(line, "TIMESTAMP", 9) == 0) {
            sscanf(line, "TIMESTAMP %ld", &block->timestamp);
        } else if (strncmp(line, "PREV_HASH", 9) == 0) {
            sscanf(line, "PREV_HASH %63s", block->previous_hash);
        } else if (strncmp(line, "HASH", 4) == 0) {
            sscanf(line, "HASH %63s", block->hash);
        } else if (strncmp(line, "TX_COUNT", 8) == 0) {
            sscanf(line, "TX_COUNT %d", &block->transaction_count);
        } else if (strncmp(line, "TX", 2) == 0) {
            Transaction* tx = &block->transactions[tx_index++];
            sscanf(line, "TX %63s %63s %lf %[^\n]", tx->sender, tx->recipient, &tx->amount, tx->message);
        } else if (strncmp(line, "END_BLOCK", 9) == 0) {
            return block;
        }
    }

    free(block);
    return NULL;
}

bool save_blockchain(const Blockchain* chain) {
    FILE* file = fopen(STORAGE_FILE, "w");
    if (!file) {
        perror("❌ Failed to open blockchain file for writing");
        return false;
    }

    Block* current = chain->genesis;
    while (current) {
        write_block(file, current);
        current = current->next;
    }

    fclose(file);
    printf("💾 Blockchain saved to %s\n", STORAGE_FILE);
    return true;
}

Blockchain* load_blockchain() {
    FILE* file = fopen(STORAGE_FILE, "r");
    if (!file) {
        perror("📂 No saved blockchain found, starting fresh");
        return NULL;
    }

    Blockchain* loaded = (Blockchain*)malloc(sizeof(Blockchain));
    loaded->genesis = NULL;
    loaded->latest = NULL;
    loaded->size = 0;

    Block* prev = NULL;

    while (!feof(file)) {
        Block* block = read_block(file);
        if (!block) break;

        if (!loaded->genesis) {
            loaded->genesis = block;
        }

        if (prev) {
            prev->next = block;
        }

        prev = block;
        loaded->latest = block;
        loaded->size++;
    }

    fclose(file);
    printf("📥 Blockchain loaded (%d blocks)\n", loaded->size);
    return loaded;
}
