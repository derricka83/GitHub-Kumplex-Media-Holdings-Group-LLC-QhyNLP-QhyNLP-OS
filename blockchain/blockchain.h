/*
 * blockchain.h
 * Quantum SMS OS Blockchain API Header
 * Interface for core blockchain functionality, tightly integrated with ZKP, Helia/IPFS, and QhyNLP
 */

#ifndef QSMS_BLOCKCHAIN_H
#define QSMS_BLOCKCHAIN_H

#include <time.h>
#include <stdbool.h>

// Constants
#define MAX_TRANSACTIONS 100
#define HASH_SIZE 64
#define ADDRESS_SIZE 64

// Typedefs

typedef struct {
    char sender[ADDRESS_SIZE];
    char recipient[ADDRESS_SIZE];
    double amount;
    char message[256]; // Payload via Quantum SMS
} Transaction;

typedef struct Block {
    int index;
    time_t timestamp;
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
    char previous_hash[HASH_SIZE];
    char hash[HASH_SIZE];
    struct Block* next;
} Block;

typedef struct Blockchain {
    Block* genesis;
    Block* latest;
    int size;
} Blockchain;

// Extern global blockchain instance
extern Blockchain* chain;

// Initialization
void init_blockchain(void);

// Block operations
Block* create_block(const char* previous_hash);
Block* mine_block(void);
void print_block(const Block* block);
void traverse_chain(void);

// Transaction operations
bool add_transaction(Block* block, const char* sender, const char* recipient, double amount, const char* message);

// Hashing
void hash_block(Block* block, char* out_hash);

// Theme support
void apply_theme(const char* theme);
void print_theme_banner(void);

// Utilities (optional, to be implemented in blockchain_utils.h/.c)
const char* get_current_theme(void);
Block* get_latest_block(void);
Block* get_block_by_index(int index);
bool validate_chain(void);
bool validate_block(const Block* block);

// Persistence (optional storage integration with IPFS/Helia)
bool save_blockchain_to_disk(const char* path);
bool load_blockchain_from_disk(const char* path);

#endif // QSMS_BLOCKCHAIN_H
