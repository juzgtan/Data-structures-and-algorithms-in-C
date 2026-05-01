#ifndef STACK_H
#define STACK_H

#include "utils/result_code.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * STRUCTURES
 * ============================================================================
 */

/**
 * @brief Stack structure (LIFO - Last In, First Out)
 *
 * A dynamic stack that grows automatically when needed.
 * Stores pointers to elements (generic).
 */
typedef struct {
  void **data;     /**< Array of element pointers */
  size_t top;      /**< Index of next empty slot (also equals size) */
  size_t capacity; /**< Maximum elements before reallocation */
} Stack;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new stack
 * @param capacity Initial capacity (0 = use default 16)
 * @param result Output pointer to receive the new Stack
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Stack_Create(size_t capacity, Stack **result);

/**
 * @brief Destroys the stack and frees all associated memory
 * @param s Stack to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by stack elements
 * @complexity O(1)
 */
void Stack_Destroy(Stack *s);

/**
 * @brief Removes all elements from the stack
 * @param s Stack to clear
 *
 * @note Does NOT free the data pointed to by stack elements
 * @complexity O(1)
 */
void Stack_Clear(Stack *s);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements in the stack
 * @param s Stack to examine
 * @return Number of elements (0 if s is NULL)
 *
 * @complexity O(1)
 */
size_t Stack_Size(const Stack *s);

/**
 * @brief Returns the current capacity
 * @param s Stack to examine
 * @return Capacity (0 if s is NULL)
 *
 * @complexity O(1)
 */
size_t Stack_Capacity(const Stack *s);

/**
 * @brief Checks if the stack is empty
 * @param s Stack to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool Stack_IsEmpty(const Stack *s);

/**
 * @brief Checks if the stack is full
 * @param s Stack to examine
 * @return true if full, false otherwise
 *
 * @complexity O(1)
 */
bool Stack_IsFull(const Stack *s);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Pushes an element onto the top of the stack
 * @param s Stack to modify
 * @param value Pointer to value to push
 * @return Result code
 *
 * @complexity O(1) amortized
 */
ResultCode Stack_Push(Stack *s, void *value);

/**
 * @brief Removes and returns the top element
 * @param s Stack to modify
 * @param out_value Output pointer to receive popped value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Stack_Pop(Stack *s, void **out_value);

/**
 * @brief Returns the top element without removing it
 * @param s Stack to access
 * @param out_value Output pointer to receive top value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode Stack_Peek(const Stack *s, void **out_value);

#ifdef __cplusplus
}
#endif

#endif /* STACK_H */
