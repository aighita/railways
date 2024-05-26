/**
 * Ghita Andrei-Iulian 324CB
 */
#include <stdlib.h>
#include <string.h>

#include "graph-task-1.h"
#include "helpers.h"

/**
 * @brief Afișează graful.
 * 
 * @param g Pointer la graf.
 */
void print_graph(TGL *g) {
    TGL *current = g;

    // Parcurge lista de noduri
    while (current != NULL) {
        printf("[%s]:\n", current->name);
        AVertex edges = current->adj_list;

        // Parcurge lista de adiacență pentru fiecare nod
        while (edges != NULL) {
            printf("    %s - %d - [", edges->name, edges->n_sections);
            for (int i = 0; i < edges->n_sections; i++) {
                printf(" %0.2f ", edges->sections[i]);
            }
            printf("]\n");
            edges = edges->next;
        }
        current = current->next;
    }
}

TGL *alloc_vertex(char *source) {
    // Declar și aloc memorie pentru un nod
    TGL *vertex = (TGL *) malloc(sizeof(TGL));

    // Aloc memorie pentru numele de oraș al nodului
    vertex->name = (char *) malloc(sizeof(char) * 30);

    // Pointez lista de adiacență spre NULL pentru că încă nu am introdus nimic
    vertex->adj_list = NULL;

    // Pointez următorul nod din listă spre NULL pentru că este ultimul din listă
    vertex->next = NULL;

    // Copiez numele orașului sursă în numele nodului
    strcpy(vertex->name, source);

    // Returnez nodul
    return vertex;
}

void add_to_graph(TGL **g, char *source, char *destination, int nr_seg, float *seg) {
    // printf("Vom adauga: %s %s %d", source, destination, nr_seg);

    // Dacă graf-ul este NULL, aloc un nod nou pentru acesta
    if (*g == NULL) {
        // Aloc un nod pentru orașul sursă
        *g = alloc_vertex(source);

        // Adaug orașul destinație în lista de adiacență
        // Aloc memorie pentru un struct de tip edge
        (*g)->adj_list = (AVertex) malloc(sizeof(TCelVertex));
        memset((*g)->adj_list, 0, sizeof(TCelVertex));
        
        // Aloc memorie pentru nume
        (*g)->adj_list->name = (char *) malloc(sizeof(char) * 30);
        strcpy((*g)->adj_list->name, destination);
        (*g)->adj_list->n_sections = nr_seg;

        // Aloc memorie pentru tronsoane și copiez
        (*g)->adj_list->sections = (float *) malloc(sizeof(float) * (nr_seg + 1));
        memcpy((*g)->adj_list->sections, seg, nr_seg * sizeof(float));
        (*g)->adj_list->next = NULL;

        // Din moment ce graf-ul este neorientat va trebui să adaugăm un nod
        // și pentru destinație cu edge către sursă și tronsoanele inversate
        (*g)->next = alloc_vertex(destination);

        // Adaug orașul sursă în lista de adiacență a orașului destinație
        (*g)->next->adj_list = (AVertex) malloc(sizeof(TCelVertex));
        memset((*g)->next->adj_list, 0, sizeof(TCelVertex));
        (*g)->next->adj_list->name = (char *) malloc(sizeof(char) * 30);
        strcpy((*g)->next->adj_list->name, source);
        (*g)->next->adj_list->n_sections = nr_seg;
        (*g)->next->adj_list->next = NULL;

        // Aloc memorie pentru tronsoane și inversez
        (*g)->next->adj_list->sections = (float *) malloc(sizeof(float) * (nr_seg + 1));
        for (int i = nr_seg - 1; i >= 0 ; i--) {
            (*g)->next->adj_list->sections[i] = seg[nr_seg - i - 1];
        }

        (*g)->next->next = NULL;
        return;
    }

    // Dacă graf-ul nu este NULL (i.e. au fost adăugate noduri) caut în lista dacă
    // nodul ce trebuie adăugat există deja și îi adaug muchia.
    // În caz contrar adaug nodul și muchia apoi invers pentru destinație

    // Pointer către începutul listei de noduri
    TGL *current = *g;
    // Verific dacă nodul source există în listă
    while (current != NULL) {
        if (strcmp(current->name, source) == 0) break;
        current = current->next;
    }

    if (current == NULL) {
        // Dacă am ajuns la finalul listei înseamnă că nodul nu există și trebuie adăugat
        current = alloc_vertex(source);
        current->adj_list = (AVertex) malloc(sizeof(TCelVertex));
        memset(current->adj_list, 0, sizeof(TCelVertex));
        current->adj_list->name = (char *) malloc(sizeof(char) * 30);
        strcpy(current->adj_list->name, destination);
        current->adj_list->n_sections = nr_seg;
        current->adj_list->sections = (float *) malloc(sizeof(float) * (nr_seg + 1));
        memset(current->adj_list->sections, 0, sizeof(float) * (nr_seg + 1));
        memcpy(current->adj_list->sections, seg, nr_seg * sizeof(float));
        current->next = *g;
        *g = current;
    } else {
        // Parcurg lista de adiacență până la final și adaug edge-ul către destinație
        AVertex edges = current->adj_list;
        while (edges->next != NULL) edges = edges->next;
        edges->next = (AVertex) malloc(sizeof(TCelVertex));
        memset(edges->next, 0, sizeof(TCelVertex));
        edges = edges->next;
        edges->name = (char *) malloc(sizeof(char) * 30);
        strcpy(edges->name, destination);
        edges->n_sections = nr_seg;
        edges->sections = (float *) malloc(sizeof(float) * (nr_seg + 1));
        memset(edges->sections, 0, sizeof(float) * (nr_seg + 1));
        memcpy(edges->sections, seg, nr_seg * sizeof(float));
        edges->next = NULL;
    }

    // Verific dacă nodul destinație există în listă
    current = *g;
    while (current != NULL) {
        if (strcmp(current->name, destination) == 0) break;
        current = current->next;
    }

    if (current == NULL) {
        // Dacă am ajuns la finalul listei înseamnă că nodul nu există și trebuie adăugat
        current = alloc_vertex(destination);
        current->adj_list = (AVertex) malloc(sizeof(TCelVertex));
        memset(current->adj_list, 0, sizeof(TCelVertex));
        current->adj_list->name = (char *) malloc(sizeof(char) * 30);
        strcpy(current->adj_list->name, source);
        current->adj_list->n_sections = nr_seg;
        current->adj_list->sections = (float *) malloc(sizeof(float) * (nr_seg + 1));
        memset(current->adj_list->sections, 0, sizeof(float) * (nr_seg + 1));
        // Adăugăm segmentele de tronson dar inversate
        for (int i = nr_seg - 1; i >= 0; i--) {
            current->adj_list->sections[i] = seg[nr_seg - i - 1];
        }
        current->next = *g;
        *g = current;
    } else {
        // Parcurg lista de adiacență până la final și adaug edge-ul către destinație
        AVertex edges = current->adj_list;
        while (edges->next != NULL) edges = edges->next;
        edges->next = (AVertex) malloc(sizeof(TCelVertex));
        memset(edges->next, 0, sizeof(TCelVertex));
        edges = edges->next;
        edges->name = (char *) malloc(sizeof(char) * 30);
        strcpy(edges->name, source);
        edges->n_sections = nr_seg;
        edges->sections = (float *) malloc(sizeof(float) * nr_seg);
        // Adăugăm segmentele de tronson dar inversate
        for (int i = nr_seg - 1; i >= 0; i--) {
            edges->sections[i] = seg[nr_seg - i - 1];
        }
        edges->next = NULL;
    }
}

