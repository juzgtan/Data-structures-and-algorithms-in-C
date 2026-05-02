#include "data_structures/linked_list/singly_linked_list.h"
#include "result_code.h"
#include <stdbool.h>
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

/**
 * @brief Validates that the list is not empty
 * @param list Singly linked list to check
 * @return Result code
 *
 * CASES HANDLED:
 * - list == NULL -> kNullParameter
 * - size == 0    -> kEmpty
 * - otherwise    -> kSuccess
 *
 * @complexity O(1)
 */
static ResultCode _check_not_empty(const SinglyLinkedList *list) {
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
 * SinglyLinkedList_Create - Creates a new empty singly linked list
 *
 * Implementation flow:
 * 1. Set output pointer to NULL (safety first)
 * 2. Validate output parameter not NULL
 * 3. Check output pointer is not poiting to valid memory (prement leak)
 * 4. Allocate SinglyLinkedList struct
 * 5. Initialize fields to empty state
 * 6. Assign result and return success
 *
 * @param result Output pointer to retrieve the new SinglyLinkedList
 * @return Result code
 *
 * complexity O(1)
 */
ResultCode SinglyLinkedList_Create(SinglyLinkedList **result) {
  /* Step 1: Alway set output to NULL first */
  if (result != NULL) {
    *result = NULL;
  }

  /* Step 2: Validate output paramenter */
  if (result == NULL) {
    return kNullParameter;
  }

  /* Step 3: Check putput pointer is not already poiting to valid memory
   * This prevents memory leak if caller passes an already-allocated pointer */
  if (*result != NULL) {
    return kOutputPointerIsNotNull;
  }

  /* Step 4: Allocate SinglyLinkedList struct */
  SinglyLinkedList *list = (SinglyLinkedList *)malloc(sizeof(SinglyLinkedList));
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
 * SinglyLinkedList_Destroy - Frees all memory associated with the list
 *
 * Implementation flow:
 * 1. Handle NULL input gracefully (no operation)
 * 2. Remove and free all nodes in the list
 * 3. Free the SinglyLinkedList structure itself
 *
 * @note After this call, the list pointer becomes invalid
 * @param list Singly linked list to destroy (can be NULL)
 * @complexity O(n)
 */
void SinglyLinkedList_Destroy(SinglyLinkedList *list) {
  /* Step 1: Handle NULL gracefully */
  if (list == NULL) {
    return;
  }

  /* Step 2: Clear all nodes (frees all node memory) */
  SinglyLinkedList_Clear(list);

  /* Step 3: Free struct ifselt */
  free(list);
}

/** SinglyLinkedList_Clear - Remove all nodes from the list
 *
 * Implementation flow:
 * 1. Check if list is NULL (silently return)
 * 2. Traverse from head, freeing earch node
 * 3. Reset head, tail, and size to initial state
 *
 * @param list Singly linked list to clear
 * @complexity O(n)
 */
void SinglyLinkedList_Clear(SinglyLinkedList *list) {
  /* Step 1: Handle NULL gracefully */
  if (list == NULL) {
    return;
  }
  /* Step 2: Traverse and free all nodes
   * EXAMPLE: head -> A -> B -> C -> NULL
   * Iter 1: current = A , next = B, free(A)
   * Iter 2: current = B, next = C, free(B)
   * Iter 3: current = C, next = NULL, free(C)
   * Done.
   */
  SListNode *current = list->head;
  while (current != NULL) {
    SListNode *next = current->next; /* Save next before feeing current */
    free(current);                   /* Free current node */
    current = next;                  /* Move to next node */
  }

  /* Step 3: Reset list to empty state */
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
}

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * SinglyLinkedList_Size - Returns the number of elements in the list
 * Return 0 if list is NULL (safe behavior)
 * @complexity O(1)
 */
size_t SinglyLinkedList_Size(const SinglyLinkedList *list) {
  return list == NULL ? 0 : list->size;
}

/**
 * SinglyLinkedList_IsEmpty - Checks if the list contains any elements
 * Returns true if the list is NULL or contains no elements (size == 0),
 * otherwise returns false.
 * @complexity O(1)
 */
bool SinglyLinkedList_IsEmpty(const SinglyLinkedList *list) {
  return list == NULL ? true : list->size == 0;
}

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * SinglyLinkedList_Front - Return the first element without removing it
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list is not empty
 * 3. Retrieve data from head node
 * 4. Set output pointer
 *
 * @param list Singly linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 * @complexity O(1)
 */
ResultCode SinglyLinkedList_Front(const SinglyLinkedList *list,
                                  void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check if list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data fromt head node */
  *out_value = list->head;

  return kSuccess;
}

/**
 * SinglyLinkedList_Back - Returns the last element without removing it
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list is not empty
 * 3. Retrieve data from tail node
 * 4. Set output pointer
 *
 * @param list Singly linked list to access
 * @param out_value Output pointer to recieve back value
 * @return Result code
 *
 * @complexity O(1) - (Due to tail pointer)
 */
ResultCode SinglyLinkedList_Back(const SinglyLinkedList *list,
                                 void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Retrieve data from tail node */
  *out_value = list->tail;

  return kSuccess;
}

/**
 * SinglyLinkedList_GetAt - Returns elements at specific index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Find node at given index
 * 3. Retrieve data from that node
 * 4. Set output pointer
 *
 * @param list Singly linked list to access
 * @param index Position to retrieve value
 * @return Result code
 * @complexity O(n) - worst case
 */
ResultCode SinglyLinkedList_GetAt(const SinglyLinkedList *list, size_t index,
                                  void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Find node at index */
  SListNode *node;
  ResultCode rc = _get_node_at(list, index, &node);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data and return */
  *out_value = node->data;

  return kSuccess;
}

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/** SinglyLinkedList_PushFront - Inserts an element at the beginning of the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Link new node to current head
 * 4. Update head pointer
 * 5. Update tail if list was empty
 * 6. Increment size
 *
 * EXAMPLE:
 * Before: head -> A -> B -> NULL
 * After PushFront: head -> C -> A -> B -> NULL
 *
 * @param list Singly linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 * @complexity O(1)
 */
ResultCode SinglyLinkedList_PushFront(SinglyLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  SListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Insert at front */
  node->next = list->head; /* New node points to current head */
  list->head = node;       /* Head now points to new node */

  /* Step 4: Update tail if list was empty */
  if (list->tail == NULL) {
    list->tail = node; /* In empty list, head and tail are same */
  }

  /* Step 5: Increment size */
  list->size++;

  return kSuccess;
}

/**
 * SinglyLinkedList_PushBack - Inserts an element at the end of the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Handle empty list case
 * 4. Handle non-empty list case (append after tail)
 * 5. Increment size
 *
 * EXAMPLE:
 * Before: head -> A -> B -> NULL, tail -> B
 * After PushBack(C): head -> A -> B -> C, tail -> C
 *
 * @param list Singly linked list to modify
 * @param value Pointer to value to insert
 * @result Result code
 * complexity O(1) - (Due to tail pointer)
 */
ResultCode SinglyLinkedList_PushBack(SinglyLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  SListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Insert at black */
  if (list->tail == NULL) {
    /* Case 1: Empty list - new node becomes both head and tail */
    list->head = node;
    list->tail = node;
  } else {
    /* Case 2: Non-empty list - Append after current tail */
    list->tail->next = node; /* Curent tail points to new node */
    list->tail = node;       /* Update tail to new node */
  }

  /* Step 4: Increment size */
  list->size++;

  return kSuccess;
}

/**
 * SinglyLinkedList_PopFront - Removes first element from the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list is not empty
 * 3. Save old head node
 * 4. Update to next node
 * 5. Handle single-element list case
 * 6. Free old head node
 * 7. Decrement size
 *
 * EXAMPLE:
 * Before: head -> A -> B -> C -> NULL
 * After: PopFront(A): head -> B -> C -> NULL, A is freed
 *
 * @param list Singly linked list to modify
 * @return Result code
 * @complexity O(1)
 */
ResultCode SinglyLinkedList_PopFront(SinglyLinkedList *list) {
  /* Step 1: Validate parameters */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Save old head node */
  SListNode *old_node = list->head;

  /* Step 4: Update head node to nex node */
  list->head = list->head->next;

  /* Step 5: Handler-single element list case
   * If list becomes empty after removal, tail must also be NULL
   */
  if (list->head == NULL) {
    list->tail = NULL; /* List is now empty */
  }

  /* Free old head node */
  free(old_node);

  /* Step 7: Decrement size */
  list->size--;

  return kSuccess;
}
