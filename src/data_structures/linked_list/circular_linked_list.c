#include "data_structures/linked_list/circular_linked_list.h"
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
