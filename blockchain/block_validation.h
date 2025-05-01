/*
 * block_validation.h
 * Header for Block Validation in the Quantum SMS Blockchain System
 * Provides integrity verification for blocks and the overall chain
 */

#ifndef BLOCK_VALIDATION_H
#define BLOCK_VALIDATION_H

#include <stdbool.h>
#include "blockchain.h"

// Compare two hashes for equality
bool hashes_match(const char* h1, const char* h2);

// Check if a single block’s hash is valid
bool is_block_hash_valid(const Block* block);

// Check if the current block's previous_hash matches the actual hash of the previous block
bool is_previous_hash_valid(const Block* current, const Block* previous);

// Validate a single block's integrity
bool validate_block(const Block* block);

// Validate the entire blockchain
bool validate_chain();

#endif // BLOCK_VALIDATION_H
