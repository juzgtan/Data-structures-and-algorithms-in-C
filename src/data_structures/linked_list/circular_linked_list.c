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
 * @brief Retrieves the node at specific index in circlar linked list
 *
 * @param list Circular linked list
 * @param index Position to retrieve (0-based)
 * @param out_node Outout pointer to receive the node
 * @result Result code
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2.Check index bounds
 * 3. Traverse from head to the desired index
 * 4. Set output pointer
 *
 * @complexity O(n) - in worst case
 */
static ResultCode _get_node_at(const CircularLinkedList *list, size_t index,
                               CListNode **out_node) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_node == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index >= list->size) {
    return kInvalidIndex;
  }

  /* Step 3: Traverse from head to the desired index */
  CListNode *current = list->head;
  for (size_t i = 0; i < index; i++) {
    current = current->next;
  }

  /* Step 4: Set output and return */
  *out_node = current;

  return kSuccess;
}

/**
 * @brief Validates that the list is not empty
 *
 * @param list Circular linked list to check
 * @return Result code
 *
 * CASES HANDLED:
 * - list == NULL -> kNullParameter
 * - size == 0 -> kEmpty
 * - otherwise -> kSuccess
 *
 * @complexity O(1)
 */
static ResultCode _check_not_empty(const CircularLinkedList *list) {
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

/**
 * CircularLinkedList_Clear - Removes all nodes from the list
 *
 * Implementation flow:
 * 1. Check list is NULL or empty (silently return )
 * 2. Break the circular linl first
 * 3. Traverse and free all nodes
 * 4. Reset list to empty state
 *
 * @param list Circular linked list to clear
 * @complexity O(n)
 */
void CircularLinkedList_Clear(CircularLinkedList *list) {
  /* Step 1: Handle NULL or empty gracefully */
  if (list == NULL || list->size == 0) {
    return;
  }

  /* Step 2: Break circular link first
   * This prevents infinite loop during traversal
   */
  if (list->tail != NULL) {
    list->tail->next = NULL;
  }

  /* Step 3: Traverse and free all nodes */
  CListNode *current = list->head;
  while (current != NULL) {
    CListNode *next = current->next;
    free(current);
    current = next;
  }

  /* Step 4: Reset list to empty state */
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
}

/**
 * CircularLinkedList_Destroy - Frees all memory associated with the list
 *
 * Implementation flow:
 * 1. Check if list is NULL (silently return)
 * 2. Clear all nodes (reuse CircularLinkedList_Clear logic)
 * 3. Free the CircularLinkedList struct itself
 *
 * @param list Circular linked list to destroy (can be NULL)
 * @complexity O(n)
 */
void CircularLinkedList_Destroy(CircularLinkedList *list) {
  if (list == NULL) {
    return;
  }

  CircularLinkedList_Clear(list);
  free(list);
}

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/** CircularLinkedList_Size - Returns the numbers of elements in the list
 *
 * Returns 0 if list is NULL (safe behavior)
 *
 * @complexity O(1)
 */
size_t CircularLinkedList_Size(const CircularLinkedList *list) {
  return list == NULL ? 0 : list->size;
}

/** CircularLinkedList_IsEmpty - Checks if the list contains any elements
 *
 * Return true if list is NULL or Empty
 *
 * @complexity O(1)
 */
bool CircularLinkedList_IsEmpty(const CircularLinkedList *list) {
  return list == NULL ? true : list->size == 0;
}

/* ============================================================================
 * DATA ACCESS FUNCTIONS
 * ============================================================================
 */

/**
 * CircularLinkedList_Front - Returns the first element without removing it
 *
 * Implementation flow:
 * 1. Vaidate parameters
 * 2. Check list is not empty
 * 3. Retrieve data from head node
 * 4. Set output pointer
 *
 * @param list Circular linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Front(const CircularLinkedList *list,
                                    void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value != NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data from head and set value for output pointer */
  *out_value = list->head->data;

  return kSuccess;
}

/**
 * CircularLinkedList_Back - Returns the fast element without removing it
 *
 * Implementation flow:
 * 1. Vaidate parameters
 * 2. Check list is not empty
 * 3. Retrieve data from tail node
 * 4. Set output pointer
 *
 * @param list Circular linked list to access
 * @param out_value Output pointer to receive front value
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Back(const CircularLinkedList *list,
                                   void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value != NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data from tail and set value for output pointer */
  *out_value = list->tail->data;

  return kSuccess;
}

