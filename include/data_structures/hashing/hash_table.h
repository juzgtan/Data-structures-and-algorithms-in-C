#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "hash_functions.h"
#include "utils/result_code.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * STRUCTURES
 * ============================================================================
 */

/**
 * @brief Hash table entry (key-value pair)
 */
typedef struct HashEntry {
  char *key;              /* Key string (dynamically allocated) */
  void *value;            /* Value pointer */
  struct HashEntry *next; /* Next entry in bucket (for chaining) */
} HashEntry;

/**
 * @brief Hash table structure
 */
typedef struct {
  HashEntry **buckets;    /* Array of bucket heads */
  size_t size;            /* Number of key-value pairs */
  size_t capacity;        /* Number of buckets */
  float load_factor;      /* Threshold for resizing (e.g., 0.75) */
  HashFunction hash_func; /* Hash function to use */

  /* Optional: function to free values when removed */
  void (*value_destructor)(void *);
} HashTable;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new hash table
 * @param initial_capacity Initial number of buckets (0 = use default)
 * @param hash_func Hash function to use (NULL = use default DJB2)
 * @param result Output pointer to receive the new HashTable
 * @return Result code
 */
ResultCode HashTable_Create(size_t initial_capacity, HashFunction hash_func,
                            HashTable **result);

/**
 * @brief Destroys the hash table and frees all memory
 * @param ht Hash table to destroy (can be NULL)
 */
void HashTable_Destroy(HashTable *ht);

/**
 * @brief Removes all entries from the hash table
 * @param ht Hash table to clear
 */
void HashTable_Clear(HashTable *ht);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of key-value pairs in the hash table
 * @param ht Hash table to examine
 * @return Number of entries
 */
size_t HashTable_Size(const HashTable *ht);

/**
 * @brief Returns the number of buckets (capacity)
 * @param ht Hash table to examine
 * @return Number of buckets
 */
size_t HashTable_Capacity(const HashTable *ht);

/**
 * @brief Checks if the hash table is empty
 * @param ht Hash table to examine
 * @return true if empty, false otherwise
 */
bool HashTable_IsEmpty(const HashTable *ht);

/**
 * @brief Gets the current load factor (size / capacity)
 * @param ht Hash table to examine
 * @return Load factor
 */
float HashTable_LoadFactor(const HashTable *ht);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Inserts or updates a key-value pair
 * @param ht Hash table to modify
 * @param key Key string (will be copied)
 * @param value Value pointer
 * @return Result code
 */
ResultCode HashTable_Put(HashTable *ht, const char *key, void *value);

/**
 * @brief Retrieves a value by key
 * @param ht Hash table to search
 * @param key Key to look up
 * @param out_value Output pointer to receive the value
 * @return Result code
 */
ResultCode HashTable_Get(const HashTable *ht, const char *key,
                         void **out_value);

/**
 * @brief Removes a key-value pair by key
 * @param ht Hash table to modify
 * @param key Key to remove
 * @return Result code
 */
ResultCode HashTable_Remove(HashTable *ht, const char *key);

/**
 * @brief Checks if a key exists in the hash table
 * @param ht Hash table to search
 * @param key Key to check
 * @return true if exists, false otherwise
 */
bool HashTable_ContainsKey(const HashTable *ht, const char *key);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Resizes the hash table to a new capacity
 * @param ht Hash table to modify
 * @param new_capacity New number of buckets
 * @return Result code
 */
ResultCode HashTable_Resize(HashTable *ht, size_t new_capacity);

/**
 * @brief Sets a destructor function for values
 *
 * When set, this function is called automatically when a value is
 * removed or replaced, allowing proper cleanup of dynamically allocated data.
 *
 * @param ht Hash table to modify
 * @param destructor Destructor function (can be NULL)
 */
void HashTable_SetValueDestructor(HashTable *ht, void (*destructor)(void *));

/**
 * @brief Gets all keys from the hash table
 * @param ht Hash table to examine
 * @param keys Output array for keys (must be pre-allocated)
 * @param out_len Number of keys written
 * @return Result code
 */
ResultCode HashTable_GetKeys(const HashTable *ht, const char **keys,
                             size_t *out_len);

/**
 * @brief Gets all values from the hash table
 * @param ht Hash table to examine
 * @param values Output array for values (must be pre-allocated)
 * @param out_len Number of values written
 * @return Result code
 */
ResultCode HashTable_GetValues(const HashTable *ht, void **values,
                               size_t *out_len);

#ifdef __cplusplus
}
#endif

#endif /* HASH_TABLE_H */
