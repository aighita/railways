
#include "graph-task-2.h"

TGraph init_graph() {
    TGraph g = (TGraph) malloc(sizeof(TGCell));
    if (g == NULL) return NULL;

    memset(g, 0, sizeof(TGCell));
    g->n = 0;
    g->edges = NULL;
    g->cities = NULL;
    g->cities_nb = 0;
    g->parents = NULL;
    return g;
}

PEdge create_edge(char *destination, int cost, int id) {
    PEdge newEdge = (PEdge) malloc(sizeof(TEdge));
    if (newEdge == NULL) return NULL;

    // newEdge->name = strdup(destination);
    newEdge->name = (char *) malloc(30 * sizeof(char));
    strcpy(newEdge->name, destination);
    newEdge->cost = cost;
    newEdge->next = NULL;
    newEdge->id = id;
    newEdge->is_reversed = false;

    return newEdge;
}

int get_index_for_city(TGCell *graf, char *toFind) {
    if (graf == NULL) return -1;

    for (int i = 0; i < graf->cities_nb; i++) {
        if (!strcmp(graf->cities[i], toFind)) {
            return i;
        }
    }

    return -1;
}

void add_edge(TGraph graf, char *source, char *destination, int cost, int edge_id) {
    int id = get_index_for_city(graf, source);
    PEdge edge = create_edge(destination, cost, edge_id);

    if (id == -1) {
        // Orașul sursă nu există deci il adaug
        graf->cities_nb++;
        graf->cities = realloc(graf->cities, graf->cities_nb * sizeof(char *));
        // strcpy(graf->cities[graf->cities_nb - 1], source);
        graf->cities[graf->cities_nb - 1] = strdup(source);
        id = graf->cities_nb - 1;
        graf->edges = realloc(graf->edges, graf->cities_nb * sizeof(PEdge));
        graf->edges[graf->cities_nb - 1] = edge;
    } else {
        // Orașul sursă există
        // Adaug muchia în lista de adiacență
        PEdge temp = graf->edges[id];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = edge;
    }

    id = get_index_for_city(graf, destination);
    PEdge edge1 = create_edge(source, cost, edge_id);
    edge1->is_reversed = true;

    if (id == -1) {
        // Orașul destinație nu există în graf deci il adaug
        graf->cities_nb++;
        graf->cities = realloc(graf->cities, graf->cities_nb * sizeof(char *));
        // strcpy(graf->cities[graf->cities_nb - 1], destination);
        graf->cities[graf->cities_nb - 1] = strdup(destination);
        id = graf->cities_nb - 1;
        graf->edges = realloc(graf->edges, graf->cities_nb * sizeof(PEdge));
        graf->edges[graf->cities_nb - 1] = edge1;
    } else {
        // Orașul destinație există
        // Adaug muchia inversată în lista de adiacență
        edge1->next = graf->edges[id];
        graf->edges[id] = edge1;
    }
}

void cerinta_2(TGraph graf, int start, int *distances) {
    int *visited = (int *) malloc(graf->cities_nb * sizeof(int));
    if (visited == NULL) return;

    // Inițializează vectorul de vizitare și distanțele
    for (int i = 0; i < graf->cities_nb; i++) {
        visited[i] = 0;
        distances[i] = INT_MAX;
    }
    distances[start] = 0;

    // Parcurge toate nodurile din graf
    for (int i = 0; i < graf->cities_nb - 1; i++) {
        int minDist = INT_MAX;
        int minIndex = -1;
        // Găsește nodul nevizitat cu distanța minimă
        for (int j = 0; j < graf->cities_nb; j++) {
            if (visited[j] == 0 && distances[j] < minDist) {
                minDist = distances[j];
                minIndex = j;
            }
        }

        if (minIndex != -1) {
            int id = minIndex;
            visited[id] = 1;

            // Actualizează distanțele pentru nodurile adiacente
            for (PEdge edge = graf->edges[id]; edge != NULL; edge = edge->next) {
                int j = get_index_for_city(graf, edge->name);
                if (visited[j] == 0 && distances[id] + edge->cost < distances[j]) {
                    graf->parents[j]->edge_id = edge->id;
                    graf->parents[j]->parent_vertex = id;
                    distances[j] = distances[id] + edge->cost;
                }
            }
        }
    }

    free(visited);
}

void get_edge_by_id(TGCell *graf, int id, FILE *out) {
    for (int i = 0; i < graf->cities_nb; i++) {
        PEdge edge = graf->edges[i];
        while (edge) {
            if ((edge->is_reversed == false) && (edge->id == id)) {
                fprintf(out, "%s %s\n", graf->cities[i], edge->name);
                return;
            }
            edge = edge->next;
        }
    }
}

void distruge_graph_2(TGraph graf) {
    if (graf) {
        if (graf->cities) {
            for (int i = 0; i < graf->cities_nb; i++) {
                free(graf->cities[i]);
            }
            free(graf->cities);
        }
        if (graf->edges) {
            for (int i = 0; i < graf->cities_nb; i++) {
                PEdge edge = graf->edges[i];
                while (edge) {
                    PEdge temp = edge;
                    edge = edge->next;
                    free(temp->name);
                    free(temp);
                }
            }
            free(graf->edges);
        }
        free(graf);
    }
}
