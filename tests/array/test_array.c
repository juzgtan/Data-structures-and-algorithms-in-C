#include "data_structures/array/array.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST_COLOR_RED "\033[31m"
#define TEST_COLOR_GREEN "\033[32m"
#define TEST_COLOR_YELLOW "\033[33m"
#define TEST_COLOR_BLUE "\033[34m"
#define TEST_COLOR_RESET "\033[0m"

static int tests_passed = 0;
static int tests_failed = 0;
static int tests_run = 0;

#define TEST_START()                                                           \
  printf("%s========================================%s\n", TEST_COLOR_BLUE,    \
         TEST_COLOR_RESET);                                                    \
  printf("%s           Running Array Tests%s\n", TEST_COLOR_YELLOW,            \
         TEST_COLOR_RESET);                                                    \
  printf("%s========================================%s\n", TEST_COLOR_BLUE,    \
         TEST_COLOR_RESET);                                                    \
  tests_passed = 0;                                                            \
  tests_failed = 0;                                                            \
  tests_run = 0;

#define TEST_END()                                                             \
  printf("%s========================================%s\n", TEST_COLOR_BLUE,    \
         TEST_COLOR_RESET);                                                    \
  printf("%sResults: %d passed, %d failed, %d total%s\n",                      \
         tests_failed == 0 ? TEST_COLOR_GREEN : TEST_COLOR_RED, tests_passed,  \
         tests_failed, tests_run, TEST_COLOR_RESET);                           \
  printf("%s========================================%s\n", TEST_COLOR_BLUE,    \
         TEST_COLOR_RESET);                                                    \
  return tests_failed == 0 ? 0 : 1;

#define TEST_CASE(name) void test_##name(void)

#define RUN_TEST(name)                                                         \
  do {                                                                         \
    printf("%s[ RUN      ]%s %s\n", TEST_COLOR_YELLOW, TEST_COLOR_RESET,       \
           #name);                                                             \
    tests_run++;                                                               \
    test_##name();                                                             \
    printf("%s[       OK ]%s %s\n", TEST_COLOR_GREEN, TEST_COLOR_RESET,        \
           #name);                                                             \
    tests_passed++;                                                            \
  } while (0)

#define ASSERT_TRUE(expr)                                                      \
  do {                                                                         \
    if (!(expr)) {                                                             \
      printf("%s[  FAILED  ]%s %s:%d: Assertion failed: %s\n", TEST_COLOR_RED, \
             TEST_COLOR_RESET, __FILE__, __LINE__, #expr);                     \
      tests_failed++;                                                          \
      tests_run--;                                                             \
      return;                                                                  \
    }                                                                          \
  } while (0)

/* Fix macro ASSERT_EQUAL to handle diff types */
#define ASSERT_EQUAL(expected, actual)                                         \
  do {                                                                         \
    if ((expected) != (actual)) {                                              \
      printf("%s[  FAILED  ]%s %s:%d: Expected: %ld, Got: %ld\n",              \
             TEST_COLOR_RED, TEST_COLOR_RESET, __FILE__, __LINE__,             \
             (long)(expected), (long)(actual));                                \
      tests_failed++;                                                          \
      tests_run--;                                                             \
      return;                                                                  \
    }                                                                          \
  } while (0)

#define ASSERT_NOT_NULL(ptr) ASSERT_TRUE((ptr) != NULL)
#define ASSERT_RESULT_OK(rc) ASSERT_EQUAL(rc, 0)

/* ============================================================================
 * Test Cases
 * ============================================================================
 */

/* ============================================================================
 * Main
 * ============================================================================
 */

int main(void) {
  TEST_START();

  /*/ Tests here  */

  TEST_END();
}
