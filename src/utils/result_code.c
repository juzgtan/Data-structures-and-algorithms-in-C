#include "utils/result_code.h"

const char *Result_ErrorMessage(Result r) {
  switch (r) {
  case kSuccess:
    return "Success";

  case kNullParameter:
    return "Null parameter";
  case kInvalidArgument:
    return "Invalid argument";
  case kOutputPointerIsNotNull:
    return "Output pointer is not null";
  case kArgumentOutOfRange:
    return "Argument out of range";
  case kArithmeticOverflow:
    return "Arithmetic overflow";
  case kArithmeticUnderflow:
    return "Arithmetic underflow";
  case kInvalidIndex:
    return "Invalid index";

  case kEmpty:
    return "Empty";
  case kFull:
    return "Full";
  case kNotFound:
    return "Not found";
  case kDuplicate:
    return "Duplicate";
  case kCorruptedArray:
    return "Corrupted array";

  case kFailedMemoryAllocation:
    return "Failed memory allocation";
  case kOverflow:
    return "Overflow";
  case kUnderflow:
    return "Underflow";
  case kDependencyError:
    return "Dependency error";

  case kSecurityError:
    return "Security error";
  case kSystemError:
    return "System error";

  default:
    return "Unknown error";
  }
}