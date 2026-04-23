#ifndef ARRAY_H
#define ARRAY_H

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
 * @brief Dynamic array structure
 *
 * A generic dynamic array that automatically grows when needed.
 * Stores elements of fixed size in contiguous memory.
 */
typedef struct {
  size_t size;      /**< Number of elements currently in the array */
  size_t capacity;  /**< Maximum elements before reallocation */
  size_t item_size; /**< Size of each element in bytes */
  void *data;       /**< Pointer to the underlying data buffer */
} Array;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new dynamic array
 * @param item_size Size of each element in bytes (must be > 0)
 * @param result Output pointer to receive the new Array
 * @return Result code
 *
 * @complexity O(1)
 *
 * Example:
 * @code
 * Array *arr;
 * if (Array_Create(sizeof(int), &arr) == kSuccess) {
 *     // Use array
 * }
 * @endcode
 */
ResultCode Array_Create(size_t item_size, Array **result);

/**
 * @brief Destroys the array and frees all associated memory
 * @param arr Array to destroy (can be NULL)
 *
 * @complexity O(1) for the array itself (elements are not freed)
 */
void Array_Destroy(Array *arr);

/**
 * @brief Removes all elements but keeps the capacity
 * @param arr Array to clear
 *
 * @complexity O(1)
 */
void Array_Clear(Array *arr);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements currently in the array
 * @param arr Array to examine
 * @return Number of elements (0 if arr is NULL)
 *
 * @complexity O(1)
 */
size_t Array_Size(const Array *arr);

/**
 * @brief Returns the current capacity (maximum elements before reallocation)
 * @param arr Array to examine
 * @return Capacity (0 if arr is NULL)
 *
 * @complexity O(1)
 */
size_t Array_Capacity(const Array *arr);

/**
 * @brief Checks if the array contains any elements
 * @param arr Array to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool Array_IsEmpty(const Array *arr);

/**
 * @brief Reserves memory for at least new_capacity elements
 * @param arr Array to modify
 * @param new_capacity Desired capacity (must be >= current size)
 * @return Result code
 *
 * @complexity O(n) where n is the number of elements (due to reallocation)
 */
ResultCode Array_Reserve(Array *arr, size_t new_capacity);

/**
 * @brief Reduces capacity to exactly match the current size
 * @param arr Array to modify
 * @return Result code
 *
 * @complexity O(n) where n is the number of elements
 */
ResultCode Array_ShrinkToFit(Array *arr);

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns a pointer to the internal data buffer (non-const)
 * @param arr Array to access
 * @return Pointer to data buffer (NULL if arr is NULL)
 *
 * @warning Direct access may bypass bounds checking
 * @complexity O(1)
 */
void *Array_Data(Array *arr);

/**
 * @brief Returns a const pointer to the internal data buffer (read-only)
 * @param arr Array to access
 * @return Const pointer to data buffer (NULL if arr is NULL)
 *
 * @complexity O(1)
 */
const void *Array_DataConst(const Array *arr);

/**
 * @brief Safely retrieves a pointer to element at index
 * @param arr Array to access
 * @param index Element index (0-based)
 * @param out Output pointer to receive element address
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Array_Get(const Array *arr, size_t index, const void **out);

/**
 * @brief Safely assigns a value to element at index
 * @param arr Array to modify
 * @param index Element index (0-based)
 * @param value Pointer to value to copy
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Array_Set(Array *arr, size_t index, const void *value);

/**
 * @brief Fast, unsafe element access (no bounds checking)
 * @param arr Array to access (assumed non-NULL)
 * @param index Element index (assumed valid)
 * @return Pointer to element
 *
 * @warning Use only when index is GUARANTEED to be valid
 * @complexity O(1)
 */
void *Array_GetUnchecked(const Array *arr, size_t index);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Appends an element to the end of the array
 * @param arr Array to modify
 * @param value Pointer to value to append
 * @return Result code
 *
 * @complexity O(1) amortized
 */
ResultCode Array_PushBack(Array *arr, const void *value);

/**
 * @brief Removes the last element from the array
 * @param arr Array to modify
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Array_PopBack(Array *arr);

/**
 * @brief Inserts an element at the specified index
 * @param arr Array to modify
 * @param index Position to insert at (0 <= index <= size)
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(n) where n = size - index
 */
ResultCode Array_Insert(Array *arr, size_t index, const void *value);

/**
 * @brief Removes an element at the specified index
 * @param arr Array to modify
 * @param index Position to remove (0 <= index < size)
 * @return Result code
 *
 * @complexity O(n) where n = size - index - 1
 */
ResultCode Array_Remove(Array *arr, size_t index);

#ifdef __cplusplus
}
#endif

#endif /* ARRAY_H */