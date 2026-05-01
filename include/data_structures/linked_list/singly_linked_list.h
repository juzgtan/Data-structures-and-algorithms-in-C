#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

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
 * @brief Singly linked list node
 */
typedef struct SListNode {
  void *data;             /**< Pointer to stored data */
  struct SListNode *next; /**< Pointer to next node */
} SListNode;

/**
 * @brief Singly linked list structure
 *
 * A singly linked list with head pointer for forward-only traversal.
 * Each node contains data and a pointer to the next node.
 *
 * Advantages:
 * - Less memory than doubly linked list (no prev pointer)
 * - Simpler implementation
 *
 * Disadvantages:
 * - Cannot traverse backwards
 * - Cannot easily delete a node without knowing its previous node
 */
typedef struct {
  size_t size;     /**< Number of elements in the list */
  SListNode *head; /**< Pointer to the first node */
  SListNode
      *tail; /**< Pointer to the last node (optional, for O(1) push_back) */
} SinglyLinkedList;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new empty singly linked list
 * @param result Output pointer to receive the new SinglyLinkedList
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode SinglyLinkedList_Create(SinglyLinkedList **result);

/**
 * @brief Destroys the singly linked list and frees all associated memory
 * @param list Singly linked list to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void SinglyLinkedList_Destroy(SinglyLinkedList *list);

/**
 * @brief Removes all nodes from the list
 * @param list Singly linked list to clear
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void SinglyLinkedList_Clear(SinglyLinkedList *list);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements in the list
 * @param list Singly linked list to examine
 * @return Number of elements (0 if list is NULL)
 *
 * @complexity O(1)
 */
size_t SinglyLinkedList_Size(const SinglyLinkedList *list);

/**
 * @brief Checks if the list is empty
 * @param list Singly linked list to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool SinglyLinkedList_IsEmpty(const SinglyLinkedList *list);

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the first element without removing it
 * @param list Singly linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode SinglyLinkedList_Front(const SinglyLinkedList *list,
                                  void **out_value);

/**
 * @brief Returns the last element without removing it
 * @param list Singly linked list to access
 * @param out_value Output pointer to receive back value
 * @return Result code
 *
 * @complexity O(1) if tail pointer maintained, O(n) otherwise
 */
ResultCode SinglyLinkedList_Back(const SinglyLinkedList *list,
                                 void **out_value);

/**
 * @brief Returns element at specific index without removing it
 * @param list Singly linked list to access
 * @param index Position to retrieve (0-based)
 * @param out_value Output pointer to receive value
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode SinglyLinkedList_GetAt(const SinglyLinkedList *list, size_t index,
                                  void **out_value);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Inserts an element at the beginning of the list
 * @param list Singly linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode SinglyLinkedList_PushFront(SinglyLinkedList *list, void *value);

/**
 * @brief Inserts an element at the end of the list
 * @param list Singly linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(1) if tail pointer maintained, O(n) otherwise
 */
ResultCode SinglyLinkedList_PushBack(SinglyLinkedList *list, void *value);

/**
 * @brief Removes the first element from the list
 * @param list Singly linked list to modify
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode SinglyLinkedList_PopFront(SinglyLinkedList *list);

/**
 * @brief Removes the last element from the list
 * @param list Singly linked list to modify
 * @return Result code
 *
 * @complexity O(n) (need to find second-to-last node)
 */
ResultCode SinglyLinkedList_PopBack(SinglyLinkedList *list);

/**
 * @brief Inserts an element at the specified index
 * @param list Singly linked list to modify
 * @param index Position to insert at (0 <= index <= size)
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode SinglyLinkedList_InsertAt(SinglyLinkedList *list, size_t index,
                                     void *value);

/**
 * @brief Removes an element at the specified index
 * @param list Singly linked list to modify
 * @param index Position to remove (0 <= index < size)
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode SinglyLinkedList_RemoveAt(SinglyLinkedList *list, size_t index);

/* ============================================================================
 * ADVANCED OPERATIONS
 * ============================================================================
 */

/**
 * @brief Reverses the singly linked list in-place
 * @param list Singly linked list to reverse
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode SinglyLinkedList_Reverse(SinglyLinkedList *list);

/**
 * @brief Finds the middle node of the list (using slow/fast pointer technique)
 * @param list Singly linked list to examine
 * @param out_value Output pointer to receive middle node's data
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode SinglyLinkedList_FindMiddle(const SinglyLinkedList *list,
                                       void **out_value);

/**
 * @brief Detects if the list has a cycle (Floyd's cycle detection)
 * @param list Singly linked list to check
 * @return true if cycle exists, false otherwise
 *
 * @complexity O(n)
 */
bool SinglyLinkedList_HasCycle(const SinglyLinkedList *list);

#ifdef __cplusplus
}
#endif

#endif /* SINGLY_LINKED_LIST_H */
