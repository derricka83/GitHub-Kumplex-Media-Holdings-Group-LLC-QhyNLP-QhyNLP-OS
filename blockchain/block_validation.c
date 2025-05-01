/*
 * block_validation.c
 * Block Validation Engine for Quantum SMS Blockchain
 * Ensures structural integrity, hash consistency, and prepares for future ZKP-based verification
 */

#include "blockchain.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Internal helper to compare hashes
bool hashes_match(const char* h1, const char* h2) {
    return strncmp(h1, h2, HASH_SIZE) == 0;
}

// Verifies the hash of a block is correctly calculated
bool is_block_hash_valid(const Block* block) {
    char expected_hash[HASH_SIZE];
    Block temp = *block;
    hash_block(&temp, expected_hash);

    if (!hashes_match(expected_hash, block->hash)) {
        printf("❌ Block #%d: Hash mismatch! Expected: %s, Found: %s\n", block->index, expected_hash, block->hash);
        return false;
    }
    return true;
}

// Verifies the linkage between two blocks
bool is_previous_hash_valid(const Block* current, const Block* previous) {
    if (!hashes_match(current->previous_hash, previous->hash)) {
        printf("❌ Block #%d: Previous hash mismatch!\n", current->index);
        return false;
    }
    return true;
}

// Validate a single block (hash + structure)
bool validate_block(const Block* block) {
    if (!block) return false;
    return is_block_hash_valid(block);
}

// Validates the entire blockchain
bool validate_chain() {
    if (!chain || !chain->genesis) {
        printf("⚠️ Blockchain is uninitialized.\n");
        return false;
    }

    Block* current = chain->genesis;
    Block* previous = NULL;

    while (current) {
        if (!validate_block(current)) return false;
        if (previous && !is_previous_hash_valid(current, previous)) return false;

        previous = current;
        current = current->next;
    }

    printf("✅ Blockchain is valid.\n");
    return true;
}
