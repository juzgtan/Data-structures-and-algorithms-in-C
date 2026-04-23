#ifndef RESULT_CODE_H
#define RESULT_CODE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Result / Error Code Definition
 * ============================================================================
 */

/*
 * Result codes:
 *   0        : Success
 *   -1xx     : Parameter errors
 *   -2xx     : State errors
 *   -3xx     : Runtime errors
 *   -4xx     : System errors
 */
typedef enum Result {
  /* ===== Success ===== */
  kSuccess = 0,

  /* ===== Parameter Errors (-1xx) ===== */
  kNullParameter = -100,
  kInvalidArgument = -101,
  kOutputPointerIsNotNull = -102,
  kArgumentOutOfRange = -103,
  kArithmeticOverflow = -104,
  kArithmeticUnderflow = -105,
  kInvalidIndex = -106,

  /* ===== State Errors (-2xx) ===== */
  kEmpty = -200,
  kFull = -201,
  kNotFound = -202,
  kDuplicate = -203,
  kCorruptedArray = -204,

  /* ===== Runtime Errors (-3xx) ===== */
  kFailedMemoryAllocation = -300,
  kOverflow = -301,
  kUnderflow = -302,
  kDependencyError = -303,

  /* ===== System Errors (-4xx) ===== */
  kSecurityError = -400,
  kSystemError = -401
} Result;

/* Optional alias for semantic clarity */
typedef Result ResultCode;

/* ============================================================================
 * Utility API
 * ============================================================================
 */

/**
 * @brief Check if result indicates success.
 *
 * @param r Result code
 * @return 1 if success, 0 otherwise
 */
static inline int Result_IsSuccess(Result r) { return r == kSuccess; }

/**
 * @brief Check if result indicates failure.
 *
 * @param r Result code
 * @return 1 if failure, 0 otherwise
 */
static inline int Result_IsFailure(Result r) { return r != kSuccess; }

/**
 * @brief Convert result code to human-readable message.
 *
 * @param r Result code
 * @return Constant string describing the error
 *
 * @note Implementation must not return NULL.
 */
const char *Result_ErrorMessage(Result r);

#ifdef __cplusplus
}
#endif

#endif /* RESULT_CODE_H */