TGL *read_graph(FILE* fin, char **rute, int nr_rute) {
    // Declar variabile pentru citire din fișier
    char *source, *destination;
    int nr_seg;
    float *seg;

    // Declar graf-ul ce va fi construit și returnat
    TGL *g = NULL;

    // Aloc memorie pentru variabile
    source = (char *) malloc(sizeof(char) * 30);
    destination = (char *) malloc(sizeof(char) * 30);

    int route_index = 0;

    // Citim și construim graful cât timp fscanf != EOF
    while (fscanf(fin, "%s %s %d", source, destination, &nr_seg) != EOF) {
        // Alocăm memorie pentru citirea valorilor tronsoanelor
        seg = (float *) malloc(sizeof(float) * nr_seg);

        // Citim valorile de uzură pe tronsoane
        for (int i = 0; i < nr_seg; i++) {
            fscanf(fin, "%f", &seg[i]);
        }

        // Adăugăm ce am citit în structurile grafului
        add_to_graph(&g, source, destination, nr_seg, seg);

        // Adăugăm sursa și destinația în vectorul de rute
        if (route_index < nr_rute * 2) {
            strcpy(rute[route_index], source);
            strcpy(rute[route_index + 1], destination);
            route_index += 2;
        }

        // Eliberăm memoria pentru segmente
        free(seg);
    }

    // Eliberăm memoria variabilelor de citire din fișier
    free(source);
    free(destination);

    return g;
}

void actualizeaza_graph(TGL *g) {
    TGL *current = g;

    while (current != NULL) {
        AVertex edges = current->adj_list;
        while (edges != NULL) {
            TGL *aux_g = g;
            while (aux_g != NULL) {
                if (strcmp(aux_g->name, edges->name) == 0) break;
                aux_g = aux_g->next;
            }
            AVertex aux_edges = aux_g->adj_list;
            while (aux_edges != NULL) {
                if (strcmp(aux_edges->name, current->name) == 0) break;
                aux_edges = aux_edges->next;
            }
            for (int i = 0; i < edges->n_sections; i++) {
                edges->sections[i] = float_max(aux_edges->sections[edges->n_sections - i - 1], edges->sections[i]);
            }
            edges = edges->next;
        }
        current = current->next;
    }
}

