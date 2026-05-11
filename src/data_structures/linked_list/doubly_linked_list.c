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

/**
 * @brift Retrieves the node at specific index
 * @param list Double linked list to traverse
 * @param index Position to retrieve (0-based)
 * @param out_value Output pointer to receive the node
 * @result Result code
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check index bounds
 * 3. Choose traversal direction based on index position
 * 4. Traverse from closer end tho the desied index
 * 5. Set output pointer
 *
 * Optimization strategy:
 * If index < size/2: Traverse from head (forward)
 * If index >= size/2: traverse from tail (backward)
 *
 * EXAMPLE: size = 10
 * index = 3 (3 < 5): traverse feom head (forward)
 * index = 7 (7 >= 5): traverse from tail (backward)
 *
 * @complexity O(n/2)
 */
static ResultCode _get_node_at(const DoublyLinkedList *list, size_t index,
                               DListNode **out_value) {
  /* Step 1: Validate patameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index >= list->size) {
    return kInvalidIndex;
  }

  DListNode *current;
  /* Step 3: Choose traversal direction for better prerformence */
  if (index < list->size / 2) {
    /* Case 1: Index in first harf - traverse from head */
    current = list->head;
    for (size_t i = 0; i < index; i++) {
      current = current->next;
    }
  } else {
    /* Case 2: Index in second half - traverse from tail */
    current = list->head;
    for (size_t i = list->size - 1; i > index; i--) {
      current = current->prev;
    }
  }

  /* Step 4: Set output and return */
  *out_value = current;

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

/**
 * DoublyLinkedList_GetAt - Return element at specific index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Find node at given index
 * 3. Retrieve data from that node
 * 4. Set out put pointer
 *
 * @param list Doubly linked list to access
 * @param index Position to retrieve (o-based)
 * @param out_value Output pointer to receive value
 * @return Result code
 * @complexity O(n/2) - average
 */
ResultCode DoublyLinkedList_GetAt(const DoublyLinkedList *list, size_t index,
                                  void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Find node at index */
  DListNode *node;
  ResultCode rc = _get_node_at(list, index, &node);
  if (rc != kSuccess) {
    return kSuccess;
  }

  /* Step 3: Retrieve data and return */
  *out_value = node->data;

  return kSuccess;
}

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * DoubleLinkedList_PushFront - Inserts an element at beginning of the list
 *
 * Implementation flow:
 * 1. Validate parameters:
 * 2. Create new node
 * 3. Handle empty list case
 * 4. Handle non-empty list case (link at front)
 * 5. Update head pointer
 * 6. Increment size
 *
 * EXAMPLE:
 * Before: head <-> A <-> B <-> NULL (tail)
 * After PushFront(C): head <-> C <-> A <-> B <-> NULL (tail)
 *
 * @param list Doubly lined list to modify
 * @param value Pointer to value to insert
 * @param result Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_PushFront(DoublyLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  DListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Handle empty list case */
  if (list->head == NULL) {
    /* First element: Head and tail both pointer the new node */
    list->head = node;
    list->tail = node;
  } else {
    /* Step 4: Handle non-empty list - link at front
     * New node's next pointer to current head
     * Current head's prev pointer to new node */
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
  }

  /* Step 5: Increment size */
  list->size++;

  return kSuccess;
}

/**
 * DoublyLinkedList_PushBack - Inserts an element at the end of the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Handle empty  list case
 * 4. Handle non-empty list case (link at back)
 * 5. Update tail pointer
 * 6. Increment size
 *
 * EXAMPLE:
 * Before: head <-> A <-> B <-> NULL (tail)
 * After PushBack(C): head <-> A <-> B <-> C <-> NULL (tail)
 *
 * @param list Doubly linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_PushBack(DoublyLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  DListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Handle empty list case */
  if (list->tail == NULL) {
    /* First element: head and tail both point to the new node */
    list->head = node;
    list->tail = node;
  } else {
    /* Step 4: Handle non-empty list - link at back
     * Current tail's next points to new node
     * New node's prev points to current tail
     */
    node->prev = list->tail;
    list->tail->next = node;
    list->tail = node;
  }

  /* Step 5: Increment size */
  list->size++;

  return kSuccess;
}
