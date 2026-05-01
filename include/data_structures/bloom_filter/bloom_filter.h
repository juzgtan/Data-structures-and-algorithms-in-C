#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include "data_structures/bit_vector/bit_vector.h"
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
 * @brief Bloom filter structure
 *
 * A space-efficient probabilistic data structure for set membership testing.
 * - False positives possible, false negatives impossible.
 * - Uses multiple hash functions and a bit array.
 */
typedef struct {
  BitVector *bits;     /**< Underlying bit vector */
  size_t num_bits;     /**< Number of bits in the filter */
  size_t num_hashes;   /**< Number of hash functions to use */
  size_t num_elements; /**< Number of elements inserted */
} BloomFilter;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new Bloom filter with specified parameters
 * @param num_bits Number of bits (0 = use default 1024)
 * @param num_hashes Number of hash functions (0 = use default 3)
 * @param result Output pointer to receive the new BloomFilter
 * @return Result code
 *
 * @complexity O(num_bits/8)
 */
ResultCode BloomFilter_Create(size_t num_bits, size_t num_hashes,
                              BloomFilter **result);

/**
 * @brief Creates a Bloom filter with optimal parameters
 * @param expected_elements Expected number of elements to insert
 * @param false_positive_rate Desired false positive probability (0 < p < 1)
 * @param result Output pointer to receive the new BloomFilter
 * @return Result code
 *
 * Automatically calculates:
 * - Optimal number of bits: m = - (n * ln(p)) / (ln(2)^2)
 * - Optimal number of hashes: k = (m / n) * ln(2)
 *
 * @complexity O(m/8)
 */
ResultCode BloomFilter_CreateOptimal(size_t expected_elements,
                                     double false_positive_rate,
                                     BloomFilter **result);

/**
 * @brief Destroys the Bloom filter and frees all associated memory
 * @param bf Bloom filter to destroy (can be NULL)
 *
 * @complexity O(1)
 */
void BloomFilter_Destroy(BloomFilter *bf);

/**
 * @brief Removes all elements from the Bloom filter
 * @param bf Bloom filter to clear
 *
 * @complexity O(num_bits/8)
 */
void BloomFilter_Clear(BloomFilter *bf);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of bits in the filter
 * @param bf Bloom filter to examine
 * @return Number of bits (0 if bf is NULL)
 *
 * @complexity O(1)
 */
size_t BloomFilter_NumBits(const BloomFilter *bf);

/**
 * @brief Returns the number of hash functions used
 * @param bf Bloom filter to examine
 * @return Number of hash functions (0 if bf is NULL)
 *
 * @complexity O(1)
 */
size_t BloomFilter_NumHashes(const BloomFilter *bf);

/**
 * @brief Returns the number of elements inserted
 * @param bf Bloom filter to examine
 * @return Number of elements (0 if bf is NULL)
 *
 * @complexity O(1)
 */
size_t BloomFilter_NumElements(const BloomFilter *bf);

/* ============================================================================
 * OPERATIONS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Adds an element to the Bloom filter
 * @param bf Bloom filter to modify
 * @param data Pointer to data to add
 * @param data_len Length of data in bytes
 * @return Result code
 *
 * @complexity O(k) where k = number of hash functions
 */
ResultCode BloomFilter_Add(BloomFilter *bf, const void *data, size_t data_len);

/**
 * @brief Checks if an element might be in the set
 * @param bf Bloom filter to query
 * @param data Pointer to data to check
 * @param data_len Length of data in bytes
 * @return true if probably present, false if definitely absent
 *
 * @complexity O(k) where k = number of hash functions
 */
bool BloomFilter_ProbablyContains(const BloomFilter *bf, const void *data,
                                  size_t data_len);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Calculates the current false positive probability
 * @param bf Bloom filter to analyze
 * @return False positive probability (between 0 and 1)
 *
 * Formula: p = (1 - e^(-k * n / m))^k
 *
 * @complexity O(1)
 */
double BloomFilter_FalsePositiveRate(const BloomFilter *bf);

/**
 * @brief Calculates the expected false positive rate
 * @param num_bits Number of bits in the filter
 * @param num_hashes Number of hash functions
 * @param expected_elements Expected number of elements to insert
 * @return Expected false positive probability
 *
 * @complexity O(1)
 */
double BloomFilter_ExpectedFalsePositiveRate(size_t num_bits, size_t num_hashes,
                                             size_t expected_elements);

/**
 * @brief Calculates how full the filter is (set bits / total bits)
 * @param bf Bloom filter to analyze
 * @return Load factor (between 0 and 1)
 *
 * @complexity O(num_bits/8)
 */
double BloomFilter_LoadFactor(const BloomFilter *bf);

/**
 * @brief Computes the union of two Bloom filters (bf1 = bf1 OR bf2)
 * @param bf1 First Bloom filter (will be modified)
 * @param bf2 Second Bloom filter
 * @return Result code
 *
 * Requirements: Both filters must have same size and number of hash functions.
 *
 * @complexity O(num_bits/8)
 */
ResultCode BloomFilter_Union(BloomFilter *bf1, const BloomFilter *bf2);

/**
 * @brief Computes the intersection of two Bloom filters (bf1 = bf1 AND bf2)
 * @param bf1 First Bloom filter (will be modified)
 * @param bf2 Second Bloom filter
 * @return Result code
 *
 * Requirements: Both filters must have same size and number of hash functions.
 *
 * @complexity O(num_bits/8)
 */
ResultCode BloomFilter_Intersection(BloomFilter *bf1, const BloomFilter *bf2);

/**
 * @brief Checks if bf1 is a subset of bf2
 * @param bf1 Potential subset
 * @param bf2 Superset
 * @return true if bf1 ⊆ bf2, false otherwise
 *
 * @complexity O(num_bits/8)
 */
bool BloomFilter_IsSubset(const BloomFilter *bf1, const BloomFilter *bf2);

/**
 * @brief Prints information about the Bloom filter (debugging)
 * @param bf Bloom filter to analyze
 *
 * @complexity O(num_bits/8)
 */
void BloomFilter_PrintInfo(const BloomFilter *bf);

/**
 * @brief Prints the underlying bit vector (debugging)
 * @param bf Bloom filter to print
 *
 * @complexity O(num_bits)
 */
void BloomFilter_PrintBits(const BloomFilter *bf);

/**
 * @brief Resets the Bloom filter (same as Clear)
 * @param bf Bloom filter to reset
 *
 * @complexity O(num_bits/8)
 */
void BloomFilter_Reset(BloomFilter *bf);

/**
 * @brief Creates a deep copy of the Bloom filter
 * @param src Source Bloom filter
 * @param out_dest Output pointer for destination Bloom filter
 * @return Result code
 *
 * @complexity O(num_bits/8)
 */
ResultCode BloomFilter_Copy(const BloomFilter *src, BloomFilter **out_dest);

#ifdef __cplusplus
}
#endif

#endif /* BLOOM_FILTER_H */
