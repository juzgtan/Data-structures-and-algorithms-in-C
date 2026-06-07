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

/**
 * @brief Ensures the queue has enough capacity for one more element
 * @param q Queue to grow if full
 * @return ResultCode
 *
 * Growth strategy:
 * - capacity == 0  -> allocate 16 slots
 * - otherwise      -> double capacity
 *
 * Resize strategy:
 * - Allocate a new buffer
 * - Copy elements in logical queue order
 * - Reset front to 0
 * - Reset rear to size - 1
 *
 * Example:
 *
 * Before resize (capacity = 8):
 *
 *   index:  0  1  2  3  4  5  6  7
 *           A  B  C  D  E  F  G  H
 *                          ^front
 *                       ^rear
 *
 * front = 5
 * rear  = 4
 *
 * Logical queue order:
 *
 *   F -> G -> H -> A -> B -> C -> D -> E
 *
 * After resize (capacity = 16):
 *
 *   index:  0  1  2  3  4  5  6  7
 *           F  G  H  A  B  C  D  E
 *           ^front               ^rear
 *
 * front = 0
 * rear  = size - 1
 */
#define QUEUE_INITIAL_CAPACITY 16
#define QUEUE_GROWTH_FACTOR 2

static ResultCode _ensure_capacity(Queue *q) {
  /* Step 1: Validate quaue pointer */
  if (q == NULL) {
    return kNullParameter;
  }

  /* Step 2: Already have enough capacity, No resize needed */
  if (q->size < q->capacity) {
    return kSuccess;
  }

  /* Step 3: Caculate new capacity
   * Formula: new_capacity = old_capacity * 2 (or 16 if starting from 0)
   */
  size_t new_capacity = q->capacity == 0 ? QUEUE_INITIAL_CAPACITY : q->capacity;

  /* Step 4: Prevent arthimetic overflow
   * EXAMPLE: new_capacity = 1GB, doubling would exceed SIZE_MAX on 32bit
   * Check: new_capacity > SIZE_MAX / 2 -> overflow would occur
   */
  if (new_capacity > SIZE_MAX / QUEUE_GROWTH_FACTOR) {
    return kArithmeticOverflow;
  }
  new_capacity *= QUEUE_GROWTH_FACTOR;

  /* Step 5: Check overflow for multipication with sizeof(void*)
   * new_capacity * sizeof(void*) must not exceed SIZE_MAX
   */
  if (new_capacity > SIZE_MAX / sizeof(void *)) {
    return kArithmeticOverflow;
  }

  /* Step 6: Reallocate memory fo data buffer
   * We intentionaly use malloc instead of realloc because
   * we want to rebuild the queue into a clean contiguous layout
   * regardless of wrap-around state */
  void **new_data = (void **)malloc(new_capacity * sizeof(void *));
  if (new_data == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 7: Copy elements in logical queue order
   * Circular queue physical layout may not match logical FIFO order.
   * Formula: physical_index = (front + i) % old_capacity
   * Examples: capacity = 8, front = 5, size = 8
   * i=0 -> index=5
   * i=1 -> index=6
   * i=2 -> index=7
   * i=3 -> index=0
   * ...
   * This walks through the queue exactly in FIFO order.
   */
  if (q->size > 0) {
    for (size_t i = 0; i < q->size; i++) {
      size_t old_index = (q->front + 1) % q->capacity;
      new_data[i] = q->data[old_index];
    }
  }
  /* Step 8: Release old buffer
   * Safe because all elements have already been copied.
   */
  free(q->data);

  /* Step 9: Update queue metadata */
  q->data = new_data;
  q->capacity = new_capacity;

  /* Step 10: Rebuild queue layout
   * Since elements are now stored contiguously:
   *   [0 .. size-1]
   * front always becomes 0.
   * rear points to the last valid element.
   */
  if (q->size == 0) {
    q->front = 0;
    q->rear = 0;
  } else {
    q->front = 0;
    q->rear = q->size - 1;
  }

  return kSuccess;
}

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

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * Queue_Enqueue - Adds an element to the back of the queue
 *
 * Implementation flow:
 * 1. Validate parameters (queue and value not NULL)
 * 2. Ensuse enough capacity (grow if needed)
 * 3. Store value at rear position
 * 4. Update rear index using modulo arithmetic (wap-around)
 * 5. Increment size
 *
 * Circular buffer mechanics
 * - When rear reaches capacity - 1, next enqueue wraps to index 0
 * EXAMPLE: capacity = 8, rear = 7 , after enqueue: rear = (7 + 1) % 8
 *
 * @param q Queue to modify
 * @param value Pointer to value to enqueue
 * @return Result code

 * @complexity O(1) - amortized
 */
ResultCode Queue_Enqueue(Queue *q, void *value) {
  /* Step 1: Validate parameters */
  if (q == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Ensure capacity for one more element
   * This may triger reallocation and buffer reorganization
   */
  ResultCode rc = _ensure_capacity(q);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Store value at rear position
   * q->rear points to the next available slot
   */
  q->data[q->rear] = value;

  /* Step 4: Update rear index (circular)
   * Formular: rear = (rear + 1 ) % capacity
   * This wraps around to 0 when reaching the end
   */
  q->rear = (q->rear + 1) % q->capacity;

  /* Step 5: Increment size */
  q->size++;

  return kSuccess;
}

/**
 * Queue_Dequeue - Removes and returns the front element
 *
 * Implementation flow:
 * 1. Validate parameters (queue and output pointer or not NULL)
 * 2. Check queue is not empty
 * 3. Retrieve value from front position
 * 4. Update front index using modulo arthimetic (wrap-around)
 * 5. Decrement size
 * 6. Set output pointer
 *
 * NOTE: Does NOT free the memory pointed to by the value.
 * Caller is reponsible for freeing dynamically allocated data.
 *
 * @param q Queue to modify
 * @param out_value Output pointer to receive dequeued value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Queue_Dequeue(Queue *q, void **out_value) {
  /* Step 1: Validate parameters */
  if (q == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check queue not empty */
  if (Queue_IsEmpty(q)) {
    return kEmpty;
  }

  /* Step 3:  Retrieve value from front position */
  *out_value = q->data[q->front];

  /* Step 4: Update front index (circular)
   * Formula: front = (front + 1) % capacity
   * This wrap around to 0 when reaching the end
   */
  q->front = (q->front + 1) % q->capacity;

  /* Step 5: Decrement size */
  q->size--;

  return kSuccess;
}

/**
 * Queue_Peek - Returns the front element without removing it
 *
 * Implementation flow:
 * 1. Validate parameters (queue, and output pointer not NULL)
 * 2. Check queue is not empty
 * 3. Retrieve value from front position
 * 4. Set output pointer
 *
 * @param q Queue to access
 * @param out_value Output pointer to receive front value
 * @return Result code

 * @complexity O(1)
 */
ResultCode Queue_Peek(const Queue *q, void **out_value) {
  /* Step 1: Validate parameters */
  if (q == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check queue is not empty */
  if (Queue_IsEmpty(q)) {
    return kEmpty;
  }

  /* Step 3: Retrieve value fromt front position */
  *out_value = q->data[q->front];

  return kSuccess;
}