/**
 * CircularLinkedList_GetAt - Returns element at specific index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Get node at specific index
 * 3. Set out put pointer
 *
 * @param list Circular linked list to access
 * @param index Position to retrieve (0-based)
 * @param out_value Output pointer to receive value
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode CircularLinkedList_GetAt(const CircularLinkedList *list,
                                    size_t index, void **out_value) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Get node at specific index */
  CListNode *node;
  ResultCode rc = _get_node_at(list, index, &node);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Retrieve data from index and set value for output pointer */
  *out_value = node->data;

  return kSuccess;
}
/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * CircularLinkedList_PushFront - Insert an element at the beginning of the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Handle empty list case (point to if self)
 * 4. Handle non-empty list case (insert at front, maintain circularity
 * 5. Increment size
 *
 * In a circular list, inserting at the front requires updating tail->next
 * to point to the new head)
 *
 * @param list Circular linked list to modify
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_PushFront(CircularLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  CListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Handle empty list case */
  if (list->head == NULL) {
    /* Empty list - new node points to ifself */
    node->next = node;
    list->head = node;
    list->tail = node;
  } else {
    /* Step 4: Non-empty list - insert at front
     * New node points to current head *
     * Tail's next points to new node (maintain circularity)
     */
    node->next = list->head;
    list->head = node;
    list->tail->next = node; /* Update tail's next to new head */
  }

  /* Step 5: Increment size */
  list->size++;

  return kSuccess;
}

/**
 * CircularLinkedList_PushBack - Inserts an element at the end of the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Create new node
 * 3. Handle empty list case (point to itself)
 * 4.Handle non-empty list case (insert at back, maintain circularity)
 * 5 Increment size
 *
 * @param list Circular linked list to modify
 * @param value Pointer to value to insert
 * @result Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_PushBack(CircularLinkedList *list, void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new node */
  CListNode *node = _create_node(value);
  if (value == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Handle empty list case */
  if (list->tail == NULL) {
    /* Empty list - new node points to ifself */
    node->next = list->head;
    list->head = node;
    list->tail = node;
  } else {
    /* Step 4: Non-empty list - insert at back
     * New node points to head (maintain circularity)
     * Current tail's next point to new node
     */
    node->next = list->head;
    list->tail->next = node;
    list->tail = node;
  }

  /* Step 5: Increment size */
  list->size++;

  return kSuccess;
}

/**
 * CircularLinkedList_PopFront - Removes the first element from the list
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list not empty
 * 3. Handle single-element list case
 * 4. For mutil-element list , remove front and update tai->next
 * 5. Decrement size
 *
 * @param list Circular linked lis to modify
 * @result Return code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_PopFront(CircularLinkedList *list) {
  /* Step 1: Validate parameters */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Handle single-element list case */
  if (list->tail == NULL) {
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return kSuccess;
  }

  /* STep 4: Remove front npde from mutil-element list */
  CListNode *old_head = list->head;
  list->head = list->head->next;
  list->tail->next = list->head; /* Maintain circular property */

  free(old_head);
  list->size--;

  return kSuccess;
}

/**
 * CircularLinkedList_PopBack - Removes the last element from the list
 *
 * Implementation flow:
 * 1. Validat parameters
 * 2. Check list is not empty
 * 3. Handle single-element list case
 * 4. For multi-element list, find second-to-last node
 * 5. Update tail and maintain circularity
 * 6. Decrement size
 *
 * @param list Circular linked list to modify
 * @return Result code
 *
 * @complexity O(n) - (Neeed to find second-to-last node )
 */
ResultCode CircularLinkedList_PopBack(CircularLinkedList *list) {
  /* Step 1: Validate parameters */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Handle singly-element list case */
  if (list->size == 1) {
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return kSuccess;
  }

  /* Step 4: Find second-to-last node
   * Traverse from head until current->next is the tail
   */
  CListNode *current = list->head;

  while (current->next != list->tail) {
    current = current->next;
  }

  /* Step 5: Remove tail node */
  free(list->tail);
  list->tail = current;
  list->tail->next = list->head; /* Maintain circular property */
  list->size++;

  return kSuccess;
}

/**
 * CircularLinkedList_InsertAt - Inserts an element at the specificed index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check index bounds
 * 3. Handle special cases (front, back)
 * 4. For middle insertion. find node before insertion point
 * 5. Create new node and link it
 * 6. Increment size
 *
 * @param list Circular linked list to modify
 * @param index Position to insert (0 <= index <= size)
 * @param value Pointer to value to insert
 * @return Result code
 *
 * @complexity O(n) - worst case
 */
