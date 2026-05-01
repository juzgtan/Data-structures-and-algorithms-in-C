#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include "utils/result_code.h"
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * COLOR DEFINITION
 * ============================================================================
 */

/**
 * @brief Red-Black Tree node colors
 */
typedef enum RBColor {
  RB_RED = 0,  /**< Red node (cannot have red children) */
  RB_BLACK = 1 /**< Black node */
} RBColor;

/* ============================================================================
 * STRUCTURES
 * ============================================================================
 */

/**
 * @brief Red-Black Tree node
 */
typedef struct RBNode {
  void *data;            /**< Pointer to stored data */
  RBColor color;         /**< Node color (RED or BLACK) */
  struct RBNode *left;   /**< Pointer to left child */
  struct RBNode *right;  /**< Pointer to right child */
  struct RBNode *parent; /**< Pointer to parent node */
} RBNode;

/**
 * @brief Red-Black Tree structure
 *
 * A self-balancing binary search tree with guaranteed O(log n) operations.
 *
 * Properties maintained:
 * 1. Every node is RED or BLACK
 * 2. Root is BLACK
 * 3. All leaves (NIL) are BLACK
 * 4. RED nodes cannot have RED children
 * 5. All paths from root to leaf have the same number of BLACK nodes
 */
typedef struct {
  RBNode *root; /**< Pointer to the root node */
  RBNode *nil;  /**< Sentry NIL node (always BLACK) */
  size_t size;  /**< Number of nodes in the tree */
  int (*compare)(const void *,
                 const void *); /**< Element comparison function */
} RedBlackTree;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new empty Red-Black Tree
 * @param compare Comparison function (NULL = default integer comparison)
 * @param result Output pointer to receive the new RedBlackTree
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode RedBlackTree_Create(int (*compare)(const void *, const void *),
                               RedBlackTree **result);

/**
 * @brief Destroys the Red-Black Tree and frees all associated memory
 * @param tree RB-Tree to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void RedBlackTree_Destroy(RedBlackTree *tree);

/**
 * @brief Removes all nodes from the Red-Black Tree
 * @param tree RB-Tree to clear
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void RedBlackTree_Clear(RedBlackTree *tree);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of nodes in the RB-Tree
 * @param tree RB-Tree to examine
 * @return Number of nodes (0 if tree is NULL)
 *
 * @complexity O(1)
 */
size_t RedBlackTree_Size(const RedBlackTree *tree);

/**
 * @brief Checks if the RB-Tree is empty
 * @param tree RB-Tree to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool RedBlackTree_IsEmpty(const RedBlackTree *tree);

/**
 * @brief Returns the height of the RB-Tree
 * @param tree RB-Tree to examine
 * @return Height (0 if empty)
 *
 * @complexity O(n)
 */
size_t RedBlackTree_Height(const RedBlackTree *tree);

/**
 * @brief Returns the black height (number of BLACK nodes from root to leaf)
 * @param tree RB-Tree to examine
 * @return Black height
 *
 * @complexity O(n)
 */
size_t RedBlackTree_BlackHeight(const RedBlackTree *tree);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Inserts a new element into the RB-Tree
 * @param tree RB-Tree to modify
 * @param data Pointer to data to insert
 * @return Result code (kDuplicate if element already exists)
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_Insert(RedBlackTree *tree, void *data);

/**
 * @brief Removes an element from the RB-Tree
 * @param tree RB-Tree to modify
 * @param data Data to remove
 * @return Result code
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_Remove(RedBlackTree *tree, const void *data);

/**
 * @brief Removes a specific node from the RB-Tree
 * @param tree RB-Tree to modify
 * @param node Node to remove (must be in the tree)
 * @return Result code
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_RemoveNode(RedBlackTree *tree, RBNode *node);

/* ============================================================================
 * LOOKUP FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Checks if the RB-Tree contains a specific value
 * @param tree RB-Tree to search
 * @param data Data to find
 * @return true if found, false otherwise
 *
 * @complexity O(log n)
 */
bool RedBlackTree_Contains(const RedBlackTree *tree, const void *data);

/**
 * @brief Finds and returns a pointer to the data
 * @param tree RB-Tree to search
 * @param data Data to find
 * @param out Output pointer to receive found data
 * @return Result code
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_Find(const RedBlackTree *tree, const void *data,
                             void **out);

/**
 * @brief Returns the minimum value in the RB-Tree
 * @param tree RB-Tree to examine
 * @param out Output pointer to receive minimum value
 * @return Result code
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_GetMin(const RedBlackTree *tree, void **out);

/**
 * @brief Returns the maximum value in the RB-Tree
 * @param tree RB-Tree to examine
 * @param out Output pointer to receive maximum value
 * @return Result code
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_GetMax(const RedBlackTree *tree, void **out);

/* ============================================================================
 * TRAVERSAL FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Performs inorder traversal (sorted order)
 * @param tree RB-Tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode RedBlackTree_Inorder(const RedBlackTree *tree, void **output,
                                size_t *out_len);

/**
 * @brief Performs preorder traversal (root -> left -> right)
 * @param tree RB-Tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode RedBlackTree_Preorder(const RedBlackTree *tree, void **output,
                                 size_t *out_len);

/**
 * @brief Performs postorder traversal (left -> right -> root)
 * @param tree RB-Tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode RedBlackTree_Postorder(const RedBlackTree *tree, void **output,
                                  size_t *out_len);

/**
 * @brief Performs level-order traversal (BFS)
 * @param tree RB-Tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode RedBlackTree_LevelOrder(const RedBlackTree *tree, void **output,
                                   size_t *out_len);

/* ============================================================================
 * VALIDATION FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Validates all Red-Black Tree properties
 * @param tree RB-Tree to validate
 * @return true if valid RB-Tree, false otherwise
 *
 * @complexity O(n)
 */
bool RedBlackTree_IsValidRBTree(const RedBlackTree *tree);

/**
 * @brief Checks if the tree is balanced
 * @param tree RB-Tree to check
 * @return true if balanced, false otherwise
 *
 * @complexity O(1)
 */
bool RedBlackTree_IsBalanced(const RedBlackTree *tree);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the root node of the RB-Tree
 * @param tree RB-Tree to examine
 * @param out_root Output pointer to receive root node
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode RedBlackTree_GetRoot(const RedBlackTree *tree, RBNode **out_root);

/**
 * @brief Finds the inorder successor of a value (next larger value)
 * @param tree RB-Tree to search
 * @param data Data to find successor for
 * @param out Output pointer to receive successor data
 * @return Result code
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_GetSuccessor(const RedBlackTree *tree, const void *data,
                                     void **out);

/**
 * @brief Finds the inorder predecessor of a value (next smaller value)
 * @param tree RB-Tree to search
 * @param data Data to find predecessor for
 * @param out Output pointer to receive predecessor data
 * @return Result code
 *
 * @complexity O(log n)
 */
ResultCode RedBlackTree_GetPredecessor(const RedBlackTree *tree,
                                       const void *data, void **out);

#ifdef __cplusplus
}
#endif

#endif /* RED_BLACK_TREE_H */