void cerinta1(TGL *g) {
    TGL *current = g;

    // Dublez uzura pentru toate tronsoanele (dacă uzura = 0 va rămâne 0)
    while (current != NULL) {
        AVertex edges = current->adj_list;
        while (edges != NULL) {
            for (int i = 0; i < edges->n_sections; i++) {
                edges->sections[i] *= 2;
                if (edges->sections[i] > 100) edges->sections[i] = 100;
            }
            edges = edges->next;
        }
        current = current->next;
    }

    // Încep modificarea condiționată a uzurii pentru fiecare tronson
    current = g;
    while (current != NULL) {
        // Pentru fiecare nod
        AVertex edges = current->adj_list;
        while (edges != NULL) {
            // Pentru fiecare edge al nodului
            for (int i = 0; i < edges->n_sections; i++) {
                // Pentru fiecare tronson cu index != 0 și != n_sections - 1
                if (edges->sections[i] == 0) {
                    if ((i > 0) && (i < edges->n_sections - 1)) {
                        edges->sections[i] = float_max(edges->sections[i - 1], edges->sections[i + 1]) / 4;
                    }
                }
                // Pentru fiecare tronson pe index 0
                // Se calculează folosind condițiile
                if ((i == 0) && (edges->sections[i] == 0)) {
                    AVertex aux = current->adj_list;
                    float max = 0;
                    while (aux != NULL) {
                        if (aux == edges) {
                            aux = aux->next;
                            continue;
                        }
                        max = float_max(max, aux->sections[i]);
                        aux = aux->next;
                    }
                    max = float_max(max, edges->sections[i + 1]);
                    edges->sections[i] = max / 4;
                }
                // Pentru fiecare tronson pe index n_sections - 1
                // Se caută cel pe index 0 la destinație și se copiază
                if ((i == edges->n_sections - 1) && (edges->sections[i] == 0)) {
                    TGL *aux_g = g;
                    while (aux_g != NULL) {
                        if (strcmp(aux_g->name, edges->name) == 0) break;
                        aux_g = aux_g->next;
                    }
                    AVertex aux_edges = aux_g->adj_list;
                    while (aux_edges != NULL) {
                        if (strcmp(aux_edges->name, current->name) == 0) break;
                        aux_edges = aux_edges->next;
                    }
                    edges->sections[edges->n_sections - 1] = aux_edges->sections[0];
                }
            }
            edges = edges->next;
        }
        current = current->next;
    }
}

void pastreaza_rute(TGL *g, char **rute, int R, float L) {
    for (int i = 0; i < 2 * R; i = i + 2) {
        TGL *current = g;
        while (current != NULL) {
            if (strcmp(rute[i], current->name) == 0) break;
            current = current->next;
        }
        AVertex edges = current->adj_list;
        while (edges != NULL) {
            if (strcmp(rute[i + 1], edges->name) == 0) break;
            edges = edges->next;
        }
        float sum = 0;
        for (int j = 0; j < edges->n_sections; j++) {
            sum += edges->sections[j];
        }
        float sum_medie = sum / edges->n_sections;
        if (sum_medie >= L) {
            free(rute[i]);
            free(rute[i + 1]);
            rute[i] = NULL;
            rute[i + 1] = NULL;
        }
    }
}

void afiseaza_rute(TGL *g, char **rute, int R, FILE *fout) {
    for (int i = 0; i < 2 * R; i += 2) {
        TGL *current = g;
        while (current != NULL) {
            if (strcmp(rute[i], current->name) == 0) break;
            current = current->next;
        }
        AVertex edges = current->adj_list;
        while (edges != NULL) {
            if (strcmp(rute[i + 1], edges->name) == 0) break;
            edges = edges->next;
        }
        fprintf(fout, "%s %s %d", rute[i], rute[i + 1], edges->n_sections);
        for (int j = 0; j < edges->n_sections; j++) {
            fprintf(fout, " %0.2f", edges->sections[j]);
        }
        fprintf(fout, "\n");
    }
}

void distruge_graph(TGL *g) {
    TGL *current = g;
    while (current != NULL) {
        free(current->name);
        AVertex edges = current->adj_list;
        while (edges != NULL) {
            free(edges->name);
            free(edges->sections);
            AVertex aux = edges;
            edges = edges->next;
            free(aux);
        }
        TGL *aux_g = current;
        current = current->next;
        free(aux_g);
    }
}
