#include "data_structures/linked_list/singly_linked_list.h"
#include "result_code.h"
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

/**
 * @brief Retrieves the node at a specific index
 * @param list Singly linked list to traversa
 * @param index Position to retrieve (0-based)
 * @param out_node Output pointer to receive the node
 * @return Result code
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check index bounds
 * 3. Traversa from head to the desired index
 * 4. Set output pointer
 *
 * CASES HANDLED:
 * - list == NULL     -> kNullParameter
 * - out_node == NULL -> kNullParameter
 * - index >= size    -> kInvalidIndex
 *
 * @complexity O(n) worst case and O(index) average
 */

static ResultCode _get_node_at(const SinglyLinkedList *list, size_t index,
                               SListNode **out_node) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_node == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index >= list->size) {
    return kInvalidIndex;
  }

  /* Step 3: Traversa from head to the desired index */
  SListNode *current = list->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }

  /* Step 4: Set output and return */
  *out_node = current;

  return kSuccess;
}
