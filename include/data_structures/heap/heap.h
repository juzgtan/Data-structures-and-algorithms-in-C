#ifndef HEAP_H
#define HEAP_H

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
 * @brief Binary heap structure (array-based)
 * 
 * Can be configured as min-heap or max-heap.
 * Automatically grows when needed.
 */
typedef struct {
  void **data;                              /**< Array of element pointers */
  size_t size;                              /**< Number of elements in the heap */
  size_t capacity;                          /**< Maximum elements before reallocation */
  bool is_min_heap;                         /**< true = min-heap, false = max-heap */
  int (*compare)(const void *, const void *); /**< Element comparison function */
} Heap;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new empty heap
 * @param capacity Initial capacity (0 = use default 16)
 * @param is_min_heap true for min-heap, false for max-heap
 * @param result Output pointer to receive the new Heap
 * @return Result code
 * 
 * @complexity O(1)
 */
ResultCode Heap_Create(size_t capacity, bool is_min_heap, Heap **result);

/**
 * @brief Destroys the heap and frees all associated memory
 * @param h Heap to destroy (can be NULL)
 * 
 * @note Does NOT free the data pointed to by heap elements
 * @complexity O(1)
 */
void Heap_Destroy(Heap *h);

/**
 * @brief Removes all elements from the heap
 * @param h Heap to clear
 * 
 * @note Does NOT free the data pointed to by heap elements
 * @complexity O(1)
 */
void Heap_Clear(Heap *h);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements in the heap
 * @param h Heap to examine
 * @return Number of elements (0 if h is NULL)
 * 
 * @complexity O(1)
 */
size_t Heap_Size(const Heap *h);

/**
 * @brief Returns the current capacity
 * @param h Heap to examine
 * @return Capacity (0 if h is NULL)
 * 
 * @complexity O(1)
 */
size_t Heap_Capacity(const Heap *h);

/**
 * @brief Checks if the heap is empty
 * @param h Heap to examine
 * @return true if empty or NULL, false otherwise
 * 
 * @complexity O(1)
 */
bool Heap_IsEmpty(const Heap *h);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Inserts an element into the heap
 * @param h Heap to modify
 * @param element Pointer to element to insert
 * @return Result code
 * 
 * @complexity O(log n)
 */
ResultCode Heap_Push(Heap *h, void *element);

/**
 * @brief Removes and returns the root element (min or max)
 * @param h Heap to modify
 * @param out_element Output pointer to receive popped element
 * @return Result code
 * 
 * @complexity O(log n)
 */
ResultCode Heap_Pop(Heap *h, void **out_element);

/**
 * @brief Returns the root element without removing it
 * @param h Heap to access
 * @param out_element Output pointer to receive root element
 * @return Result code
 * 
 * @complexity O(1)
 */
ResultCode Heap_Peek(const Heap *h, void **out_element);

/**
 * @brief Builds a heap from an existing array (Floyd's algorithm)
 * @param h Heap to build (will be cleared)
 * @param arr Array of element pointers
 * @param n Number of elements in the array
 * @return Result code
 * 
 * @complexity O(n)
 */
ResultCode Heap_Build(Heap *h, void **arr, size_t n);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Sets a custom comparison function for heap elements
 * @param h Heap to modify
 * @param compare Comparison function (NULL resets to default int comparison)
 * 
 * The comparison function should return:
 * - Negative if a < b
 * - Zero if a == b
 * - Positive if a > b
 */
void Heap_SetCompare(Heap *h, int (*compare)(const void *, const void *));

#ifdef __cplusplus
}
#endif

#endif /* HEAP_H */