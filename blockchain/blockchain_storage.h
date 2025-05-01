/*
 * blockchain_storage.h
 * Interface for persistent blockchain storage operations
 * Supports saving and loading blockchain data from disk
 */

#ifndef BLOCKCHAIN_STORAGE_H
#define BLOCKCHAIN_STORAGE_H

#include "blockchain.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Saves the entire blockchain to disk.
 * @param chain Pointer to the blockchain instance.
 * @return true if successful, false otherwise.
 */
bool save_blockchain(const Blockchain* chain);

/**
 * Loads a blockchain from disk if available.
 * @return Pointer to the loaded blockchain instance, or NULL if loading failed.
 */
Blockchain* load_blockchain(void);

#ifdef __cplusplus
}
#endif

#endif // BLOCKCHAIN_STORAGE_H