ResultCode CircularLinkedList_InsertAt(CircularLinkedList *list, size_t index,
                                       void *value) {
  /* Step 1: Validate parameters */
  if (list == NULL || value == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index > list->size) {
    return kInvalidIndex;
  }

  /* Step 3: Handle special cases for efficiency */
  if (index == 0) {
    return CircularLinkedList_PushFront(list, value);
  }

  if (index == list->size) {
    return CircularLinkedList_PushBack(list, value);
  }

  /* Step 4: Insert in middle - find node before insertion point */
  CListNode *prev;
  ResultCode rc = _get_node_at(list, index - 1, &prev);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 5: Create new node */
  CListNode *node = _create_node(value);
  if (node == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 6: Link the new node */
  node->next = prev->next;
  prev->next = node;

  list->size++;

  return kSuccess;
}

/**
 * CircularLinkedList_RemoveAt - Removes an elements at the specified index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check index bounds
 * 3. Handle special cases (front, back)
 * 4. For middle removal, find node before removal point
 * 5. Unlink and free the target node
 * 6. Decrement size
 *
 * @param list Circular linked list to modify
 * @param index Position to remove (0 <= index < size)
 * @return Result code
 *
 * @complexity O(n) - worse case
 */
ResultCode CircularLinkedList_RemoveAt(CircularLinkedList *list, size_t index) {
  /* Step 1: Validate parameter */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check index bounds */
  if (index >= list->size) {
    return kInvalidIndex;
  }

  /* Step 3: Handle special cases */
  if (index == 0) {
    return CircularLinkedList_PopFront(list);
  }

  if (index == list->size) {
    return CircularLinkedList_PopBack(list);
  }

  /* Step 4: Find node before the one to remove */
  CListNode *prev;
  ResultCode rc = _get_node_at(list, index, &prev);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 5: Remove node */
  CListNode *to_remove = prev->next;
  prev->next = to_remove->next;

  /* Step 6:  Update tail if removing last node */
  if (to_remove == list->tail) {
    list->tail = prev;
  }

  free(to_remove);
  list->size--;

  return kSuccess;
}

/* ============================================================================
 * CIRCULAR-SPECIFIC OPERATIONS
 * ============================================================================
 */

/**
 * CircularLinkedList_Rotates the list by moving head forward
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list not empty
 * 3. Normalize steps  (steps % size)
 * 4. Find new head and new tail
 * 5. Update head, tail and maintain circularity
 *
 * This a unique operation only possition with circular lists
 *
 * EXAMPLE: Rotate by 2 steps on [A,B,C,D,E]
 * After rotation: [C,D,E,A,B]
 * (Element at index 2 becomes new head)
 *
 * @param: list Circular linked list to modify
 * @param steps Numbers of steps to rotate forward
 * @return Result code
 *
 * @complexity O(steps % size)
 */
ResultCode CircularLinkedList_Rotate(CircularLinkedList *list, size_t steps) {
  /* Step 1: Validate parameters */
  if (list == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Normalize steps (no need to rotate full circle) */
  if (steps == 0 || steps == list->size) {
    return kSuccess;
  }

  steps = steps % list->size;

  /* Step 4: Find new head (node at index 'steps') */
  CListNode *new_head = list->head;
  for (size_t i = 0; i < steps; i++) {
    new_head = new_head->next;
  }

  /* Step 5: Find new tail (node before new head) */
  CListNode *new_tail = list->head;
  while (new_tail->next != new_head) {
    new_tail = new_tail->next;
  }

  /* STep 6: Update head and tail, maintain circularity */
  list->head = new_head;
  list->tail = new_tail;
  list->tail->next = list->head;

  return kSuccess;
}

/**
 * CircularLinkedList_Split - Splits circular list into two at  given index
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check index is valid (0 < index < size)
 * 3. Create two new lists
 * 4. Find split point
 * 5. Assign nodes to each list
 * 6. Clear original list
 *
 * @param list Circular linked list to split (will be cleared)
 * @param index Position to split at (0 < index < size)
 * @param out_list1 Output for first part (indices 0 to index -1)
 * @param out_list2 Output for second part (indices index to size - 1)
 * @return Result code
 *
 * @complexity O(index)
 */
ResultCode CircularLinkedList_Split(CircularLinkedList *list, size_t index,
                                    CircularLinkedList **out_list1,
                                    CircularLinkedList **out_list2) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_list1 == NULL || out_list2 == NULL) {
    return kNullParameter;
  }

  /* Step 2: Validate split index */
  if (index == 0 || index >= list->size) {
    return kInvalidArgument;
  }

  /* Step 3: Create two new lists */
  ResultCode rc = CircularLinkedList_Create(out_list1);
  if (rc != kSuccess) {
    return rc;
  }

  rc = CircularLinkedList_Create(out_list2);
  if (rc != kSuccess) {
    CircularLinkedList_Destroy(*out_list1);
    return rc;
  }

  CircularLinkedList *list1 = *out_list1;
  CircularLinkedList *list2 = *out_list2;

  /* Step 4: Find split point (bide at index - 1) */
  CListNode *split_node;
  rc = _get_node_at(list, index - 1, &split_node);
  if (rc != kSuccess) {
    CircularLinkedList_Destroy(list1);
    CircularLinkedList_Destroy(list2);
    return rc;
  }

  CListNode *second_head = split_node->next;

  /* Step 5: Assign nodes to first list (head to slit_node) */
  list1->head = list->head;
  list1->tail = split_node;
  list1->tail->next = list1->head;
  list1->size = index;

  /* Step 6: Assign node to second list (second_head to tail) */
  list2->head = second_head;
  list2->tail = list->tail;
  list2->tail->next = list->head;
  list2->size = list->size - index;

  /* Step 7: Clear original list */
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;

  return kSuccess;
}

