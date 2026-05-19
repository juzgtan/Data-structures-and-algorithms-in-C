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

/**
 * DoublyLinkedList_PopFront - Remmoves the first element from the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list is not empty
 * 3. Save old head node
 * 4. Update head to next
 * 5. Handle single-element list case
 * 6. Free old head node
 * 7. Decrement size
 *
 * EXAMPLE:
 * Before: head <-> A <-> B <-> C <-> NULL
 * After PopFont(A): head <-> B <-> C <-> NULL, Af is freed
 * @param list Doubly linked list to modify
 * @return Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_PopFront(DoublyLinkedList *list) {
  /* Step 1: Validate parameter */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Save old node */
  DListNode *old_head = list->head;

  /* Step 5: Handle single-element list case
   * If list becomes empty after removal, tail must also be NULL
   * Otherwire, new head's prev shound be NULL
   */

  if (list->head == NULL) {
    /* List is now empty */
    list->tail = NULL;
  } else {
    /* List still has element: remove backward link to old head */
    list->head->prev = NULL;
  }

  /* Step 6: Free old head node */
  free(old_head);

  /* Step 7: Decrement size */
  list->size--;

  return kSuccess;
}

/**
 * DoublyLinkedList_PopBack - Removes the last element from the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list is not empty
 * 3. Save old node tail node
 * 4. Update tail to previous node
 * 5. Handle single-element list case
 * 6. Free old tail node
 * 7. Decrement size
 *
 * EXAMPLE:
 * Before: head <-> A <-> B <-> C <-> NULL, tail = C
 * After PopBack(): head <-> A <-> B <-> NULL, tail = B, C is freed
 *
 * @param list Doubly linked list to modify
 * @param Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_PopBack(DoublyLinkedList *list) {
  /* Step 1: Validate parameters */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Save old tail node */
  DListNode *old_tail = list->tail;

  /* Step 4: Update tail to previous node */
  list->tail = list->tail->prev;

  /* Step 5: Handle singly-element list case
   * If list becomes empty after removal, head must also be NULL
   * Otherwise, new tail's next should be NULL
   */
  if (list->tail == NULL) {
    /* List is now empty */
    list->head = NULL;
  } else {
    /* List still has elements: Remove forward link to old tail */
    list->tail->next = NULL;
  }

  /* Step 6: Free the old tail node */
  free(old_tail);

  /* Step 7: Decrement size */
  list->size--;

  return kSuccess;
}

/**
 * DoublyLinkedList_InsertAt - Inserts an element at specified index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check index bounds
 * 3. Handle special cases (front, back)
 * 4. For middle insertion, find node at index
 * 5. Create new node and link it betwenn prev and current
 * 6. Increment size
 *
 * EXAMPLE: (insert at index = 2)
 * Before: [A] <-> [B] <-> [C] <-> [D]
 * After InsertAt(2, X): [A] <-> [B] <-> [X] <-> [C] <-> [D]
 *
 * @param list Doubly linked list to modify
 * @param index Position to insert at (0 <= index <= size)
 * @param value Pointer to value to insert
 * @return Result code
 * @complexity O(n) - worst case
 */
ResultCode DoublyLinkedList_InsertAt(DoublyLinkedList *list, size_t index,
                                     void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index > list->size) {
    return kInvalidIndex;
  }

  /* Step 3: Handle special cases for efficienly */
  /* Insert at beginning */
  if (index == 0) {
    return DoublyLinkedList_PushFront(list, value);
  }

  /* Insert at end */
  if (index == list->size) {
    return DoublyLinkedList_PushBack(list, value);
  }

  /* Step 4: Insert at middle - find node currently at this index
   * The new node will be inserted BEFORE the current node */
  DListNode *current;
  ResultCode rc = _get_node_at(list, index, &current);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 5: Create new node */
  DListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 6: Link new node betwen current -> prev and current
   * Before: [...] <-> [prev] <-> [current] <-> [...]
   * After:  [...] <-> [prev] <-> [node] <-> [current] <-> [...] */
  node->prev = current->prev;
  node->next = current;
  current->prev->next = node;
  current->prev = node;

  /* Step 7: Increment size */
  list->size++;

  return kSuccess;
}

