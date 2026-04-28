#include "data_structures/array/array.h"
#include "utils/result_code.h"
#include <stdint.h>
#include <stdlib.h>

/* ============================================================================
 * INTERNAL HELPER FUNCTIONS (Private - not exposed in header)
 * ============================================================================
 */

/**
 * @brief Validates if the given index is within bounds
 * @param arr Array to check
 * @param index Index to validate
 * @return kSuccess if valid, appropriate error code otherwise
 *
 * Cases handled:
 * - arr == NULL -> kNullParameter
 * - index >= size -> kInvalidIndex
 * - otherwise -> kSuccess
 */

static ResultCode _check_index(const Array *arr, size_t index) {
  /* Step 1: Check if array is NULL */
  if (arr == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check if index is within bounds */
  if (index >= arr->size) {
    return kInvalidIndex;
  }

  /* Step 3: Return if index is valid */
  return kSuccess;
}

/* ===========================================================================
 * LIFECYCLE FUNCTIONS
 * ===========================================================================
 */

/**
 * Array_Create - Create a new dynamic array
 *
 * Implement flow
 * 1. Set output pointer to NULL (safety first)
 * 2. Validate all input parameters
 * 3. Check output pointer is not poiting to valid memory (prement leak)
 * 4. Allocate Array Struct
 * 5. Initialize struct fields with default value
 * 6. Check for overflow before allocating data
 * 7. Allocate data buffer with initial capacity
 * 8. Assign and return success
 *
 * @param item_size for each element in bytes
 * @param result Output pointer to receive the new array
 * @return Result code
 *
 * @complecity )(1)
 */

ResultCode Array_Create(size_t item_size, Array **result) {
  /* Step 1: Always set output to NULL first */
  if (result != NULL) {
    *result = NULL;
  }

  /* Step 2: Validate output parameter */
  if (result == NULL) {
    return kNullParameter;
  }

  /* Step 3: validate item_size */
  if (item_size == 0) {
    return kInvalidArgument;
  }

  /* Step 4: Check output pointer is not already poiting to valid memory
   * This prement memory leak if caller passer an already allocated pointer
   */
  if (*result != NULL) {
    return kOutputPointerIsNotNull;
  }

  /* Step 5: Alloate Array struct */
  Array *arr = (Array *)malloc(sizeof(Array));
  if (arr == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 6: Initialize struc fields */
  arr->size = 0;
  arr->capacity = 16;
  arr->item_size = item_size;
  arr->data = NULL;

  /* Step 7: Check for overdlow before allocating data
   * Example: capacity = 16, item_size = 1GB -> 16GB might overflow SIZE_MAX
   * Formula: item_size > SIZE_MAX / capacity -> overflow will occur
   */
  if (arr->capacity > SIZE_MAX / item_size) {
    free(arr);
    return kArithmeticOverflow;
  }

  /* Step 8: Allocate data buffer */
  arr->data = malloc(arr->capacity * item_size);
  if (arr->data == NULL) {
    free(arr);
    return kFailedMemoryAllocation;
  }

  /* Step 9: Success - set output and return */
  *result = arr;
  return kSuccess;
}
/*
 * Array_Destroys - Frees all memory assosiated with the array
 *
 * Implementation flow:
 * 1. Check if array is NULL (siletly return)
 * 2. Free the data buffer is it exitsts
 * 3. Reset all fields to zero (hekp detect use-after-free bugs)
 * 4. Free the array strcut itself
 *
 * @param arr Array to destroy (can be NULL)
 *
 * @complexity (O(1))
 */

void Array_Destroy(Array *arr) {
  /* Step 1: Handle NULL gracefully */
  if (arr == NULL) {
    return;
  }

  /* Step 2: Free data buffer if allocated */
  if (arr->data != NULL) {
    free(arr->data);
    arr->data = NULL; // Prevent dangling pointer
  }

  /* Step 3: Reset fields (help debug use-affter-free issues )*/
  arr->size = 0;
  arr->capacity = 0;
  arr->item_size = 0;

  /* Step 4: Free the struct itself */
  free(arr);
}

/**
 * Array_Clear - Removes all elements but keeps the capacity
 *
 * Simply reserts size to 0. The data buffer remains allocated
 * to avoid rellocation when elements are added again
 *
 * @param arr Array to clean
 *
 * @complexitIy(01)
 */
void Array_Clear(Array *arr) {
  if (arr != NULL) {
    arr->size = 0;
  }
}
/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * Array_Size - Return numbers of elements current in the array
 * Return 0 if array is NULL (safe behavior)
 *
 * @complecity O(1)
 */
size_t Array_Size(const Array *arr) { return arr == NULL ? 0 : arr->size; }

/**
 * Array_Capacity - Return the current capacity
 * Return 0 if array is NULL
 *
 * @complecity O(1)
 */
size_t Array_Capacity(const Array *arr) {
  return arr == NULL ? 0 : arr->capacity;
}

/**
 * Array_IsEmpty - Check if array contains any elements
 * Return true is array is NULL or Array_IsEmpty
 *
 * @complecity O(1)
 */
bool Array_IsEmpty(const Array *arr) { return arr == NULL ? true : arr->size; }
