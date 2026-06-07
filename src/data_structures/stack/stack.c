#include "result_code.h"
#include <data_structures/stack/stack.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ============================================================================
 * INTERNAL HELPER FUNCTIONS (Private - not exposed in header)
 * ============================================================================
 */

/**
 * @brief Check if the stack is full
 *
 * @param s Stack to check
 * @return true if full, false othewise
 */
static bool _is_full(const Stack *s) { return s->top == s->capacity; }

/**
 * @brief Ensures the stack has enough capacity
 *
 * Implementation flow:
 * 1. validate parameter
 * 2. Check if Stack already have capacity
 * 3. If not enough capacity -> caculate new capacity
 * 4. Handle case stack overflow
 * 5. Reallocate memory
 *
 * @param s Stack to check
 * @return Result code
 *
 * Growth strategy
 * - If capacity is 0 -> grow to 16
 * - Otherwise -> double the capacity
 * - Check for overflow before each multiplication
 */
static ResultCode _ensure_capacity(Stack *s) {
  /* Step 1: Valicate parameter */
  if (s == NULL) {
    return kNullParameter;
  }

  /* Step 2: Already have capacity */
  if (s->top < s->capacity) {
    return kSuccess;
  }

  /* Step 3: Caculate new capacity */
  size_t new_capacity = s->capacity == 0 ? 16 : s->capacity;

  /* Step 4: Prevent overflow */
  if (new_capacity > SIZE_MAX / 2) {
    return kArithmeticOverflow;
  }

  new_capacity *= 2;

  /* Step 5: Reallocate memory */
  void **new_data = (void **)realloc(s->data, new_capacity * sizeof(void *));
  if (new_data == NULL) {
    return kFailedMemoryAllocation;
  }

  s->data = new_data;
  s->capacity = new_capacity;

  return kSuccess;
}

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * Stack_Create - Creates a new stack
 *
 * Implementation flow:
 * 1. Set output pointer to NULL
 * 2. Validate output parameter
 * 3. Check output pointer not already pointing to valid memory
 * 4. Allocate Stack struc
 * 5. Initialize fields
 * 6. Allocate data buffer (if capacity > 0 )
 * 7. Assign result
 *
 * @param capacity Initial capacity (0 = use default)
 * @param result Output pointer to receive the new stack
 * @return Result Code
 */
ResultCode Stack_Create(size_t capacity, Stack **result) {
  /* Step 1: Set output to NULL */
  if (result != NULL) {
    *result = NULL;
  }

  /* Step 2: Validate output parameter */
  if (result == NULL) {
    return kNullParameter;
  }

  /* Step 3: Prevent memory leak */
  if (*result != NULL) {
    return kOutputPointerIsNotNull;
  }

  /* Step 4: Allocate stack struct */
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (stack == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 5: Initialize fields */
  stack->top = 0;
  stack->capacity = capacity == 0 ? 16 : capacity;
  stack->data = NULL;

  /* Step 6: Allocate data buffer */
  if (stack->capacity > 0) {
    stack->data = (void **)malloc(stack->capacity * sizeof(void *));

    if (stack->data == NULL) {
      free(stack);
      return kFailedMemoryAllocation;
    }
  }

  /* Step 7: Assign result and return */
  *result = stack;

  return kSuccess;
}

/**
 * Stack_Destroy - Frees all memory associated with the stack
 *
 * @param s Stack to destroy (can be NULL)
 */
void Stack_Destroy(Stack *s) {
  if (s == NULL) {
    return;
  }

  if (s->data != NULL) {
    free(s->data);
    s->data = NULL;
  }

  s->top = 0;
  s->capacity = 0;
  free(s);
}

/**
 * Stack_Clear - Removes all elements from the stack
 *
 * @param s Stack to clear
 */
void Stack_Clear(Stack *s) {
  if (s != NULL) {
    s->top = 0;
  }
}

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */
size_t Stack_Size(const Stack *s) { return s == NULL ? 0 : s->top; }
size_t Stack_Capacity(const Stack *s) { return s == NULL ? 0 : s->capacity; }
bool Stack_IsEmpty(const Stack *s) { return s == NULL ? true : s->top == 0; }
bool Stack_IsFull(const Stack *s) { return s == NULL ? true : _is_full(s); }

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * Stack_Push - Pushes an element on to the top of the stack
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Ensure capacity
 * 3. Store value at top position
 * 4. Increment top
 *
 * @param s Stack to modify
 * @param value Pointer to value to push
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Stack_Push(Stack *s, void *value) {
  /* Step 1: Validate parameters */
  if (s == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Ensure capacity */
  ResultCode rc = _ensure_capacity(s);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Push value on top */
  s->data[s->top] = value;

  /* Step 4: Increment top */
  s->top++;

  return kSuccess;
}
