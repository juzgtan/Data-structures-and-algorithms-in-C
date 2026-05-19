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
