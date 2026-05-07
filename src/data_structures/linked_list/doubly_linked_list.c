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
