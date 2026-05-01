#ifndef BINARY_TREE_H
#define BINARY_TREE_H

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
 * @brief Binary tree node
 */
typedef struct TreeNode {
  void *data;             /**< Pointer to stored data */
  struct TreeNode *left;  /**< Pointer to left child */
  struct TreeNode *right; /**< Pointer to right child */
} TreeNode;

/**
 * @brief Binary tree structure (general binary tree, not BST)
 *
 * A general binary tree without ordering constraints.
 */
typedef struct {
  TreeNode *root; /**< Pointer to the root node */
  size_t size;    /**< Number of nodes in the tree */
} BinaryTree;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new empty binary tree
 * @param result Output pointer to receive the new BinaryTree
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_Create(BinaryTree **result);

/**
 * @brief Creates a binary tree with a root node
 * @param root_data Data for the root node
 * @param result Output pointer to receive the new BinaryTree
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_CreateFromRoot(void *root_data, BinaryTree **result);

/**
 * @brief Destroys the binary tree and frees all associated memory
 * @param tree Binary tree to destroy (can be NULL)
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void BinaryTree_Destroy(BinaryTree *tree);

/**
 * @brief Removes all nodes from the tree
 * @param tree Binary tree to clear
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n)
 */
void BinaryTree_Clear(BinaryTree *tree);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of nodes in the tree
 * @param tree Binary tree to examine
 * @return Number of nodes (0 if tree is NULL)
 *
 * @complexity O(1) (cached value)
 */
size_t BinaryTree_Size(const BinaryTree *tree);

/**
 * @brief Checks if the tree is empty
 * @param tree Binary tree to examine
 * @return true if empty or NULL, false otherwise
 *
 * @complexity O(1)
 */
bool BinaryTree_IsEmpty(const BinaryTree *tree);

/**
 * @brief Returns the height of the tree (number of edges on longest path)
 * @param tree Binary tree to examine
 * @return Height (0 if empty)
 *
 * @complexity O(n)
 */
size_t BinaryTree_Height(const BinaryTree *tree);

/**
 * @brief Returns the number of leaf nodes
 * @param tree Binary tree to examine
 * @return Number of leaf nodes
 *
 * @complexity O(n)
 */
size_t BinaryTree_CountLeaves(const BinaryTree *tree);

/**
 * @brief Returns the diameter of the tree (longest path between any two nodes)
 * @param tree Binary tree to examine
 * @return Diameter in edges
 *
 * @complexity O(n)
 */
size_t BinaryTree_Diameter(const BinaryTree *tree);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Sets the root node of the binary tree (replaces existing)
 * @param tree Binary tree to modify
 * @param data Data for the root node
 * @return Result code
 *
 * @warning Old nodes are NOT freed automatically
 * @complexity O(1)
 */
ResultCode BinaryTree_SetRoot(BinaryTree *tree, void *data);

/**
 * @brief Inserts a node as the left child of a parent
 * @param tree Binary tree to modify
 * @param parent Parent node (must be in the tree)
 * @param data Data for the new node
 * @param out_node Output pointer to receive the new node (optional)
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_InsertLeft(BinaryTree *tree, TreeNode *parent, void *data,
                                 TreeNode **out_node);

/**
 * @brief Inserts a node as the right child of a parent
 * @param tree Binary tree to modify
 * @param parent Parent node (must be in the tree)
 * @param data Data for the new node
 * @param out_node Output pointer to receive the new node (optional)
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_InsertRight(BinaryTree *tree, TreeNode *parent,
                                  void *data, TreeNode **out_node);

/**
 * @brief Removes a node and its entire subtree
 * @param tree Binary tree to modify
 * @param node Node to remove (must be in the tree)
 * @return Result code
 *
 * @note Does NOT free the data pointed to by nodes
 * @complexity O(n) for finding parent
 */
ResultCode BinaryTree_RemoveNode(BinaryTree *tree, TreeNode *node);

/* ============================================================================
 * LOOKUP FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Finds and returns a node containing the given data
 * @param tree Binary tree to search
 * @param data Data to find
 * @param compare Comparison function (NULL for pointer comparison)
 * @param out_node Output pointer to receive found node
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinaryTree_Find(const BinaryTree *tree, const void *data,
                           int (*compare)(const void *, const void *),
                           TreeNode **out_node);

/**
 * @brief Checks if the tree contains a specific value
 * @param tree Binary tree to search
 * @param data Data to find
 * @param compare Comparison function (NULL for pointer comparison)
 * @return true if found, false otherwise
 *
 * @complexity O(n)
 */
bool BinaryTree_Contains(const BinaryTree *tree, const void *data,
                         int (*compare)(const void *, const void *));

/* ============================================================================
 * TRAVERSAL FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Performs inorder traversal (left -> current -> right)
 * @param tree Binary tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinaryTree_Inorder(const BinaryTree *tree, void **output,
                              size_t *out_len);

/**
 * @brief Performs preorder traversal (current -> left -> right)
 * @param tree Binary tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinaryTree_Preorder(const BinaryTree *tree, void **output,
                               size_t *out_len);

/**
 * @brief Performs postorder traversal (left -> right -> current)
 * @param tree Binary tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinaryTree_Postorder(const BinaryTree *tree, void **output,
                                size_t *out_len);

/**
 * @brief Performs level-order traversal (BFS)
 * @param tree Binary tree to traverse
 * @param output Array to fill with node data (must be pre-allocated)
 * @param out_len Number of elements written to output
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinaryTree_LevelOrder(const BinaryTree *tree, void **output,
                                 size_t *out_len);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the root node of the tree
 * @param tree Binary tree to examine
 * @param out_root Output pointer to receive root node
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_GetRoot(const BinaryTree *tree, TreeNode **out_root);

/**
 * @brief Returns the left child of a node
 * @param node Parent node
 * @param out_child Output pointer to receive left child
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_GetLeftChild(const TreeNode *node, TreeNode **out_child);

/**
 * @brief Returns the right child of a node
 * @param node Parent node
 * @param out_child Output pointer to receive right child
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_GetRightChild(const TreeNode *node, TreeNode **out_child);

/**
 * @brief Returns the data stored in a node
 * @param node Tree node
 * @param out_data Output pointer to receive data
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_GetData(const TreeNode *node, void **out_data);

/**
 * @brief Sets the data stored in a node
 * @param node Tree node to modify
 * @param data New data pointer
 * @return Result code
 *
 * @complexity O(1)
 */
ResultCode BinaryTree_SetData(TreeNode *node, void *data);

/**
 * @brief Checks if a node is a leaf (has no children)
 * @param node Tree node to check
 * @return true if leaf, false otherwise
 *
 * @complexity O(1)
 */
bool BinaryTree_IsLeaf(const TreeNode *node);

/**
 * @brief Creates a deep copy of the binary tree
 * @param src Source binary tree
 * @param out_dest Output pointer for destination tree
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode BinaryTree_Copy(const BinaryTree *src, BinaryTree **out_dest);

/**
 * @brief Checks if two binary trees are identical
 * @param a First binary tree
 * @param b Second binary tree
 * @return true if identical, false otherwise
 *
 * @complexity O(n)
 */
bool BinaryTree_IsIdentical(const BinaryTree *a, const BinaryTree *b);

#ifdef __cplusplus
}
#endif

#endif /* BINARY_TREE_H */
