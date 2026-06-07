#include "data_structures/queue/queue.h"
#include "result_code.h"
#include <stdint.h>
#include <stdlib.h>

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
