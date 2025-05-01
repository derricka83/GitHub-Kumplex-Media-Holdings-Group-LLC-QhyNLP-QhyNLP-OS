/*
 * transaction_pool.c
 * Advanced Transaction Pool for Quantum SMS OS Blockchain
 * Supports secure memory, validation, and concurrent-safe transaction queueing
 */

#include "transaction_pool.h"
#include "utils/crypto.h"
#include "utils/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define MAX_POOL_SIZE 2048
#define MAX_TX_LIFESPAN 600 // seconds (10 minutes)

static TransactionPool pool;
static pthread_mutex_t pool_lock = PTHREAD_MUTEX_INITIALIZER;

void init_transaction_pool() {
    pthread_mutex_lock(&pool_lock);
    pool.count = 0;
    pool.last_flush_time = time(NULL);
    memset(pool.transactions, 0, sizeof(pool.transactions));
    pthread_mutex_unlock(&pool_lock);
    log_info("🧃 Transaction pool initialized with capacity %d", MAX_POOL_SIZE);
}

bool validate_transaction(const Transaction* tx) {
    if (!tx) return false;
    if (tx->amount <= 0.0) return false;
    if (strlen(tx->sender) == 0 || strlen(tx->recipient) == 0) return false;
    if (strcmp(tx->sender, tx->recipient) == 0) return false;
    // Extend: digital signature validation, smart contract hooks, gas fees, etc.
    return true;
}

bool is_duplicate_transaction(const Transaction* tx) {
    for (int i = 0; i < pool.count; ++i) {
        if (strncmp(pool.transactions[i].id, tx->id, TX_ID_SIZE) == 0) {
            return true;
        }
    }
    return false;
}

bool add_to_pool(Transaction* tx) {
    pthread_mutex_lock(&pool_lock);

    if (pool.count >= MAX_POOL_SIZE) {
        log_warn("⚠️ Transaction pool full: max %d transactions", MAX_POOL_SIZE);
        pthread_mutex_unlock(&pool_lock);
        return false;
    }

    if (!validate_transaction(tx)) {
        log_error("❌ Transaction failed validation");
        pthread_mutex_unlock(&pool_lock);
        return false;
    }

    if (is_duplicate_transaction(tx)) {
        log_warn("⚠️ Duplicate transaction rejected");
        pthread_mutex_unlock(&pool_lock);
        return false;
    }

    // Timestamp transaction for lifespan control
    tx->timestamp = time(NULL);

    pool.transactions[pool.count++] = *tx;
    log_success("➕ Transaction added: %s → %s (%.2f) | ID: %s", tx->sender, tx->recipient, tx->amount, tx->id);

    pthread_mutex_unlock(&pool_lock);
    return true;
}

int fetch_transactions(Transaction* buffer, int max_count) {
    pthread_mutex_lock(&pool_lock);

    int count = (pool.count < max_count) ? pool.count : max_count;
    for (int i = 0; i < count; ++i) {
        buffer[i] = pool.transactions[i];
    }

    pthread_mutex_unlock(&pool_lock);
    return count;
}

void remove_transactions_by_id(const char ids[][TX_ID_SIZE], int count) {
    pthread_mutex_lock(&pool_lock);

    for (int i = 0; i < count; ++i) {
        for (int j = 0; j < pool.count; ++j) {
            if (strncmp(pool.transactions[j].id, ids[i], TX_ID_SIZE) == 0) {
                // Shift remaining
                for (int k = j; k < pool.count - 1; ++k) {
                    pool.transactions[k] = pool.transactions[k + 1];
                }
                pool.count--;
                break;
            }
        }
    }

    pthread_mutex_unlock(&pool_lock);
}

void flush_expired_transactions() {
    time_t now = time(NULL);
    pthread_mutex_lock(&pool_lock);

    int removed = 0;
    for (int i = 0; i < pool.count; ) {
        if ((now - pool.transactions[i].timestamp) > MAX_TX_LIFESPAN) {
            for (int j = i; j < pool.count - 1; ++j) {
                pool.transactions[j] = pool.transactions[j + 1];
            }
            pool.count--;
            removed++;
        } else {
            i++;
        }
    }

    pthread_mutex_unlock(&pool_lock);
    if (removed > 0) {
        log_info("🧹 Flushed %d expired transactions from pool", removed);
    }
}

void print_transaction_pool_summary() {
    pthread_mutex_lock(&pool_lock);
    printf("📊 Transaction Pool Summary:\n");
    printf("Total transactions: %d\n", pool.count);
    for (int i = 0; i < pool.count; ++i) {
        Transaction* tx = &pool.transactions[i];
        printf("  🧾 ID: %s | %s → %s | Amount: %.2f | Time: %s",
               tx->id, tx->sender, tx->recipient, tx->amount, ctime(&tx->timestamp));
    }
    pthread_mutex_unlock(&pool_lock);
}
