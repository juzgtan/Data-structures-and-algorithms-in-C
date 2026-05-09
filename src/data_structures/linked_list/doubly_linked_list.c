#include "data_structures/linked_list/doubly_linked_list.h"
#include "result_code.h"
#include <stdlib.h>

/* ============================================================================
 * INTERNAL HELPER FUNCTIONS (Private - not exposed in header)
 * ============================================================================
 */
/**
 * @brief Create a new doubly linked list node
 * @param data Pointer to data to store in the node
 * @return Pointer to new node or NULL if allocatation fails
 *
 * Implementation flow:
 * 1. Allocate memory for the node
 * 2. Initialize node fields with provided data
 * 3. Set next and prev pointers to NULL
 *
 * @complexity O(1)
 */
static DListNode *_create_node(void *data) {
  /* Step 1: Allocate memory for the node */
  DListNode *node = (DListNode *)malloc(sizeof(DListNode));
  if (node == NULL) {
    return NULL;
  }

  /* Step 2: Initialize node fields */
  node->data = data; /* Store pointer to data (shallow copyt) */
  node->next = NULL; /* Next pointer to initially NULL */
  node->prev = NULL; /* Previous pointer initially NULL */

  return node;
}

/**
 *
 * @brief Validates that list not empty
 * @param list Doubly linked list to check
 * @return Result code
 *
 * Cases handle:
 * - list == NULL -> kNullParameter
 * - size == 0    -> kEmpty
 * - otherwise    -> kSuccess
 *
 * @complexity O(1)
 */
static ResultCode _check_not_empty(const DoublyLinkedList *list) {
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

/* DoublyLinkedList_Create - Creates a new empty doubly linked list
 *
 * Implementation flow:
 * 1. Set output pointer to NULL (safety first)
 * 2. Validate output parameter not NULL
 * 3. Check output pointer not pointing to valid memory (prevent leak)
 * 4. Allocate DoublyLinkedList struct
 * 5. Initialize fields to empty node
 * 6. Assign result and return success
 *
 * @param result Output pointer to receive new DoubleLinkedList
 * @return Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_Create(DoublyLinkedList **result) {
  /* Step 1: Always set output to NULL first */
  if (result != NULL) {
    *result = NULL;
  }

  /* Step 2: Validate output paramenter */
  if (result == NULL) {
    return kNullParameter;
  }

  /* Step 3: Check output pointer is not already pointing to valid memory */
  if (*result != NULL) {
    return kOutputPointerIsNotNull;
  }

  /* Step 4: Allocate DoublyLinkedList struct */
  DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
  if (list == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 5: Initialize fields to empty state */
  list->size = 0;    /* No elements initizlly */
  list->head = NULL; /* Head points to nothing */
  list->tail = NULL; /* Tail points to nothing */

  /* Step 6: Success - set output and return */
  *result = list;

  return kSuccess;
}

/**
 * DoublyLinkedList_Clear - Removes all nodes from the list
 *
 * Implementation flow:
 * 1. Check if list is NULL (silently return)
 * 2. Trarvese from head, freeing each node
 * 3. Reset head, tail, and size to initial state
 *
 * @param list Doubly linked list to clear
 * @complexity O(n)
 */
void DoublyLinkedList_Clear(DoublyLinkedList *list) {
  /* Step 1: Handle NULL gracefully */
  if (list == NULL) {
    return;
  }

  /* Step 2: Trarvese and free all nodes */
  DListNode *current = list->head;
  while (current != NULL) {
    DListNode *next = current->next; /* Save next before freeing current */
    free(current);                   /* Free current node */
    current = next;                  /* Move to next node */
  }

  /* Step 3: Reset list to empty state */
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
}

/**
 * DoublyLinkedList_Destroy - Frees all memory associated with the node
 *
 * Implementation flow:
 * 1. Check if list is NULL (silently return)
 * 2. Clear all nodes
 * 3. Free DoublyLinkedList struct itself
 *
 * @param list Doubly linked list to destroy (can be NULL)
 * @complexity O(n)
 */
void DoublyLinkedList_Destroy(DoublyLinkedList *list) {
  /* Step 1: Handle NULL gracefully */
  if (list == NULL) {
    return;
  }

  /* Step 2: Clear all node (free all node memory) */
  DoublyLinkedList_Clear(list);

  /* Step 3: Free struct itself */
  free(list);
}

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * DoublyLinkedList_Size - Return number of elements in the list
 * Return 0 if list is NULL (safe behavior)
 * @complexity O(1)
 */
size_t DoublyLinkedList_Size(const DoublyLinkedList *list) {
  return list == NULL ? 0 : list->size;
}

/**
 * DoublyLinkedList_IsEmpty - Check if the list contains any elements
 * Return True if list is NULL or empty
 * @complexity O(1)
 */
bool DoublyLinkedList_IsEmpty(const DoublyLinkedList *list) {
  return list == NULL ? true : list->size == 0;
}

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * DoublyLinkedList_Front - Return the first element without removing it
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check if is not empty
 * 3. Retrieve data from head node
 * 4. Set output pointer
 *
 * @param list Doubly linked list to access
 * @out_value Output pointer to receive front value
 * @return Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_Front(const DoublyLinkedList *list,
                                  void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data from head node */
  *out_value = list->head->prev;

  return kSuccess;
}

/**
 * DoublyLinkedList_Back - Return the last element without removing it
 *
 * Implementation flow:
 * 1. Validate paramenters
 * 2. Check list is not empty
 * 3. Retrieve data from tail node
 * 4. Set output pointer
 *
 * @param list Doubly linked list to access
 * @param out_value Output pointer to receive back value
 * @return Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_Back(const DoublyLinkedList *list,
                                 void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retireve data  from tail node */
  *out_value = list->tail->data;

  return kSuccess;
}
