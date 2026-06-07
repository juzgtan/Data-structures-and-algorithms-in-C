#include "data_structures/queue/queue.h"
#include "result_code.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ============================================================================
 * INTERNAL HELPER FUNCTIONS (Private - not exposed in header)
 * ============================================================================
 */

/**
 * @brief Check if the queue if full
 *
 * @param q Queue to check
 * @return true if full, false otherwise
 */
static bool _is_full(const Queue *q) { return q->size == q->capacity; }

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * Queue_Create - Creates a new Queue
 *
 * Implementation flow:
 * 1. Set output pointer to NULL (safety first)
 * 2. Validate all input parameters
 * 3. Check output pointer is not pointing to valid memory (prevent leak)
 * 4. Allocate Queue struct
 * 5. Initialize struct fields with default values
 * 6. Allocate data buffer with initial capacity
 * 7. Assign result and return success
 *
 * @param capacity Initial capacity (0  = use default 16)
 * @param result Output pointer to receive the new Queue
 * @return Result code
 */
ResultCode Queue_Create(size_t capacity, Queue **result) {
  /* Step 1: Always set output to NULL first */
  if (result != NULL) {
    *result = NULL;
  }

  /* Step 2: Validate output parameter */
  if (result == NULL) {
    return kNullParameter;
  }

  /* Step 3: Check output pointer is not already pointing to valid memory
    This prenent memory leaks if caller passes an already-allocated pointer
  */
  if (*result != NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 4: Allocate Queue struct */
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (queue == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 5: Initialize struct fields */
  queue->front = 0; /* Index of first elenemt */
  queue->rear = 0;  /* Index where next elemebt will be inserted */
  queue->size = 0;  /* Number of elements currently in queue */
  queue->capacity = capacity == 0 ? 16 : capacity; /* Defaule capacoty 16 */
  queue->data = NULL;

  /* Step 6: Allocate data buffer
   * Check for overflow: capacity * sizeof(void*) might overflow SIZE_MAX
   */
  if (queue->capacity > 0) {
    if (queue->capacity > SIZE_MAX / sizeof(void *)) {
      free(queue);
      return kArithmeticOverflow;
    }
    queue->data = (void **)malloc(queue->capacity * sizeof(void *));
    if (queue->data == NULL) {
      free(queue);
      return kFailedMemoryAllocation;
    }
  }

  /* Step 7: Set output and return */
  *result = queue;

  return kSuccess;
}

/**
 * Queue_Destroy - Free all memory assiciated with the queue
 *
 * Implementation flow:
 * 1. Check if queue is NULL (Silently return)
 * 2. Free the data buffer if it exists
 * 3. Reset all fields to zero (helps detect use-after-free bugs)
 * 4. Free the Queue struct ifself
 *
 * @param q Queue to destroy (can be NULL)
 */
void Queue_Destroy(Queue *q) {
  /* Step 1: Handle NUll gracefully */
  if (q == NULL) {
    return;
  }

  /* Step 2: Free data buffer if allocated */
  if (q->data != NULL) {
    free(q->data);
    q->data = NULL; /* Prevent dangling pointer */
  }

  /* Step 3: Reset fields (helps debug use-after-free issues) */
  q->front = 0;
  q->rear = 0;
  q->size = 0;
  q->capacity = 0;

  /* Step 4: Free the struct ifself */
  free(q);
}

/**
 * Queue_Clear - Removes all elements but keeps the capacity
 *
 * Simply resets front, read, and size to 0. The data buffer remains allocated
 * to avoid reallocation when elements are added again.
 *
 * NOTE: Does NOT free individual elements (Caller is reponsible for
 * freeing any dynamically allocated data stored in the queue)
 *
 * @param q Queue to clear
 */
void Queue_Clear(Queue *q) {
  if (q != NULL) {
    q->front = 0;
    q->rear = 0;
    q->size = 0;
  }
}

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * Queue_Size - Return the number of elements currently in the queue
 *
 * Return 0 if queue if NULL (safe behavior)
 */
size_t Queue_Size(const Queue *q) { return q == NULL ? 0 : q->size; }

/**
 * Queue_Capacity - Returns the current capacity (maximum elements before
 * reallocation)
 * Return 0 if  queue is NULL
 */
size_t Queue_Capacity(const Queue *q) { return q == NULL ? 0 : q->capacity; }

/**
 * Queue_IsEmpty - Checks if the queue contains any elements
 * Return true if queue is NULL of empty
 */
bool Queue_IsEmpty(const Queue *q) { return q == NULL ? true : q->size == 0; }

/**
 * Queue_IsFull - Checks if the queue has reacher its capacity
 * Return true if queue if NULL or full
 */
bool Queue_IsFull(const Queue *q) { return q == NULL ? true : _is_full(q); }
