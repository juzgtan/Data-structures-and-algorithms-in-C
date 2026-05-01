#ifndef HASH_FUNCTIONS_H
#define HASH_FUNCTIONS_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * HASH FUNCTION TYPES
 * ============================================================================
 */

/**
 * @brief Hash function type
 * @param key Pointer to the key to hash
 * @param len Length of the key in bytes
 * @return Hash value (typically 32-bit or 64-bit)
 */
typedef uint64_t (*HashFunction)(const void *key, size_t len);

/* ============================================================================
 * COMMON HASH FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Simple additive hash (not for production)
 * @param key Key to hash
 * @param len Length of key
 * @return Hash value
 */
uint64_t Hash_Additive(const void *key, size_t len);

/**
 * @brief XOR hash (not for production)
 * @param key Key to hash
 * @param len Length of key
 * @return Hash value
 */
uint64_t Hash_XOR(const void *key, size_t len);

/**
 * @brief DJB2 hash (popular, good distribution)
 * @param key Key to hash
 * @param len Length of key
 * @return Hash value
 */
uint64_t Hash_DJB2(const void *key, size_t len);

/**
 * @brief SDBM hash (good for strings)
 * @param key Key to hash
 * @param len Length of key
 * @return Hash value
 */
uint64_t Hash_SDBM(const void *key, size_t len);

/**
 * @brief FNV-1a hash (Fowler-Noll-Vo, good for short strings)
 * @param key Key to hash
 * @param len Length of key
 * @return Hash value
 */
uint64_t Hash_FNV1a(const void *key, size_t len);

/**
 * @brief Jenkins one-at-a-time hash (good for general use)
 * @param key Key to hash
 * @param len Length of key
 * @return Hash value
 */
uint64_t Hash_Jenkins(const void *key, size_t len);

/**
 * @brief MurmurHash3 32-bit (fast, good distribution)
 * @param key Key to hash
 * @param len Length of key
 * @return Hash value
 */
uint64_t Hash_Murmur3(const void *key, size_t len);

/**
 * @brief Simple hash for integers
 * @param key Integer key
 * @return Hash value
 */
uint64_t Hash_Integer(int key);

/**
 * @brief Simple hash for pointers
 * @param key Pointer key
 * @return Hash value
 */
uint64_t Hash_Pointer(const void *key);

/* ============================================================================
 * HASH UTILITIES
 * ============================================================================
 */

/**
 * @brief Combines two hash values (for composite keys)
 * @param hash1 First hash value
 * @param hash2 Second hash value
 * @return Combined hash
 */
uint64_t Hash_Combine(uint64_t hash1, uint64_t hash2);

/**
 * @brief Reduces a hash value to a bucket index
 * @param hash Hash value
 * @param bucket_count Number of buckets
 * @return Bucket index (0 to bucket_count-1)
 */
static inline size_t Hash_ToBucket(uint64_t hash, size_t bucket_count) {
  return hash % bucket_count;
}

#ifdef __cplusplus
}
#endif

#endif /* HASH_FUNCTIONS_H */
