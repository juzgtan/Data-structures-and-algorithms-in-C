#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include "utils/result_code.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * STRUCTURES
 * ============================================================================
 */

/**
 * @brief Binary Search Tree node
 */
typedef struct BSTNode {
  void *data;            /**< Pointer to stored data */
  struct BSTNode *left;  /**< Pointer to left child (smaller values) */
  struct BSTNode *right; /**< Pointer to right child (larger values) */
} BSTNode;

/**
 * @brief Binary Search Tree structure
 *
 * A binary tree that maintains the BST property:
 * - Left subtree contains values smaller than the node
 * - Right subtree contains values larger than the node
 * - No duplicate values allowed
 */
typedef struct {
  BSTNode *root; /**< Pointer to the root node */
  size_t size;   /**< Number of nodes in the tree */
  int (*compare)(const void *,
                 const void *); /**< Element comparison function */
} BinarySearchTree;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new empty BST
 * @param compare Comparison function (NULL = default integer comparison)
 * @param result Output pointer to receive the new BinarySearchTree
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinarySearchTree_Create(int (*compare)(const void *, const void *),
                                   BinarySearchTree **result);

/**
 * @brief Destroys the BST and frees all associated memory
 * @param tree BST to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void BinarySearchTree_Destroy(BinarySearchTree *tree);

/**
 * @brief Removes all nodes from the BST
 * @param tree BST to clear
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void BinarySearchTree_Clear(BinarySearchTree *tree);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of nodes in the BST
 * @param tree BST to examine
 * @return Number of nodes (0 if tree is NULL)
 *
 * @complexity O(1)
 */
size_t BinarySearchTree_Size(const BinarySearchTree *tree);

/**
 * @brief Checks if the BST is empty
 * @param tree BST to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool BinarySearchTree_IsEmpty(const BinarySearchTree *tree);

/**
 * @brief Returns the height of the BST
 * @param tree BST to examine
 * @return Height (0 if empty)
 *
 * @complexity O(n)
 */
size_t BinarySearchTree_Height(const BinarySearchTree *tree);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Inserts a new element into the BST
 * @param tree BST to modify
 * @param data Pointer to data to insert
 * @return Result code (kDuplicate if element already exists)
 *
 * @complexity O(h) where h is tree height
 */
ResultCode BinarySearchTree_Insert(BinarySearchTree *tree, void *data);

/**
 * @brief Removes an element from the BST
 * @param tree BST to modify
 * @param data Data to remove
 * @return Result code
 *
 * @complexity O(h) where h is tree height
 */
ResultCode BinarySearchTree_Remove(BinarySearchTree *tree, const void *data);

/**
 * @brief Removes a specific node from the BST
 * @param tree BST to modify
 * @param node Node to remove (must be in the tree)
 * @return Result code
 *
 * @complexity O(h) where h is tree height
 */
ResultCode BinarySearchTree_RemoveNode(BinarySearchTree *tree, BSTNode *node);

/* ============================================================================
 * LOOKUP FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Checks if the BST contains a specific value
 * @param tree BST to search
 * @param data Data to find
 * @return true if found, false otherwise
 *
 * @complexity O(h)
 */
bool BinarySearchTree_Contains(const BinarySearchTree *tree, const void *data);

/**
 * @brief Finds and returns a pointer to the data
 * @param tree BST to search
 * @param data Data to find
 * @param out Output pointer to receive found data
 * @return Result code
 *
 * @complexity O(h)
 */
ResultCode BinarySearchTree_Find(const BinarySearchTree *tree, const void *data,
                                 void **out);

/**
 * @brief Returns the minimum value in the BST
 * @param tree BST to examine
 * @param out Output pointer to receive minimum value
 * @return Result code
 *
 * @complexity O(h)
 */
ResultCode BinarySearchTree_GetMin(const BinarySearchTree *tree, void **out);

/**
 * @brief Returns the maximum value in the BST
 * @param tree BST to examine
 * @param out Output pointer to receive maximum value
 * @return Result code
 *
 * @complexity O(h)
 */
ResultCode BinarySearchTree_GetMax(const BinarySearchTree *tree, void **out);

/* ============================================================================
 * TRAVERSAL FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Performs inorder traversal (sorted order)
 * @param tree BST to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinarySearchTree_Inorder(const BinarySearchTree *tree, void **output,
                                    size_t *out_len);

/**
 * @brief Performs preorder traversal (root -> left -> right)
 * @param tree BST to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinarySearchTree_Preorder(const BinarySearchTree *tree,
                                     void **output, size_t *out_len);

/**
 * @brief Performs postorder traversal (left -> right -> root)
 * @param tree BST to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinarySearchTree_Postorder(const BinarySearchTree *tree,
                                      void **output, size_t *out_len);

/**
 * @brief Performs level-order traversal (BFS)
 * @param tree BST to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinarySearchTree_LevelOrder(const BinarySearchTree *tree,
                                       void **output, size_t *out_len);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the root node of the BST
 * @param tree BST to examine
 * @param out_root Output pointer to receive root node
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinarySearchTree_GetRoot(const BinarySearchTree *tree,
                                    BSTNode **out_root);

/**
 * @brief Validates if the tree satisfies BST property
 * @param tree BST to validate
 * @return true if valid BST, false otherwise
 *
 * @complexity O(n)
 */
bool BinarySearchTree_IsBST(const BinarySearchTree *tree);

/**
 * @brief Finds the inorder successor of a value (next larger value)
 * @param tree BST to search
 * @param data Data to find successor for
 * @param out Output pointer to receive successor data
 * @return Result code
 *
 * @complexity O(h)
 */
ResultCode BinarySearchTree_GetSuccessor(const BinarySearchTree *tree,
                                         const void *data, void **out);

/**
 * @brief Finds the inorder predecessor of a value (next smaller value)
 * @param tree BST to search
 * @param data Data to find predecessor for
 * @param out Output pointer to receive predecessor data
 * @return Result code
 *
 * @complexity O(h)
 */
ResultCode BinarySearchTree_GetPredecessor(const BinarySearchTree *tree,
                                           const void *data, void **out);

#ifdef __cplusplus
}
#endif

#endif /* BINARY_SEARCH_TREE_H */
