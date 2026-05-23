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

/**
 * CircularLinkedList_GetAt - Returns element at specific index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Get node at specific index
 * 3. Set out put pointer
 *
 * @param list Circular linked list to access
 * @param index Position to retrieve (0-based)
 * @param out_value Output pointer to receive value
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode CircularLinkedList_GetAt(const CircularLinkedList *list,
                                    size_t index, void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Get node at specific index */
  CListNode *node;
  ResultCode rc = _get_node_at(list, index, &node);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data from index and set value for output pointer */
  *out_value = node->data;

  return kSuccess;
}
/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * CircularLinkedList_PushFront - Insert an element at the beginning of the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Handle empty list case (point to if self)
 * 4. Handle non-empty list case (insert at front, maintain circularity
 * 5. Increment size
 *
 * In a circular list, inserting at the front requires updating tail->next
 * to point to the new head)
 *
 * @param list Circular linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_PushFront(CircularLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  CListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Handle empty list case */
  if (list->head == NULL) {
    /* Empty list - new node points to ifself */
    node->next = node;
    list->head = node;
    list->tail = node;
  } else {
    /* Step 4: Non-empty list - insert at front
     * New node points to current head *
     * Tail's next points to new node (maintain circularity)
     */
    node->next = list->head;
    list->head = node;
    list->tail->next = node; /* Update tail's next to new head */
  }

  /* Step 5: Increment size */
  list->size++;

  return kSuccess;
}

/**
 * CircularLinkedList_PushBack - Inserts an element at the end of the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Handle empty list case (point to itself)
 * 4.Handle non-empty list case (insert at back, maintain circularity)
 * 5 Increment size
 *
 * @param list Circular linked list to modify
 * @param value Pointer to value to insert
 * @result Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_PushBack(CircularLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  CListNode *node = _create_node(value);
  if (value == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Handle empty list case */
  if (list->tail == NULL) {
    /* Empty list - new node points to ifself */
    node->next = list->head;
    list->head = node;
    list->tail = node;
  } else {
    /* Step 4: Non-empty list - insert at back
     * New node points to head (maintain circularity)
     * Current tail's next point to new node
     */
    node->next = list->head;
    list->tail->next = node;
    list->tail = node;
  }

  /* Step 5: Increment size */
  list->size++;

  return kSuccess;
}
