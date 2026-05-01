#ifndef QUEUE_H
#define QUEUE_H

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
 * @brief Queue structure (FIFO - First In, First Out)
 *
 * A dynamic circular queue that grows automatically when needed.
 * Stores pointers to elements (generic).
 */
typedef struct {
  void **data;     /**< Array of element pointers */
  size_t front;    /**< Index of the front element */
  size_t rear;     /**< Index where the next element will be inserted */
  size_t size;     /**< Number of elements currently in the queue */
  size_t capacity; /**< Maximum elements before reallocation */
} Queue;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new queue
 * @param capacity Initial capacity (0 = use default 16)
 * @param result Output pointer to receive the new Queue
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Queue_Create(size_t capacity, Queue **result);

/**
 * @brief Destroys the queue and frees all associated memory
 * @param q Queue to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by queue elements
 * @complexity O(1)
 */
void Queue_Destroy(Queue *q);

/**
 * @brief Removes all elements from the queue
 * @param q Queue to clear
 *
 * @note Does NOT free the data pointed to by queue elements
 * @complexity O(1)
 */
void Queue_Clear(Queue *q);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements in the queue
 * @param q Queue to examine
 * @return Number of elements (0 if q is NULL)
 *
 * @complexity O(1)
 */
size_t Queue_Size(const Queue *q);

/**
 * @brief Returns the current capacity
 * @param q Queue to examine
 * @return Capacity (0 if q is NULL)
 *
 * @complexity O(1)
 */
size_t Queue_Capacity(const Queue *q);

/**
 * @brief Checks if the queue is empty
 * @param q Queue to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool Queue_IsEmpty(const Queue *q);

/**
 * @brief Checks if the queue is full
 * @param q Queue to examine
 * @return true if full, false otherwise
 *
 * @complexity O(1)
 */
bool Queue_IsFull(const Queue *q);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Adds an element to the back of the queue
 * @param q Queue to modify
 * @param value Pointer to value to enqueue
 * @return Result code
 *
 * @complexity O(1) amortized
 */
ResultCode Queue_Enqueue(Queue *q, void *value);

/**
 * @brief Removes and returns the front element
 * @param q Queue to modify
 * @param out_value Output pointer to receive dequeued value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Queue_Dequeue(Queue *q, void **out_value);

/**
 * @brief Returns the front element without removing it
 * @param q Queue to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Queue_Peek(const Queue *q, void **out_value);

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */
