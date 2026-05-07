#include "data_structures/linked_list/doubly_linked_list.h"
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
