#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

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
 * @brief Disjoint Set (Union-Find) structure
 *
 * Maintains a collection of disjoint sets with union and find operations.
 * Uses union by rank and path compression for near-constant time complexity.
 */
typedef struct {
  size_t n;       /**< Number of elements (0 to n-1) */
  size_t *parent; /**< Parent array (parent[i] = i means root) */
  size_t *rank;   /**< Rank array for union by rank optimization */
} DisjointSet;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new disjoint set with n elements
 * @param n Number of elements (must be > 0)
 * @param result Output pointer to receive the new DisjointSet
 * @return Result code
 *
 * Initially, each element is in its own set.
 *
 * @complexity O(n)
 */
ResultCode DisjointSet_Create(size_t n, DisjointSet **result);

/**
 * @brief Destroys the disjoint set and frees all associated memory
 * @param ds Disjoint set to destroy (can be NULL)
 *
 * @complexity O(1)
 */
void DisjointSet_Destroy(DisjointSet *ds);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of elements in the disjoint set
 * @param ds Disjoint set to examine
 * @return Number of elements (0 if ds is NULL)
 *
 * @complexity O(1)
 */
size_t DisjointSet_Size(const DisjointSet *ds);

/**
 * @brief Returns the number of disjoint sets
 * @param ds Disjoint set to examine
 * @return Number of sets (0 if ds is NULL)
 *
 * @complexity O(n)
 */
size_t DisjointSet_NumSets(const DisjointSet *ds);

/* ============================================================================
 * OPERATIONS FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Finds the representative (root) of the set containing x
 * @param ds Disjoint set to search
 * @param x Element to find (0 <= x < n)
 * @param out_root Output pointer to receive root
 * @return Result code
 *
 * Uses path compression for optimization.
 *
 * @complexity O(α(n)) amortized (almost constant)
 */
ResultCode DisjointSet_Find(DisjointSet *ds, size_t x, size_t *out_root);

/**
 * @brief Merges the sets containing elements x and y
 * @param ds Disjoint set to modify
 * @param x First element
 * @param y Second element
 * @return Result code
 *
 * Uses union by rank for optimization.
 *
 * @complexity O(α(n)) amortized (almost constant)
 */
ResultCode DisjointSet_Union(DisjointSet *ds, size_t x, size_t y);

/**
 * @brief Checks if two elements are in the same set
 * @param ds Disjoint set to examine
 * @param x First element
 * @param y Second element
 * @return true if connected (same set), false otherwise
 *
 * @complexity O(α(n)) amortized (almost constant)
 */
bool DisjointSet_Connected(const DisjointSet *ds, size_t x, size_t y);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the size of the set containing x
 * @param ds Disjoint set to examine
 * @param x Element to query
 * @param out_size Output pointer to receive set size
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode DisjointSet_GetSetSize(const DisjointSet *ds, size_t x,
                                  size_t *out_size);

/**
 * @brief Gets all elements in the set containing x
 * @param ds Disjoint set to examine
 * @param x Element to query
 * @param elements Output array for elements (must be pre-allocated)
 * @param out_len Number of elements written
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode DisjointSet_GetSetElements(const DisjointSet *ds, size_t x,
                                      size_t *elements, size_t *out_len);

/**
 * @brief Gets all root elements (representatives)
 * @param ds Disjoint set to examine
 * @param roots Output array for roots (must be pre-allocated)
 * @param out_len Number of roots written
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode DisjointSet_GetAllRoots(const DisjointSet *ds, size_t *roots,
                                   size_t *out_len);

/**
 * @brief Resets all elements to their own sets
 * @param ds Disjoint set to reset
 *
 * @complexity O(n)
 */
void DisjointSet_Reset(DisjointSet *ds);

/**
 * @brief Prints the internal structure (debugging)
 * @param ds Disjoint set to print
 *
 * @complexity O(n)
 */
void DisjointSet_PrintStructure(const DisjointSet *ds);

/**
 * @brief Validates the disjoint set internal consistency
 * @param ds Disjoint set to validate
 * @return true if valid, false otherwise
 *
 * @complexity O(n)
 */
bool DisjointSet_Validate(const DisjointSet *ds);

/**
 * @brief Creates a deep copy of the disjoint set
 * @param src Source disjoint set
 * @param out_dest Output pointer for destination disjoint set
 * @return Result code
 *
 * @complexity O(n)
 */
ResultCode DisjointSet_Clone(const DisjointSet *src, DisjointSet **out_dest);

#ifdef __cplusplus
}
#endif

#endif /* DISJOINT_SET_H */
