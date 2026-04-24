#include "data_structures/array/array.h"
#include "utils/result_code.h"

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
