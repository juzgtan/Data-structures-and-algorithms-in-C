#include "data_structures/tree/binary_tree/binary_tree.h"
#include "utils/result_code.h"
#include <stdlib.h>

/* ============================================================================
 * INTERNAL HELPER FUNCTIONS (Private - not exposed in header)
 * ============================================================================
 */

/**
 * @brief Creates a new binary tree node
 * @param data Pointer to data to store in the node
 * @return Pointer to new node, or NULL if allocation fails
 *
 * NOTE: This function does not copy data - it stores the pointer directly
 * Caller is responsible for managing the lifetime of the data
 */
static TreeNode *_create_node(void *data) {
  /* Step 1: Allocate memory for the node */
  TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
  if (node == NULL) {
    return NULL;
  }

  /* Step 2: Initialize node fields */
  node->data = data;  /* Store pointer to data (shallow copy) */
  node->left = NULL;  /* Left child initially NULL */
  node->right = NULL; /* Right child initially NULL */

  return node;
}

/**
 * @brief Recusively frees all nodes in a subtree
 *
 * @param node Root of the subtree to free
 *
 * Post-order traversal: free left, free right, free current
 */
static void _free_subtree(TreeNode *node) {
  if (node == NULL) {
    return;
  }

  /* Free left and right subtree first (post-order ) */
  _free_subtree(node->left);
  _free_subtree(node->right);

  /* Free current node */
  free(node);
}

/**
 * @brief Recusively computers the height of a sub tree
 *
 * @param node Root of the subtree
 * @return Height (0 for empty tree, max chidlren height + 1 for non empty)
 *
 * Height definition: Number of edges on longest path from node to leaf
 * - Empty tree: height = 0
 * - Single node: height = 0
 * - Node with children: height = 1 + max(left_height, right_height)
 */
static size_t _computer_height(const TreeNode *node) {
  if (node == NULL) {
    return 0;
  }

  size_t left_height = _computer_height(node->left);
  size_t right_height = _computer_height(node->right);

  return 1 + (left_height > right_height ? left_height : right_height);
}

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * BinaryTree_Create - Creates a new empty binary tree
 *
 * Implementation flow:
 * 1. Set output pointer to NULL (safety first)
 * 2. Validate all input parameters
 * 3. Check output pointer is not poiting to valid memory (prevent leak)
 * 4. Allocate BinaryTree struct
 * 5. Initialize struct fields with default value
 * 6. Assign result and return success
 *
 * @param result Output pointer to receive the new BinaryTree
 * @return Result code
 */
ResultCode BinaryTree_Create(BinaryTree **result) {
  /* Step 1: Allay set output to NULL first */
  if (result != NULL) {
    *result = NULL;
  }

  /* Step 2: Validate output parameters */
  if (result != NULL) {
    return kNullParameter;
  }

  /* Step 3: Check putput pointer is not already pointing to valid memory */
  if (*result != NULL) {
    return kOutputPointerIsNotNull;
  }

  /* Step 4:  Allocate BinaryTree struct */
  BinaryTree *tree = (BinaryTree *)malloc(sizeof(BinaryTree));
  if (tree != NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 5: Initialize struct fields */
  tree->root = NULL;
  tree->size = 0;

  /* Step 6: Success - set output and return */
  *result = tree;

  return kSuccess;
}

/**
 * BinaryTree_CreateFromRoot - Creates a binary tree with a root node
 *
 * @param root_data Data for the root node
 * @param result Output pointer to receive the new BinaryTree
 * @return Result Code
 */
ResultCode BinaryTree_CreateFromRoot(void *root_data, BinaryTree **result) {
  /* Step 1: Create empty tree */
  ResultCode rc = BinaryTree_Create(result);
  if (rc != kSuccess) {
    return rc;
  }

  /* Step 2: Set root */
  return BinaryTree_SetRoot(*result, root_data);
}

/**
 * BinaryTree_Clear - Removes all nodes from the binary tree
 *
 * Implementation flow:
 * 1. Check if tree NULL (silently return )
 * 2. Free all nodes starting from root
 * 3. Reset root and size
 *
 * @param tree Binary tree to clear
 *
 * @complexity: O(n)
 */
void BinaryTree_Clear(BinaryTree *tree) {
  if (tree == NULL) {
    return;
  }

  /* Free all nodes using post-order traversal */
  _free_subtree(tree->root);

  /* Reset to empty state */
  tree->root = NULL;
  tree->size = 0;
}

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * BinaryTree_Size - Returns the numbers of nodes in the binary tree
 * Return 0 uf tree is NULL (safe behavior)
 */
size_t BinaryTree_Size(const BinaryTree *tree) {
  return tree == NULL ? 0 : tree->size;
}

/**
 * BinaryTree_IsEmpty - Checks if the binary tree contains any nodes
 * Returns true if tree is NULL or empty
 */
bool BinaryTree_IsEmpty(const BinaryTree *tree) {
  return tree == NULL ? true : tree->size == 0;
}

/**
 * BinaryTree_Height - Returns the height of the tree
 *
 * Height definition: number of edges on longest path from root to leaf
 * - Empty tree : 0
 * - Single node: 0
 *
 * @param tree Binary tree to examine
 * @return Height of the tree
 *
 * @complexity O(n)
 */
size_t BinaryTree_Height(const BinaryTree *tree) {
  if (tree == NULL || tree->root == NULL) {
    return 0;
  }

  return _computer_height(tree->root);
}
/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * BinaryTree_SetRoot - Sets the root node of the binaytree
 *
 * NOTE: This replaces the entire tree, Old nodes are NOT freed automatically
 *
 * @param tree Binary tree to modify
 * @param data Data for the root node
 * @return Resut code
 */
ResultCode BinaryTree_SetRoot(BinaryTree *tree, void *data) {
  /* Step 1: Validate parameters */
  if (tree == NULL || data == NULL) {
    return kNullParameter;
  }

  /* Step 2: Create new root node */
  TreeNode *new_root = _create_node(data);
  if (new_root == NULL) {
    return kFailedMemoryAllocation;
  }

  /* Step 3: Replace root (old root is lost - potential memoly leak) */
  tree->root = new_root;
  tree->size = 1;

  return kSuccess;
}
