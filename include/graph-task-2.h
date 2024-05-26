/**
 *  Ghita Andrei-Iulian 324CB
*/
#ifndef GRAPH_TASK_2_H
#define GRAPH_TASK_2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

// Structură care reprezintă legătura de genealogie între noduri.
typedef struct {
    int link_id;
    int parent_node;
} Ancestry, *Ancestor;

// Structură care reprezintă o conexiune în rețea.
typedef struct connection {
    bool is_reversed;
    int id;
    char *label;
    int segments;
    int price;
    struct connection *next;
} Connection, *PConnection;

// Structură care reprezintă un graf (rețea).
typedef struct network {
    Ancestor *ancestors;
    PConnection *connections;
    int num_nodes;
    int total_nodes;
    char **node_names;
} GraphCell, *Graph;

/**
 * @brief Creează și inițializează o nouă conexiune.
 */
PConnection create_connection(char *destination, int cost, int id);

/**
 * @brief Adaugă o conexiune în rețea.
 */
void add_connection(Graph graph, char *from, char *to, int cost, int connection_id);

/**
 * @brief Obține conexiunea corespunzătoare unui index și o afișează în fișierul de ieșire.
 */
void get_connection_by_id(GraphCell *graph, int id, FILE *output);

/**
 * @brief Distruge rețeaua și eliberează memoria alocată.
 */
void destroy_network(Graph graph);

/**
 * @brief Obține indexul unui oraș în rețea.
 */
int find_node_index(GraphCell *graph, char *node_name);

/**
 * @brief Inițializează o nouă rețea.
 */
Graph init_network();

/**
 * @brief Algoritmul lui Dijkstra pentru calcularea distanțelor minime de la un nod sursă.
 */
void calculate_distances(Graph graph, int start, int *distances);

#endif
