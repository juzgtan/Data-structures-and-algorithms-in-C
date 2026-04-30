#include "data_structures/array/array.h"
#include "utils/result_code.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

/**
 * @brief Ensures the array has enough capacity for 'needed' new elements
 * @param arr Array to check
 * @param needed Number of additional element needed
 * @return Result code
 *
 * Growth strategy:
 * - If capacity = 0 -> grow to 16 (initial capacity)
 * - Otherwise -> double the capacity (growth factor = 2)
 * - Check for overflow before each multiplication
 *
 * Cases handled:
 * - arr == NULL -> kNullParameter
 * - Already enough capacity -> kSuccess
 * - Overflow during multiplication -> kArithmeticOverflow
 * - Array_Reserve failue -> propagate error
 */

static ResultCode _ensure_capacity(Array *arr, size_t needed) {
  /* Step 1: Validate array pointer */
  if (arr == NULL) {
    return kNullParameter;
  }

  /* Step 2: Case 1 - Already enough capacity */
  if (arr->size + needed <= arr->capacity) {
    return kSuccess;
  }

  /* Step 3: Case 2 - Need more capacity - caculate new capacity */
  size_t new_capacity = arr->capacity == 0 ? 16 : arr->capacity;

  /* Step 4: Double until enough (but check overflow eatch time)
   * Example: - Current capacity = 8, need = 20 elements
   * - new_capacity = 8  ->  8 < 20 -> double to 16 (check overflow)
   * - new_capacity = 16 -> 16 < 20 -> double to 16 (check overflow)
   * - new_capacity = 32 -> 32 > 20 -> stop
   */
  while (new_capacity < arr->size + needed) {
    /* Prevent overflow: new_capacity * 2 would exceed SIZE_MAX */
    if (new_capacity > SIZE_MAX / 2) {
      return kArithmeticOverflow;
    }
    new_capacity *= 2;
  }

  /* Step 5: Case 3 - Let Array_Reserve handle the actial memory allocation */
  return Array_Reserve(arr, new_capacity);
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

/**
 * Array_Reserve - Reserves memory for a specific capacity
 *
 * Implememtation flow:
 * 1. Validate array not NULL
 * 2. Ensure new_capacity >= current size
 * 3. Check if capacity already matches (no-op)
 * 4: Check for overflow in multiplication
 * 5: Reallocate data buffer
 * 6: Update capacity on success
 *
 * @param arr Array to modify
 * @param new_capacity Desired capacity
 * @return Return Result Code
 *
 * @complexity O(n) where n is the number of elements (due to reallocation)
 */
ResultCode Array_Reserve(Array *arr, size_t new_capacity) {
  /* Step 1: Validate array pointer */
  if (arr == NULL) {
    return kNullParameter;
  }

  /* Step 2: Cannot shrink below current size */
  if (new_capacity < arr->size) {
    return kInvalidArgument;
  }

  /* Step 3: No change needed */
  if (new_capacity == arr->size) {
    return kSuccess;
  }

  /* Step 4: Check for overflow before allocation
   * Example: new_capacity = 1e9 , item_size = 1e9 -> 1e18 > SIZE_MAX
   * Formular item_size > SIZE_MAX / new_capacity -> overflow
   */

  if (new_capacity > SIZE_MAX / arr->item_size) {
    return kArithmeticOverflow;
  }

  /* Step 5: Reallocate memory */
  void *new_data = realloc(arr->data, new_capacity * arr->item_size);
  if (new_data == NULL && new_capacity > 0) {
    return kFailedMemoryAllocation;
  }

  /* Step 6: Update array state */
  arr->data = new_data;
  arr->capacity = new_capacity;

  return kSuccess;
}

/**
 * Array_ShrinkToFit - Reduces capacity to exactly match size
 *
 * Frees any unused memory - Useful when the array has grows large
 * but then many elements were removed
 *
 * @param arr Array to shrink
 * @return Result code
 *
 * @complexity O(n) where n is the number of elements
 */

ResultCode Array_ShrinkToFit(Array *arr) {
  /* Step 1: Validate arr pointer */
  if (arr == NULL) {
    return kNullParameter;
  }

  /* Step 2: No need shirnk if already at minumum */
  if (arr->size == arr->capacity) {
    return kSuccess;
  }

  /* Step 3: Reverse with capacity = size */
  return Array_Reserve(arr, arr->size);
}

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * Array_Data - Return raw pointer to internal data (non-const)
 *
 * Warning: Return NULL if array is NULL or data is NULL
 * Caller must ensure safe usage of this pointer
 *
 * @param arr Array to ACCESS
 * @return Pointer to data buffer (NULL if arr is NULL)
 *
 * @complexity 0(1)
 */

void *Array_Data(Array *arr) { return arr == NULL ? NULL : arr->data; }

/**
 * Array_DataConst - Return const pointer to internal data (read-only)
 *
 * @param arr Array to access
 * @return Const pointer to data buffer (NULL if arr is NULL)
 *
 * @complexity O(1)
 */

const void *Array_Dataconst(Array *arr) {
  return arr == NULL ? NULL : arr->data;
}

/**
 * Array_Get - Safety retrieves a pointer to element at index
 *
 * Implement flow:
 * 1. Validate array and output pointer not NULL
 * 2. Check index is within bounds
 * 3: Caculate address and assign to output
 *
 * Note: out is const void** to prevent modification through this pointer
 *
 * @param arr Array to access
 * @param index Element index (0-based)
 * @param out Output pointer to receive element address
 * @return Result code
 *
 * @complexity O(1)
 */

ResultCode Array_Get(const Array *arr, size_t index, const void **out) {
  /* Step1: Validate parameters */
  if (arr == NULL || out == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  ResultCode rc = _check_index(arr, index);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Caculate address and assign
   * Address = data + index * item_size
   * Use char* pointer arithimetic (sizeof(char) = 1)
   */
  *out = (const char *)arr->data + index * arr->item_size;

  return kSuccess;
}

/**
 * Array_Set - Assigns a valude to element at index
 *
 * Implementation flow:
 * 1. Validate array and value not NULL
 * 2. Check index is within bounds
 * 3. Copy value into the array's data buffer
 *
 * @param arr Array to modify
 * @param index Element index (0-based)
 * @param value Pointer to value to copy
 * @return Result code
 *
 * @complexity O(1)
 */

ResultCode Array_Set(Array *arr, size_t index, const void *value) {
  /* Step 1: Validate parameters */
  if (arr == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  ResultCode rc = _check_index(arr, index);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Copy value into array
   * Caculate destination address: data + index * item size
   * Use memcpy for byte-by-byte copy
   */
  void *dest = (char *)arr->data + index * arr->item_size;
  memcpy(dest, value, arr->item_size);

  return kSuccess;
}

/**
 * Array_GetUnchecked - Fast, unsafe element access (no bounds checking)
 *
 * WARNING: Use only when index is GUARANTEED to be valid.
 * No validation is performed - results in undefined behavior if index is
 * invalid.
 *
 * Use case: Performance-critical loops where index is already validated.
 *
 * @param arr Array to access (assumed non-NULL)
 * @param index Element index (assumed valid)
 * @return Pointer to element
 *
 * @complexity O(1)
 */
void *Array_GetUnchecked(const Array *arr, size_t index) {
  /* NO CHECKS - UNSAFE! Use with extreme caution */
  return (char *)arr->data + index * arr->item_size;
}

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * Array_PushBack - Appends an element to the end of the array
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Ensure enough capacity (grow if needed)
 * 3. Copy value to the end
 * 4. Increment size
 *
 * @param arr Array to modify
 * @param value Pointer to value to Appends
 * @result Result code
 *
 * @complexity O(1)
 */

ResultCode Array_PushBack(Array *arr, const void *value) {
  /* Step 1: Validate parameters */
  if (arr == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Ensure capacity for one more element */
  ResultCode rc = _ensure_capacity(arr, 1);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Copy value to the end
   * Destination = data + size * item_size (right after last element)
   */
  void *dest = (char *)arr->data + arr->size * arr->item_size;
  memcpy(dest, value, arr->item_size);

  /* Step 4: Increment size */
  arr->size++;

  return kSuccess;
}
