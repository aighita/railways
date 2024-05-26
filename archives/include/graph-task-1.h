/* GHIȚĂ Andrei-Iulian - 324CB */
#include <stdio.h>

#ifndef _GRAPH_H_
#define _GRAPH_H_

/**
 * @brief Structura pentru a reprezenta o muchie a grafului.
*/
typedef struct edge_1 {
    char *name;
    int n_sections;
    float *sections;
    struct edge_1 *next;
}TCelVertex, *AVertex;

/**
 * @brief Structura pentru a reprezenta un nod al grafului.
*/
typedef struct vertex {
    char *name;
    AVertex adj_list;
    struct vertex *next;
} TGL;

/**
 * @brief Citeste si construieste graf-ul din fisierul de intrare "fin". Adauga rutele
 * in vectorul de rute incepand indexarea de la 0.
*/
TGL *read_graph(FILE* fin, char **rute, int nr_rute);

/**
 * @brief Adauga o noua muchie in graf.
*/
void add_to_graph(TGL **g, char *source, char *destination, int nr_seg, float *seg);

/**
 * @brief Aloca memorie pentru un nou nod.
*/
TGL *alloc_vertex(char *source);

/**
 * @brief Afiseaza graf-ul. Parcurge lista de nod-uri si pentru fiecare nod
 * afiseaza lista de muchii.
*/
void print_graph(TGL *g);

/**
 * @brief Modifica gradul de uzura pentru fiecare tronson.
*/
void cerinta1(TGL *g);

/**
 * @brief Modifica valorile de uzura a tronsoanelor.
*/
void actualizeaza_graph(TGL *g);

/**
 * @brief Pastreaza rutele care merita pastrate.
*/
void pastreaza_rute(TGL *g, char **rute, int R, float L);

/**
 * @brief Afiseaza rutele pentru cerinta 1.
*/
void afiseaza_rute(TGL *g, char **rute, int R, FILE *fout);

/**
 * @brief Distruge graful si elibereaza memoria alocata pentru acesta.
*/
void distruge_graph(TGL *g);

#endif
