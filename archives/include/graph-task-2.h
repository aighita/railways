#ifndef GRAPH_TASK_2_H
#define GRAPH_TASK_2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

typedef struct {
    int edge_id;
    int parent_vertex;
} TPCell, *TParent;

typedef struct edge_2 {
    int id;
    char *name;
    int nr_seg;
    int cost;
    bool is_reversed;
    struct edge_2 *next;
} TEdge, *PEdge;

typedef struct {
    int n;
    int cities_nb;
    char **cities;
    TParent *parents;
    PEdge *edges;
} TGCell, *TGraph;

/**
 * @brief Obține indexul unui oraș în graf.
 */
int get_index_for_city(TGCell *graf, char *toFind);

/**
 * @brief Inițializează un graf nou.
 */
TGraph init_graph();

/**
 * @brief Algoritmul Dijkstra pentru calcularea distanțelor minime de la un nod sursă.
 */
void cerinta_2(TGraph graf, int start, int *distances);

/**
 * @brief Creează și inițializează o nouă muchie.
 */
PEdge create_edge(char *cityD, int cost, int id);

/**
 * @brief Adaugă o muchie în graf.
 */
void add_edge(TGraph graf, char *cityS, char *cityD, int cost, int edge_id);

/**
 * @brief Obține muchia corespunzătoare unui index și o afișează în fișierul de ieșire.
 */
void get_edge_by_id(TGCell *graf, int id, FILE *out);

/**
 * @brief Distruge graful și eliberează memoria alocată pentru acesta.
 */
void distruge_graph_2(TGraph graf);

#endif
