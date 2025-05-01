/*
 * transaction_pool.h
 * Header for Transaction Pool in Quantum SMS OS Blockchain
 */

#ifndef TRANSACTION_POOL_H
#define TRANSACTION_POOL_H

#include <stdbool.h>
#include <time.h>

#define TX_ID_SIZE 64
#define MAX_TRANSACTIONS 2048
#define ADDRESS_SIZE 64

typedef struct {
    char id[TX_ID_SIZE];           // Unique transaction ID (e.g., hash or UUID)
    char sender[ADDRESS_SIZE];     // Sender address
    char recipient[ADDRESS_SIZE];  // Recipient address
    double amount;                 // Amount transferred
    char message[256];             // Optional transaction message
    time_t timestamp;              // Time the transaction was added
} Transaction;

typedef struct {
    Transaction transactions[MAX_TRANSACTIONS];
    int count;
    time_t last_flush_time;
} TransactionPool;

// Initializes the transaction pool
void init_transaction_pool();

// Validates a transaction before acceptance
bool validate_transaction(const Transaction* tx);

// Checks for duplicate transactions in the pool
bool is_duplicate_transaction(const Transaction* tx);

// Adds a transaction to the pool (if valid and non-duplicate)
bool add_to_pool(Transaction* tx);

// Retrieves transactions from the pool up to a maximum count
int fetch_transactions(Transaction* buffer, int max_count);

// Removes transactions from the pool by ID (after inclusion in a block)
void remove_transactions_by_id(const char ids[][TX_ID_SIZE], int count);

// Flushes expired transactions from the pool
void flush_expired_transactions();

// Outputs a human-readable summary of the current pool
void print_transaction_pool_summary();

#endif // TRANSACTION_POOL_H
