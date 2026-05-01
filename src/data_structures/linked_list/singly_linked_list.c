#include "data_structures/linked_list/singly_linked_list.h"
#include <stdlib.h>

/* ============================================================================
 * INTERNAL HELPER FUNCTIONS (Private - not exposed in header)
 * ============================================================================
 */

/**
 * @brief Creates a new singly linked list node
 * @param data Pointer to data to store in the node
 * @return Pointer to new node, or NULL if allocation fails
 *
 * Implementation flow:
 * 1. Allocate memory for the node
 * 2. Initialize node fields with provided data
 * 3. Set next pointer to NULL
 *
 * @complexity O(1)
 */

static SListNode *_create_node(void *data) {
  /* Step 1: Allocate memory for the node */
  SListNode *node = (SListNode *)malloc(sizeof(SListNode));
  if (node == NULL) {
    return NULL; /* Allocation failed */
  }

  /* Step 2: Initialize node fileds */
  node->data = data; /* Store pointer to data (shallow copy) */
  node->next = NULL; /* Next pointer initially NULL */

  return node;
}
