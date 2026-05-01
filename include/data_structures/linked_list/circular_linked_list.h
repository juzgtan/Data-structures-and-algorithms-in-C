#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

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
 * @brief Circular linked list node
 */
typedef struct CListNode {
  void *data;             /**< Pointer to stored data */
  struct CListNode *next; /**< Pointer to next node */
} CListNode;

/**
 * @brief Circular linked list structure
 *
 * A circular linked list where the last node points back to the first node.
 * Can be singly or doubly circular. This implementation is singly circular.
 *
 * Advantages:
 * - Can traverse infinitely (no NULL termination)
 * - Useful for round-robin scheduling, buffer management
 * - Can start from any node and visit all nodes
 *
 * Disadvantages:
 * - Need careful handling to avoid infinite loops
 * - More complex insertion/deletion at ends
 */
typedef struct {
  size_t size;     /**< Number of elements in the list */
  CListNode *head; /**< Pointer to the first node (for reference) */
  CListNode *tail; /**< Pointer to the last node (for O(1) push_back) */
} CircularLinkedList;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new empty circular linked list
 * @param result Output pointer to receive the new CircularLinkedList
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Create(CircularLinkedList **result);

/**
 * @brief Destroys the circular linked list and frees all associated memory
 * @param list Circular linked list to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void CircularLinkedList_Destroy(CircularLinkedList *list);

/**
 * @brief Removes all nodes from the list
 * @param list Circular linked list to clear
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void CircularLinkedList_Clear(CircularLinkedList *list);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements in the list
 * @param list Circular linked list to examine
 * @return Number of elements (0 if list is NULL)
 *
 * @complexity O(1)
 */
size_t CircularLinkedList_Size(const CircularLinkedList *list);

/**
 * @brief Checks if the list is empty
 * @param list Circular linked list to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool CircularLinkedList_IsEmpty(const CircularLinkedList *list);

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the first element without removing it
 * @param list Circular linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Front(const CircularLinkedList *list,
                                    void **out_value);

/**
 * @brief Returns the last element without removing it
 * @param list Circular linked list to access
 * @param out_value Output pointer to receive back value
 * @return Result code
 *
 * @complexity O(1) if tail pointer maintained, O(n) otherwise
 */
ResultCode CircularLinkedList_Back(const CircularLinkedList *list,
                                   void **out_value);

/**
 * @brief Returns element at specific index without removing it
 * @param list Circular linked list to access
 * @param index Position to retrieve (0-based)
 * @param out_value Output pointer to receive value
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode CircularLinkedList_GetAt(const CircularLinkedList *list,
                                    size_t index, void **out_value);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Inserts an element at the beginning of the list
 * @param list Circular linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * In a circular list, inserting at the front requires updating tail->next
 * to point to the new head.
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_PushFront(CircularLinkedList *list, void *value);

/**
 * @brief Inserts an element at the end of the list
 * @param list Circular linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * In a circular list, inserting at the end is efficient with a tail pointer.
 *
 * @complexity O(1) if tail pointer maintained, O(n) otherwise
 */
ResultCode CircularLinkedList_PushBack(CircularLinkedList *list, void *value);

/**
 * @brief Removes the first element from the list
 * @param list Circular linked list to modify
 * @return Result code
 *
 * In a circular list, removing the front requires updating tail->next
 * to point to the new head.
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_PopFront(CircularLinkedList *list);

/**
 * @brief Removes the last element from the list
 * @param list Circular linked list to modify
 * @return Result code
 *
 * @complexity O(n) (need to find second-to-last node)
 */
ResultCode CircularLinkedList_PopBack(CircularLinkedList *list);

/**
 * @brief Inserts an element at the specified index
 * @param list Circular linked list to modify
 * @param index Position to insert at (0 <= index <= size)
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode CircularLinkedList_InsertAt(CircularLinkedList *list, size_t index,
                                       void *value);

/**
 * @brief Removes an element at the specified index
 * @param list Circular linked list to modify
 * @param index Position to remove (0 <= index < size)
 * @return Result code
 *
 * @complexity O(n) worst case
 */
ResultCode CircularLinkedList_RemoveAt(CircularLinkedList *list, size_t index);

/* ============================================================================
 * CIRCULAR-SPECIFIC OPERATIONS
 * ============================================================================
 */

/**
 * @brief Rotates the list by moving the head forward k steps
 * @param list Circular linked list to modify
 * @param steps Number of steps to rotate forward
 * @return Result code
 *
 * This is a unique operation for circular lists. After rotation,
 * the element at index 'steps' becomes the new head.
 *
 * Example: [1,2,3,4,5] rotated by 2 becomes [3,4,5,1,2]
 *
 * @complexity O(steps)
 */
ResultCode CircularLinkedList_Rotate(CircularLinkedList *list, size_t steps);