/**
 * CircularLinkedList_Merge - Merges two circular lists into one
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Handle empty list cases
 * 3. Link tail of list1 to head of list2
 * 4. Link tail of list2 to head of list1
 *
 * @param list1 First circular list (will contain merged result
 * @param list2 Second circular list (will be cleared)
 * @reurn Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_Merge(CircularLinkedList *list1,
                                    CircularLinkedList *list2) {
  /* Step 1: Validate parameters */
  if (list1 == NULL || list2 == NULL) {
    return kNullParameter;
  }

  /* Step 2: Handle empty list cases */
  if (list2->size == 0) {
    return kSuccess;
  }

  if (list1->size == 0) {
    /* Copy list1 into list1 */
    list1->head = list2->head;
    list1->tail = list2->tail;
    list1->size = list2->size;

    /* Clear list2 */
    list2->head = NULL;
    list2->tail = NULL;
    list2->size = 0;

    return kSuccess;
  }

  /* Step 3: Link the lists together */
  list1->tail->next = list2->head;
  list2->tail->next = list1->head;

  /* Step 4: Update list1's tail and size */
  list1->tail = list2->tail;
  list1->size = list2->size;

  /* Step 5: Clear list2 */
  list2->head = NULL;
  list2->tail = NULL;
  list2->size = 0;

  return kSuccess;
}

/* CircularLinkedList_IsValid - Validates circular list priperties
 *
 * Check:
 * - For empty list: head == NULL && tail == NULL
 * - For non-empty: tail != NULL && tail->next == head
 *
 * @param list Circular linked list to validate
 * @return true if valid circular list, false otherwise
 *
 * @complexity O(1)
 */

bool CircularLinkedList_IsValid(const CircularLinkedList *list) {
  if (list == NULL) {
    return false;
  }

  if (list->size == 0) {
    return list->head == NULL && list->tail == NULL;
  }

  /* Check tail->next == head (circular property) */
  if (list->tail == NULL || list->tail->next != list->head) {
    return false;
  }

  return true;
}

/** CircularLinkedList_GetPrevious - Finds the previous node
 *
 * @param list Circular linked list to search
 * @param node Node to find previous of
 * @param out_prev Output pointer to receive previous node
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode CircularLinkedList_GetPrevious(const CircularLinkedList *list,
                                          CListNode *node,
                                          CListNode **out_prev) {
  /* Step 1: Validate parameters */
  if (list == NULL || node == NULL || out_prev == NULL) {
    return kNullParameter;
  }

  /* Step 2: Linner search for previous node */
  CListNode *current = list->head;
  size_t count = 0;

  do {
    if (current->next == node) {
      *out_prev = current;
      return kSuccess;
    }
    current = current->next;
    count++;
  } while (current != list->head && count < list->size);

  return kNotFound;
}

/**
 * CircularLinkedList_GetNext - Returns the next node (For iteration)
 *
 * @param list Circular linked list (for validation)
 * @param node Current node (must be in the list)
 * @param out_next Output pointer to receive next node
 * @return Result code
 * @complexity O(1)
 */
