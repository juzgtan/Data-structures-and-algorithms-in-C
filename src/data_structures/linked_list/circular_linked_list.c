#include "data_structures/linked_list/circular_linked_list.h"
#include "result_code.h"
#include <stddef.h>
#include <stdlib.h>

/* ============================================================================
 * INTERNAL HELPER FUNCTIONS (Private - not exposed in header)
 * ============================================================================
 */

/**
 * @brief Creates a new circular linked list node
 * @param data Pointer to data to store in the node
 * @return Pointer to new node, or NULL if allocation fails
 *
 * @complexity O(1)
 */
static CListNode *_create_node(void *data) {
  CListNode *node = (CListNode *)malloc(sizeof(CListNode));
  if (node == NULL) {
    return NULL;
  }

  node->data = data;
  node->next = NULL; /* Will be set to appropriate node */

  return node;
}

/**
 * @brief Retrieves the node at specific index in circlar linked list
 *
 * @param list Circular linked list
 * @param index Position to retrieve (0-based)
 * @param out_node Outout pointer to receive the node
 * @result Result code
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2.Check index bounds
 * 3. Traverse from head to the desired index
 * 4. Set output pointer
 *
 * @complexity O(n) - in worst case
 */
static ResultCode _get_node_at(const CircularLinkedList *list, size_t index,
                               CListNode **out_node) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_node == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index >= list->size) {
    return kInvalidIndex;
  }

  /* Step 3: Traverse from head to the desired index */
  CListNode *current = list->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }

  /* Step 4: Set output and return */
  *out_node = current;

  return kSuccess;
}

/**
 * @brief Validates that the list is not empty
 *
 * @param list Circular linked list to check
 * @return Result code
 *
 * CASES HANDLED:
 * - list == NULL -> kNullParameter
 * - size == 0 -> kEmpty
 * - otherwise -> kSuccess
 *
 * @complexity O(1)
 */
static ResultCode _check_not_empty(const CircularLinkedList *list) {
  if (list == NULL) {
    return kNullParameter;
  }
  if (list->size == 0) {
    return kEmpty;
  }

  return kSuccess;
}

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * CircularLinkedList_Create - Creates a new empty circular linked list
 *
 * Implementation flow:
 * 1. Set output pointer to NULL (safety first)
 * 2. Validate output parameter not NULL
 * 3. Check output pointer os not pointing to valid memory (prevent leak)
 * 4. Allocate CircularLinkedList struct
 * 5. Initialize fields to empty state
 * 6. Assign result and return success
 *
 * @param result Output pointer to receive the new CircularLinkedList
 * @param Result code
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Create(CircularLinkedList **result) {
  /* Step 1: Always set output to NULL first */
  if (result != NULL) {
    *result = NULL;
  }

  /* Step 2: Validate output parameter */
  if (result == NULL) {
    return kNullParameter;
  }

  /* Step 3: Check output pointer is not already pointing to valid memory */
  if (*result != NULL) {
    return kOutputPointerIsNotNull;
  }

  /* Step 4: Allocate CircularLinkedList struct */
  CircularLinkedList *list =
      (CircularLinkedList *)malloc(sizeof(CircularLinkedList));
  if (list == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 5: Initialize fields to empty state */
  list->size = 0;    /* No elements initially */
  list->head = NULL; /* Head points to nothing */
  list->tail = NULL; /* Tail points to nothing */

  /* Step 6: Success - Set output and return */
  *result = list;
  return kSuccess;
}

/**
 * CircularLinkedList_Clear - Removes all nodes from the list
 *
 * Implementation flow:
 * 1. Check list is NULL or empty (silently return )
 * 2. Break the circular linl first
 * 3. Traverse and free all nodes
 * 4. Reset list to empty state
 *
 * @param list Circular linked list to clear
 * @complexity O(n)
 */
void CircularLinkedList_Clear(CircularLinkedList *list) {
  /* Step 1: Handle NULL or empty gracefully */
  if (list == NULL || list->size == 0) {
    return;
  }

  /* Step 2: Break circular link first
   * This prevents infinite loop during traversal
   */
  if (list->tail != NULL) {
    list->tail->next = NULL;
  }

  /* Step 3: Traverse and free all nodes */
  CListNode *current = list->head;
  while (current != NULL) {
    CListNode *next = current->next;
    free(current);
    current = next;
  }

  /* Step 4: Reset list to empty state */
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
}

/**
 * CircularLinkedList_Destroy - Frees all memory associated with the list
 *
 * Implementation flow:
 * 1. Check if list is NULL (silently return)
 * 2. Clear all nodes (reuse CircularLinkedList_Clear logic)
 * 3. Free the CircularLinkedList struct itself
 *
 * @param list Circular linked list to destroy (can be NULL)
 * @complexity O(n)
 */
void CircularLinkedList_Destroy(CircularLinkedList *list) {
  if (list == NULL) {
    return;
  }

  CircularLinkedList_Clear(list);
  free(list);
}

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/** CircularLinkedList_Size - Returns the numbers of elements in the list
 *
 * Returns 0 if list is NULL (safe behavior)
 *
 * @complexity O(1)
 */
size_t CircularLinkedList_Size(const CircularLinkedList *list) {
  return list == NULL ? 0 : list->size;
}

/** CircularLinkedList_IsEmpty - Checks if the list contains any elements
 *
 * Return true if list is NULL or Empty
 *
 * @complexity O(1)
 */
bool CircularLinkedList_IsEmpty(const CircularLinkedList *list) {
  return list == NULL ? true : list->size == 0;
}

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * CircularLinkedList_Front - Returns the first element without removing it
 *
 * Implementation flow:
 * 1. Vaidate parameters
 * 2. Check list is not empty
 * 3. Retrieve data from head node
 * 4. Set output pointer
 *
 * @param list Circular linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Front(const CircularLinkedList *list,
                                    void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value != NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data from head and set value for output pointer */
  *out_value = list->head->data;

  return kSuccess;
}

/**
 * CircularLinkedList_Back - Returns the fast element without removing it
 *
 * Implementation flow:
 * 1. Vaidate parameters
 * 2. Check list is not empty
 * 3. Retrieve data from tail node
 * 4. Set output pointer
 *
 * @param list Circular linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Back(const CircularLinkedList *list,
                                   void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value != NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data from tail and set value for output pointer */
  *out_value = list->tail->data;

  return kSuccess;
}