/**
 * DoublyLinkedList_RemoveAt - Removes an element at specifiec index
 *
 * Implementation flow:
 * 1. Valdate parameters
 * 2. Check index bounds
 * 3. Handle special cases (front, back)
 * 4. For middle removal, find node at index
 * 5. Link prev and nex nodes together
 * 6. Free the removal node
 * 7. Decrement size
 *
 * EXAMPLE: (remove at index = 2)
 * Before : [A] <-> [B] <-> [X] <-> [C] <-> [D]
 * After RemoveAt(2): [A] <-> [B] <-> [C] <-> [D]
 *
 * @param list Doubly linked list to modify
 * @param index Position to remove (0 <= idnex <= size)
 * @param Result code
 * @complexity O(n) - worse case
 */
ResultCode DoublyLinkedList_RemoveAt(DoublyLinkedList *list, size_t index) {
  /* Step 1: Validate parameters */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index >= list->size) {
    return kInvalidIndex;
  }

  /* Step 3: Handle special cases for efficienly */
  /* Remove from beginning */
  if (index == 0) {
    return DoublyLinkedList_PopFront(list);
  }

  /* Remove from end*/
  if (index == list->size - 1) {
    return DoublyLinkedList_PopBack(list);
  }

  /* Step 4: Remove from middle - find node to remove */
  DListNode *node;
  ResultCode rc = _get_node_at(list, index, &node);
  if (rc != kSuccess) {
    return rc;
  }

  /* Link prev and next nodes togetther, bypassing the node to remove
   * Before: [...] <-> [prev] <-> [node] <-> [next] <-> [...]
   * After: [...]  <-> [prev <-> [next] <-> [...] */
  node->prev->next = node->next;
  node->next->prev = node->prev;

  /* Step 6: Free the removed node */
  free(node);

  /* Step 7: Decrement size */
  list->size--;

  return kSuccess;
}

/* ============================================================================
 * ADVANCED OPERATIONS
 * ============================================================================
 */

/**
 * DoublyLinkedList_RemoveNode - Removes a specific node (given node pointer)
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Handle special cases (head, tail)
 * 3. For middle removal, link prev and next
 * 4. Free node
 * 5. Decrement size
 *
 * @param list Doubly linked list to modify
 * @param node Node to remove (must belong to the list)
 * @return Result code
 *
 * This O(1) because we have dirct access to node->prev and node->next
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_RemoveNode(DoublyLinkedList *list,
                                       DListNode *node) {
  /* Step 1: Validate parameters */
  if (list == NULL || node == NULL) {
    return kNullParameter;
  }

  /* Step 2: Handle specified cases */
  /* Where node at head*/
  if (node == list->head) {
    return DoublyLinkedList_PopFront(list);
  }

  /* Where node at tail */
  if (node == list->tail) {
    return DoublyLinkedList_PopBack(list);
  }

  /* Step 3: Remove from middle */
  node->prev->next = node->next;
  node->next->prev = node->prev;

  /* Step 4: Free the node */
  free(node);

  /* Step 5: Decrement size */
  list->size--;

  return kSuccess;
}

