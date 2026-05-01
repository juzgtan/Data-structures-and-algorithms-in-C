
#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

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
 * @brief Doubly linked list node
 */
typedef struct DListNode {
  void *data;             /**< Pointer to stored data */
  struct DListNode *next; /**< Pointer to next node */
  struct DListNode *prev; /**< Pointer to previous node */
} DListNode;

/**
 * @brief Doubly linked list structure
 *
 * A doubly linked list with head and tail pointers for O(1) access
 * to both ends. Each node contains data and pointers to both next and
 * previous nodes.
 *
 * Advantages:
 * - Can traverse both forward and backward
 * - O(1) deletion of a node when node pointer is known
 * - O(1) access to both head and tail
 *
 * Disadvantages:
 * - More memory per node (extra prev pointer)
 * - Slightly more complex implementation
 */
typedef struct {
  size_t size;     /**< Number of elements in the list */
  DListNode *head; /**< Pointer to the first node */
  DListNode *tail; /**< Pointer to the last node */
} DoublyLinkedList;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new empty doubly linked list
 * @param result Output pointer to receive the new DoublyLinkedList
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_Create(DoublyLinkedList **result);

/**
 * @brief Destroys the doubly linked list and frees all associated memory
 * @param list Doubly linked list to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void DoublyLinkedList_Destroy(DoublyLinkedList *list);

/**
 * @brief Removes all nodes from the list
 * @param list Doubly linked list to clear
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void DoublyLinkedList_Clear(DoublyLinkedList *list);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements in the list
 * @param list Doubly linked list to examine
 * @return Number of elements (0 if list is NULL)
 *
 * @complexity O(1)
 */
size_t DoublyLinkedList_Size(const DoublyLinkedList *list);

/**
 * @brief Checks if the list is empty
 * @param list Doubly linked list to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool DoublyLinkedList_IsEmpty(const DoublyLinkedList *list);

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the first element without removing it
 * @param list Doubly linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_Front(const DoublyLinkedList *list,
                                  void **out_value);

/**
 * @brief Returns the last element without removing it
 * @param list Doubly linked list to access
 * @param out_value Output pointer to receive back value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_Back(const DoublyLinkedList *list,
                                 void **out_value);

/**
 * @brief Returns element at specific index without removing it
 * @param list Doubly linked list to access
 * @param index Position to retrieve (0-based)
 * @param out_value Output pointer to receive value
 * @return Result code
 *
 * Optimizes by traversing from the closer end.
 *
 * @complexity O(n/2) average, O(n) worst case
 */
ResultCode DoublyLinkedList_GetAt(const DoublyLinkedList *list, size_t index,
                                  void **out_value);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Inserts an element at the beginning of the list
 * @param list Doubly linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_PushFront(DoublyLinkedList *list, void *value);

/**
 * @brief Inserts an element at the end of the list
 * @param list Doubly linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_PushBack(DoublyLinkedList *list, void *value);

/**
 * @brief Removes the first element from the list
 * @param list Doubly linked list to modify
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_PopFront(DoublyLinkedList *list);

/**
 * @brief Removes the last element from the list
 * @param list Doubly linked list to modify
 * @return Result code
 *
 * @complexity O(1) (due to tail pointer)
 */
ResultCode DoublyLinkedList_PopBack(DoublyLinkedList *list);

/**
 * @brief Inserts an element at the specified index
 * @param list Doubly linked list to modify
 * @param index Position to insert at (0 <= index <= size)
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode DoublyLinkedList_InsertAt(DoublyLinkedList *list, size_t index,
                                     void *value);

/**
 * @brief Removes an element at the specified index
 * @param list Doubly linked list to modify
 * @param index Position to remove (0 <= index < size)
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode DoublyLinkedList_RemoveAt(DoublyLinkedList *list, size_t index);

/* ============================================================================
 * ADVANCED OPERATIONS
 * ============================================================================
 */

/**
 * @brief Removes a specific node from the list (given node pointer)
 * @param list Doubly linked list to modify
 * @param node Node to remove (must be in the list)
 * @return Result code
 *
 * This is O(1) in doubly linked lists because we have access to prev.
 * In singly linked lists, this would be O(n).
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_RemoveNode(DoublyLinkedList *list, DListNode *node);

/**
 * @brief Inserts a node after a given node
 * @param list Doubly linked list to modify
 * @param node Node after which to insert (must be in the list)
 * @param value Pointer to value to insert
 * @param out_node Output pointer to receive new node (optional)
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_InsertAfter(DoublyLinkedList *list, DListNode *node,
                                        void *value, DListNode **out_node);

/**
 * @brief Inserts a node before a given node
 * @param list Doubly linked list to modify
 * @param node Node before which to insert (must be in the list)
 * @param value Pointer to value to insert
 * @param out_node Output pointer to receive new node (optional)
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_InsertBefore(DoublyLinkedList *list,
                                         DListNode *node, void *value,
                                         DListNode **out_node);

/**
 * @brief Reverses the doubly linked list in-place
 * @param list Doubly linked list to reverse
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode DoublyLinkedList_Reverse(DoublyLinkedList *list);

/**
 * @brief Finds a node by its data (linear search)
 * @param list Doubly linked list to search
 * @param data Data to find
 * @param compare Comparison function (NULL for pointer comparison)
 * @param out_node Output pointer to receive found node
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode DoublyLinkedList_Find(const DoublyLinkedList *list, const void *data,
                                 int (*compare)(const void *, const void *),
                                 DListNode **out_node);

#ifdef __cplusplus
}
#endif

#endif /* DOUBLY_LINKED_LIST_H */
