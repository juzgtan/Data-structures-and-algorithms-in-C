#include "result_code.h"
#include <data_structures/stack/stack.h>
#include <stddef.h>
#include <stdlib.h>

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