/**
 * DoublyLinkedList_InsertAfter - Insert a node after a given node
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Link new node betwen node and node->next
 * 4. Update tail if inserting after tail
 * 5.Increment size
 *
 * @param list Doubly linked list to modify
 * @param node Node after which to insert
 * @param out_value Optinal output for the new node (can be NULL)
 * @return Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_InsertAfter(DoublyLinkedList *list, DListNode *node,
                                        void *value, DListNode **out_node) {
  /* Step 1: Validate parameters */
  if (list == NULL || node == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  DListNode *new_node = _create_node(value);
  if (new_node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Link new node between node and node->next */
  new_node->prev = node;
  new_node->next = node->next;

  /* Step 4: Update node->next if it exists */
  if (node->next != NULL) {
    node->next->prev = new_node;
  } else {
    /*  Node if tail, new node becomes new tail */
    list->tail = new_node;
  }

  node->next = new_node;
  list->size++;

  /* Step 5: Set output if requested */
  if (out_node != NULL) {
    *out_node = new_node;
  }

  return kSuccess;
}

/**
 * DoublyLinkedList_InsertBefore - Insert a node abefore a given node
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Link new node between node->prev and node
 * 4. Update head if inserting before head
 * 5. Increment size
 *
 * @param list Doybly linked list to modify
 * @param node Node before which to insert (mus be in list)
 * @param value Pointer to value to insert
 * @param out_value Optional output for the new node (can be NULL)
 * @result Result code
 * @complexity O(1)
 */
ResultCode DoublyLinkedList_InsertBefore(DoublyLinkedList *list,
                                         DListNode *node, void *value,
                                         DListNode **out_node) {
  /* Step 1: Validate parameters */
  if (list == NULL || node == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  DListNode *new_node = _create_node(value);
  if (new_node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Link new node between node->prev and node */
  new_node->next = node;
  new_node->prev = node->prev;

  /* Step 4: Update node->prev if exists */
  if (node->prev != NULL) {
    node->prev->next = new_node;
  } else {
    /* Node is head, new node becomes new head */
    list->head = new_node;
  }

  node->prev = new_node;
  list->size++;

  /* Step 5: Set output if requested */
  if (out_node != NULL) {
    *out_node = new_node;
  }

  return kSuccess;
}

/**
 * DoubleLinkedList_Reverse - Reverse the list in-place
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check lit not empty
 * 3. Swap next and prev for each node
 * 4. Swap head and tail
 *
 * @param list Doubly list to reverse
 * @param Result code
 * @complexity O(n)*
 */
ResultCode DoublyLinkedList_Reverse(DoublyLinkedList *list) {
  /* Step 1: Validate parameters */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Swap next and prev for each node */
  DListNode *current = list->head;
  DListNode *temp = NULL;

  while (current != NULL) {
    /* Swap next and prev pointers */
    temp = current->prev;
    current->prev = current->next;
    current->next = temp;

    /* Move to next node (which was previous before swap) */
    current = current->prev;
  }

  /* Step 4: Swap head and tail */
  temp = (DListNode *)list->head;
  list->head = list->tail;
  list->tail = temp;

  return kSuccess;
}

/**
 * DoublyLinkedList_Find - Finds a node by its data (linear search)
 *
 * Implementation flow:
 * 1. Validate paramenters
 * 2. Traversal list from head
 * 3. Compare each node's data with target
 * 4. Return node if found
 *
 * @param list Doubly linked list to search
 * @param data Data to find
 * @param compare Comparition function (NULL for pointer comparition)
 * @param out_node Output pointer to receive found node
 * @param result Result code
 * @complexity O(n)
 */
ResultCode DoublyLinkedList_Find(const DoublyLinkedList *list, const void *data,
                                 int (*compare)(const void *, const void *),
                                 DListNode **out_node) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_node == NULL) {
    return kNullParameter;
  }

  /* Step 2: Traverse and search */
  DListNode *current = list->head;

  while (current != NULL) {
    int match = 0;
    if (compare != NULL) {
      match = compare(data, current->data) == 0;
    } else {
      match = (data == current->data);
    }

    if (match) {
      *out_node = current;

      return kSuccess;
    }

    current = current->next;
  }

  return kNotFound;
}

/**
 * DoublyLinkedList_IsPalindrome - Checks if the list is a palindrome
 *
 * Implementation flow:
 * 1. Validate paramenters
 * 2. Use two pointers: left from head, right from tail
 * 3. Compare data at both ends, move inward
 * 4. Return false if mismatch found
 *
 * @param list Doubly linked list to check
 * @param true if palindrome, false otherwise
 * @complexity O(n)
 */
bool DoublyLinkedList_IsPalindrome(const DoublyLinkedList *list) {
  /* Step 1: Validate parameter */
  if (list == NULL || list->size < 2) {
    return true;
  }

  /* Step 2: Initialize pointers at both ends */
  DListNode *left = list->head;
  DListNode *right = list->tail;

  /* Step 3: Compare from both ands toward center */
  for (size_t i = 0; i < list->size / 2; i++) {
    if (left->data != right->data) {
      return false;
    }
    left = list->head->data;
    right = list->tail->prev;
  }

  return true;
}