ResultCode CircularLinkedList_GetNext(const CircularLinkedList *list,
                                      CListNode *node, CListNode **out_next) {
  /* Step 1: Validate parameters */
  if (list == NULL || node == NULL || out_next == NULL) {
    return kNullParameter;
  }

  /* Step 2: Get next node and return */
  *out_next = node->next;

  return kSuccess;
}

/**
 * CircularLinkedList_RemoveNode - Removes a specific node
 *
 * Implementation flow:
 * 1. Validate parameters
 * 2. Check list not empty
 * 3. Handle special case - single node
 * 4. Find previous node
 * 5. Remove node
 * 6. Update head/tail if needed
 *
 * @param list Circular linked list to modify
 * @param node Node to remove (must be in the list )
 * @return Result code
 *
 * @complexity O (n) - needs previous node via traversal
 */
ResultCode CircularLinkedList_RemoveNode(CircularLinkedList *list,
                                         CListNode *node) {
  /* Step 1: Validate parameters */
  if (list == NULL || node == NULL) {
    return kNullParameter;
  }

  /* Step 2: Check list not empty */
  ResultCode rc = _check_not_empty(list);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 3: Special case - single node */
  if (list->size == 1 && node == list->head) {
    free(node);
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return kSuccess;
  }

  /* Step 4: Find previous node */
  CListNode *prev;
  rc = CircularLinkedList_GetPrevious(list, node, &prev);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 5: Remove node */
  prev->next = node->next;

  /* Step 6: Update head/tail if needed */
  if (node == list->head) {
    list->head = node->next;
  }

  if (node == list->tail) {
    list->tail = prev;
  }

  free(node);
  list->size--;

  return kSuccess;
}

/**
 * CircularLinkedList_Find - Find a node by its data
 *
 * Implementation flow:
 * 1.Validate parameters
 * 2. Handle empty list
 * 3. Traversal and search
 *
 * @param list Circular linked list to search
 * @param data Data to find
 * @param compare Comparison function (NULL for pointer compasition)
 * @param out_node Output ponter to receive found node
 * @return Result code
 *
 * complexity O(n)
 */

ResultCode CircularLinkedList_Find(const CircularLinkedList *list,
                                   const void *data,
                                   int (*compare)(const void *, const void *),
                                   CListNode **out_node) {
  /* Step 1: Validate parameters */
  if (list == NULL || out_node == NULL) {
    return kNullParameter;
  }

  /* Step 2: Handle empty list */
  if (list->size == 0) {
    return kNotFound;
  }

  /* Step 3: Traversal and search */
  CListNode *current = list->head;
  size_t count = 0;

  do {
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
    count++;
  } while (current != list->head && count < list->size);

  return kNotFound;
}

/* ============================================================================
 * TRAVERSAL HELPERS
 * ============================================================================
 */

/**
 * CircularLinkedList_Traverse - Traverse - tje entrie circular list
 *
 * @param list Circular linked list to traverse
 * @param visit Callback function for each node
 * @param user_data User data to pass to callback
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode CircularLinkedList_Traverse(const CircularLinkedList *list,
                                       CircularVisitFunction visit,
                                       void *user_data) {
  /* Step 1: Validate parameters */
  if (list == NULL || visit == NULL) {
    return kNullParameter;
  }

  /* Step 2: Handle empty list */
  if (list->size == 0) {
    return kSuccess;
  }

  /* Step 3: Traverse all nodes once
   * Stop when we return to head (detects full circle)
   */
  CListNode *current = list->head;
  size_t count = 0;

  do {
    visit(current->data, user_data);
    current = current->next;
    count++;
  } while (current != list->head && count < list->size);

  return kSuccess;
}

/**
 * CircularLinkedList_TraverseFrom - Traverses starting from arbitrary node
 *
 * @param list Circular linked list to traverse
 * @param start Starting node (NULL starts from head)
 * @param visit Callback function for earch node
 * @param user_data User data to pass to callback
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode CircularLinkedList_TraverseFrom(const CircularLinkedList *list,
                                           CListNode *start,
                                           CircularVisitFunction visit,
                                           void *user_data) {
  /* Step 1: Validate parameters */
  if (list == NULL || visit == NULL) {
    return kNullParameter;
  }

  /* Step 2: Handle empty list */
  if (list->size == 0) {
    return kSuccess;
  }

  /* Step 3: Start from specified node or head */
  CListNode *current = (start == NULL) ? list->head : start;
  CListNode *start_node = current;
  size_t count = 0;

  do {
    visit(current->data, user_data);
    current = current->next;
    count++;
  } while (current != start_node && count < list->size);

  return kSuccess;
}