/**
 * @brief Splits a circular list into two circular lists at given index
 * @param list Circular linked list to split
 * @param index Position to split at (0 < index < size)
 * @param out_list1 Output pointer for first part (indices 0 to index-1)
 * @param out_list2 Output pointer for second part (indices index to size-1)
 * @return Result code
 *
 * After split, both resulting lists are circular.
 *
 * @complexity O(index)
 */
ResultCode CircularLinkedList_Split(CircularLinkedList *list, size_t index,
                                    CircularLinkedList **out_list1,
                                    CircularLinkedList **out_list2);

/**
 * @brief Merges two circular linked lists
 * @param list1 First circular linked list (will be modified)
 * @param list2 Second circular linked list (will be cleared)
 * @return Result code
 *
 * Appends list2 after list1, forming a single circular list.
 * list2 becomes empty after merge.
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Merge(CircularLinkedList *list1,
                                    CircularLinkedList *list2);

/**
 * @brief Checks if the list is properly circular (no NULL termination)
 * @param list Circular linked list to validate
 * @return true if valid circular list, false otherwise
 *
 * Verifies that tail->next == head (for non-empty lists)
 *
 * @complexity O(1)
 */
bool CircularLinkedList_IsValid(const CircularLinkedList *list);

/**
 * @brief Returns the next node after the given node (for iteration)
 * @param list Circular linked list (for validation)
 * @param node Current node
 * @param out_next Output pointer to receive next node
 * @return Result code
 *
 * Useful for safe iteration in circular lists.
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_GetNext(const CircularLinkedList *list,
                                      CListNode *node, CListNode **out_next);

/**
 * @brief Finds the previous node of a given node
 * @param list Circular linked list to search
 * @param node Node to find previous of (must be in the list)
 * @param out_prev Output pointer to receive previous node
 * @return Result code
 *
 * @complexity O(n) (must traverse from head)
 */
ResultCode CircularLinkedList_GetPrevious(const CircularLinkedList *list,
                                          CListNode *node,
                                          CListNode **out_prev);

/**
 * @brief Removes a specific node from the list (given node pointer)
 * @param list Circular linked list to modify
 * @param node Node to remove (must be in the list)
 * @return Result code
 *
 * @complexity O(n) in singly circular (need previous node)
 */
ResultCode CircularLinkedList_RemoveNode(CircularLinkedList *list,
                                         CListNode *node);

/**
 * @brief Finds a node by its data (linear search)
 * @param list Circular linked list to search
 * @param data Data to find
 * @param compare Comparison function (NULL for pointer comparison)
 * @param out_node Output pointer to receive found node
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode CircularLinkedList_Find(const CircularLinkedList *list,
                                   const void *data,
                                   int (*compare)(const void *, const void *),
                                   CListNode **out_node);

/* ============================================================================
 * TRAVERSAL HELPERS
 * ============================================================================
 */

/**
 * @brief Function type for node visitor callback
 *
 * This callback function is called for each node during traversal.
 *
 * @param data Data stored in the node being visited
 * @param user_data Arbitrary user data passed to the traversal function
 *
 * Example:
 * @code
 * void print_int(void *data, void *user_data) {
 *     printf("%d ", *(int*)data);
 * }
 *
 * CircularLinkedList_Traverse(list, print_int, NULL);
 * @endcode
 */
typedef void (*CircularVisitFunction)(void *data, void *user_data);

/**
 * @brief Traverses the circular list, applying a function to each node
 * @param list Circular linked list to traverse
 * @param visit Function to call for each node
 * @param user_data User data to pass to visit function
 * @return Result code
 *
 * Stops after visiting all nodes once (detects when we return to head).
 *
 * @complexity O(n)
 */
ResultCode CircularLinkedList_Traverse(const CircularLinkedList *list,
                                       CircularVisitFunction visit,
                                       void *user_data);

/**
 * @brief Traverses the circular list starting from an arbitrary node
 * @param list Circular linked list to traverse
 * @param start Starting node (must be in the list, NULL starts from head)
 * @param visit Function to call for each node
 * @param user_data User data to pass to visit function
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode CircularLinkedList_TraverseFrom(const CircularLinkedList *list,
                                           CListNode *start,
                                           CircularVisitFunction visit,
                                           void *user_data);

/* ============================================================================
 * JOSEPHUS PROBLEM SPECIFIC
 * ============================================================================
 */

/**
 * @brief Solves the Josephus problem using circular list
 * @param list Circular linked list to modify (will be destroyed)
 * @param k Step count for elimination
 * @param elimination_order Output array for elimination order (must be
 * pre-allocated)
 * @param out_len Number of elements eliminated
 * @param last_survivor Output pointer for the survivor's data
 * @return Result code
 *
 * Josephus problem: n people in a circle, eliminate every k-th person.
 * Returns the elimination order and the last survivor.
 *
 * @complexity O(n * k)
 */
ResultCode CircularLinkedList_Josephus(CircularLinkedList *list, int k,
                                       void **elimination_order,
                                       size_t *out_len, void **last_survivor);

#ifdef __cplusplus
}
#endif

#endif /* CIRCULAR_LINKED_LIST_H */
