#ifndef GRAPH_H
#define GRAPH_H

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
 * @brief Edge node in adjacency list
 */
typedef struct Edge {
  int dest;           /**< Destination vertex index */
  int weight;         /**< Edge weight (1 for unweighted graphs) */
  struct Edge *next;  /**< Pointer to next edge */
} Edge;

/**
 * @brief Graph structure (adjacency list representation)
 * 
 * Supports both directed and undirected graphs.
 * Weighted and unweighted edges.
 */
typedef struct {
  int num_vertices;   /**< Number of vertices (0 to num_vertices-1) */
  int num_edges;      /**< Number of edges */
  bool directed;      /**< true = directed, false = undirected */
  Edge **adj_list;    /**< Array of adjacency lists */
} Graph;

/* ============================================================================
 * LIFECYCLE FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Creates a new graph
 * @param num_vertices Number of vertices (must be > 0)
 * @param directed true for directed graph, false for undirected
 * @param result Output pointer to receive the new Graph
 * @return Result code
 * 
 * @complexity O(V)
 */
ResultCode Graph_Create(int num_vertices, bool directed, Graph **result);

/**
 * @brief Destroys the graph and frees all associated memory
 * @param g Graph to destroy (can be NULL)
 * 
 * @complexity O(V + E)
 */
void Graph_Destroy(Graph *g);

/**
 * @brief Removes all edges from the graph (keeps vertices)
 * @param g Graph to clear
 * 
 * @complexity O(E)
 */
void Graph_Clear(Graph *g);

/* ============================================================================
 * CAPACITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Returns the number of vertices in the graph
 * @param g Graph to examine
 * @return Number of vertices (0 if g is NULL)
 * 
 * @complexity O(1)
 */
int Graph_NumVertices(const Graph *g);

/**
 * @brief Returns the number of edges in the graph
 * @param g Graph to examine
 * @return Number of edges (0 if g is NULL)
 * 
 * @complexity O(1)
 */
int Graph_NumEdges(const Graph *g);

/**
 * @brief Checks if the graph is directed
 * @param g Graph to examine
 * @return true if directed, false otherwise
 * 
 * @complexity O(1)
 */
bool Graph_IsDirected(const Graph *g);

/* ============================================================================
 * MODIFIER FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Adds an edge between src and dest
 * @param g Graph to modify
 * @param src Source vertex
 * @param dest Destination vertex
 * @param weight Edge weight (1 for unweighted graphs)
 * @return Result code
 * 
 * @complexity O(1)
 */
ResultCode Graph_AddEdge(Graph *g, int src, int dest, int weight);

/**
 * @brief Removes an edge between src and dest
 * @param g Graph to modify
 * @param src Source vertex
 * @param dest Destination vertex
 * @return Result code
 * 
 * @complexity O(degree(src))
 */
ResultCode Graph_RemoveEdge(Graph *g, int src, int dest);

/**
 * @brief Checks if an edge exists between src and dest
 * @param g Graph to examine
 * @param src Source vertex
 * @param dest Destination vertex
 * @return true if edge exists, false otherwise
 * 
 * @complexity O(degree(src))
 */
bool Graph_HasEdge(const Graph *g, int src, int dest);

/* ============================================================================
 * TRAVERSAL FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Performs Depth-First Search from a start vertex
 * @param g Graph to traverse
 * @param start Starting vertex
 * @param output_order Array to store traversal order (must be pre-allocated)
 * @param out_len Number of vertices visited
 * @return Result code
 * 
 * @complexity O(V + E)
 */
ResultCode Graph_DFS(const Graph *g, int start, int *output_order, size_t *out_len);

/**
 * @brief Performs Breadth-First Search from a start vertex
 * @param g Graph to traverse
 * @param start Starting vertex
 * @param output_order Array to store traversal order (must be pre-allocated)
 * @param out_len Number of vertices visited
 * @return Result code
 * 
 * @complexity O(V + E)
 */
ResultCode Graph_BFS(const Graph *g, int start, int *output_order, size_t *out_len);

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================
 */

/**
 * @brief Finds the shortest path in an unweighted graph using BFS
 * @param g Graph to search
 * @param start Starting vertex
 * @param dest Destination vertex
 * @param path Array to store the path (must be pre-allocated)
 * @param path_len Output pointer for path length
 * @return Result code
 * 
 * @complexity O(V + E)
 */
ResultCode Graph_ShortestPathUnweighted(const Graph *g, int start, int dest,
                                         int *path, size_t *path_len);

/**
 * @brief Checks if the graph is connected
 * @param g Graph to check
 * @return true if connected, false otherwise
 * 
 * @complexity O(V + E)
 */
bool Graph_IsConnected(const Graph *g);

/**
 * @brief Checks if the graph contains a cycle
 * @param g Graph to check
 * @return true if cycle exists, false otherwise
 * 
 * @complexity O(V + E)
 */
bool Graph_HasCycle(const Graph *g);

/**
 * @brief Returns the degree of a vertex
 * @param g Graph to examine
 * @param vertex Vertex index
 * @return Degree of the vertex, or -1 if error
 * 
 * @complexity O(1) for out-degree
 */
int Graph_GetDegree(const Graph *g, int vertex);

/**
 * @brief Prints the graph structure (debugging)
 * @param g Graph to print
 * @param print_vertex_func Function to print vertex data (optional)
 * 
 * @complexity O(V + E)
 */
void Graph_Print(const Graph *g, void (*print_vertex_func)(int));

/**
 * @brief Creates a deep copy of the graph
 * @param src Source graph
 * @param out_dest Output pointer for destination graph
 * @return Result code
 * 
 * @complexity O(V + E)
 */
ResultCode Graph_Copy(const Graph *src, Graph **out_dest);

/**
 * @brief Creates a transpose (reverse) of the graph
 * @param src Source graph
 * @param out_transpose Output pointer for transposed graph
 * @return Result code
 * 
 * @complexity O(V + E)
 */
ResultCode Graph_Transpose(const Graph *src, Graph **out_transpose);

#ifdef __cplusplus
}
#endif

#endif /* GRAPH_H */