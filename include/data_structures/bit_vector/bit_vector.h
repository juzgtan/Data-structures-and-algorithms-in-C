#ifndef BIT_VECTOR_H
#define BIT_VECTOR_H

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
 * @brief Bit vector structure (compact boolean array)
 *
 * Stores bits in a compact form using 1 bit per element.
 * Memory usage: 1/8 of a boolean array.
 */
typedef struct {
  size_t num_bits;     /**< Number of bits in the vector */
  size_t num_bytes;    /**< Number of bytes used for storage */
  unsigned char *data; /**< Underlying byte array */
} BitVector;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new bit vector with all bits initialized to 0
 * @param num_bits Number of bits (must be > 0)
 * @param result Output pointer to receive the new BitVector
 * @return Result code
 *
 * @complexity O(num_bytes)
 */
ResultCode BitVector_Create(size_t num_bits, BitVector **result);

/**
 * @brief Destroys the bit vector and frees all associated memory
 * @param bv Bit vector to destroy (can be NULL)
 *
 * @complexity O(1)
 */
void BitVector_Destroy(BitVector *bv);

/**
 * @brief Sets all bits to 0
 * @param bv Bit vector to clear
 *
 * @complexity O(num_bytes)
 */
void BitVector_Clear(BitVector *bv);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of bits in the vector
 * @param bv Bit vector to examine
 * @return Number of bits (0 if bv is NULL)
 *
 * @complexity O(1)
 */
size_t BitVector_Size(const BitVector *bv);

/**
 * @brief Returns the number of bytes used by the vector
 * @param bv Bit vector to examine
 * @return Number of bytes (0 if bv is NULL)
 *
 * @complexity O(1)
 */
size_t BitVector_NumBytes(const BitVector *bv);

/**
 * @brief Checks if the bit vector is empty (always false for valid vector)
 * @param bv Bit vector to examine
 * @return true only if bv is NULL
 *
 * @complexity O(1)
 */
bool BitVector_IsEmpty(const BitVector *bv);

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Sets a specific bit to a value (0 or 1)
 * @param bv Bit vector to modify
 * @param bit_index Position of the bit (0-based)
 * @param value true for 1, false for 0
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BitVector_Set(BitVector *bv, size_t bit_index, bool value);

/**
 * @brief Retrieves the value of a specific bit
 * @param bv Bit vector to read
 * @param bit_index Position of the bit (0-based)
 * @param out Output pointer to receive the bit value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BitVector_Get(const BitVector *bv, size_t bit_index, bool *out);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Flips a specific bit (0 becomes 1, 1 becomes 0)
 * @param bv Bit vector to modify
 * @param bit_index Position of the bit to flip (0-based)
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BitVector_Flip(BitVector *bv, size_t bit_index);

/**
 * @brief Sets all bits to the same value
 * @param bv Bit vector to modify
 * @param value true for all 1's, false for all 0's
 * @return Result code
 *
 * @complexity O(num_bytes)
 */
ResultCode BitVector_SetAll(BitVector *bv, bool value);

/**
 * @brief Resizes the bit vector to a new number of bits
 * @param bv Bit vector to modify
 * @param new_num_bits Desired number of bits (must be > 0)
 * @return Result code
 *
 * @complexity O(num_bytes) when growing, O(1) when shrinking
 */
ResultCode BitVector_Resize(BitVector *bv, size_t new_num_bits);

/* ============================================================================
 * BITWISE OPERATIONS
 * ============================================================================
 */

/**
 * @brief Performs bitwise AND with another bit vector (bv = bv & other)
 * @param bv Bit vector to modify (left operand)
 * @param other Right operand bit vector
 * @return Result code
 *
 * @complexity O(min(num_bytes))
 */
ResultCode BitVector_And(BitVector *bv, const BitVector *other);

/**
 * @brief Performs bitwise OR with another bit vector (bv = bv | other)
 * @param bv Bit vector to modify (left operand)
 * @param other Right operand bit vector
 * @return Result code
 *
 * @complexity O(max(num_bytes))
 */
ResultCode BitVector_Or(BitVector *bv, const BitVector *other);

/**
 * @brief Performs bitwise XOR with another bit vector (bv = bv ^ other)
 * @param bv Bit vector to modify (left operand)
 * @param other Right operand bit vector
 * @return Result code
 *
 * @complexity O(max(num_bytes))
 */
ResultCode BitVector_Xor(BitVector *bv, const BitVector *other);

/**
 * @brief Performs bitwise NOT (complement) (bv = ~bv)
 * @param bv Bit vector to modify
 * @return Result code
 *
 * @complexity O(num_bytes)
 */
ResultCode BitVector_Not(BitVector *bv);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Counts the number of bits set to 1
 * @param bv Bit vector to examine
 * @return Number of set bits
 *
 * @complexity O(num_bytes)
 */
size_t BitVector_CountSetBits(const BitVector *bv);

/**
 * @brief Counts the number of bits set to 0
 * @param bv Bit vector to examine
 * @return Number of zero bits
 *
 * @complexity O(num_bytes)
 */
size_t BitVector_CountZeroBits(const BitVector *bv);

/**
 * @brief Converts bit vector to binary string representation
 * @param bv Bit vector to convert
 * @param out_string Output pointer for allocated string (caller must free)
 * @return Result code
 *
 * @complexity O(num_bits)
 */
ResultCode BitVector_ToBinaryString(const BitVector *bv, char **out_string);

/**
 * @brief Creates bit vector from binary string
 * @param str Binary string (containing only '0' and '1')
 * @param result Output pointer for new BitVector
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BitVector_FromBinaryString(const char *str, BitVector **result);

/**
 * @brief Prints the bit vector as binary string (debugging)
 * @param bv Bit vector to print
 *
 * @complexity O(num_bits)
 */
void BitVector_Print(const BitVector *bv);

/**
 * @brief Checks if two bit vectors are equal
 * @param a First bit vector
 * @param b Second bit vector
 * @return true if equal, false otherwise
 *
 * @complexity O(num_bytes)
 */
bool BitVector_Equals(const BitVector *a, const BitVector *b);

/**
 * @brief Creates a deep copy of the bit vector
 * @param src Source bit vector
 * @param out_dest Output pointer for destination bit vector
 * @return Result code
 *
 * @complexity O(num_bytes)
 */
ResultCode BitVector_Copy(const BitVector *src, BitVector **out_dest);

#ifdef __cplusplus
}
#endif

#endif /* BIT_VECTOR_H */
