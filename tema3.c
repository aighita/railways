/**
 *  Ghita Andrei-Iulian 324CB
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "helpers.h"
#include "graph-task-1.h"
#include "graph-task-2.h"

int R;
int K;
float L;

int main(int argc, char* argv[]) {
    // Resetez fisierul de output
    reset_output_file();

    // Deschid fisierele de input si output
    FILE *fin = open_file_in();
    FILE *fout = open_file_out();

    char* a = argv[argc - 1];
    int cerinta = atoi(a);

    switch(cerinta) {
        case 1: {
            // Citesc:
            // R - numarul de rute
            // K - numarul de ani
            // L - gradul de uzura acceptata
            // printf("Am citit din fisier: \n");
            fscanf(fin, "%d %d %f", &R, &K, &L);
            // printf("%d %d %0.2f\n", R, K, L);

            // Citesc graful din fisierul de input
            TGL* g;

            // Aloc memorie pentru vectorul de rute
            char **rute = (char **) malloc(sizeof(char *) * (R * 2));
            if (rute == NULL) exit(1);
            for (int i = 0; i < R * 2; i++) {
                rute[i] = (char *) malloc(30 * sizeof(char));
                if (rute[i] == NULL) exit(1);
            }

            g = read_graph(fin, rute, R);
            // printf("\n");

            // Afisez graful pentru verificare
            // if (g == NULL) printf("Graful este NULL citirea a esuat!\n");
            // else print_graph(g);

            // Rezolv cerinta 1
            // Aplic functia de uzare a tronsoanelor de K ori
            for (int i = 0; i < K; i++) {
                cerinta1(g);
                // printf("\nDUPA %d ANI\n", i + 1);
                // print_graph(g);
            }
            actualizeaza_graph(g);
            // printf("\n");
            // print_graph(g);

            afiseaza_rute(g, rute, R, fout);
            pastreaza_rute(g, rute, R, L);

            int route_id = 0;
            for (int i = 0; i < 2 * R; i += 2) {
                if (rute[i] != NULL) {
                    fprintf(fout, "%d ", route_id + 1);
                    free(rute[i]);
                    free(rute[i + 1]);
                }
                route_id += 1;
            }
            free(rute);

            distruge_graph(g);
            break;
        }
        case 2: {
            // Alocarea memoriei pentru variabile
            char *start_city = (char *) malloc(100 * sizeof(char));
            char *source = (char *) malloc(30 * sizeof(char));
            char *destination = (char *) malloc(100 * sizeof(char));
            int *lowed = (int *) calloc(100, sizeof(int));
            int *distances = NULL;
            TGraph graf = NULL;
            int k, m, cost, nr, i, j, start, max_dist, max_id;

            // Verificare alocare memorie
            if (start_city == NULL || source == NULL || destination == NULL || lowed == NULL) {
                exit(1);
            }

            // Citirea datelor de intrare
            fscanf(fin, "%s", start_city);
            fscanf(fin, "%d %d", &k, &m);

            // Inițializarea grafului
            graf = init_graph();
            if (graf == NULL) {
                exit(1);
            }

            // Citirea muchiilor și adăugarea acestora în graf
            for (i = 0; i < m; i++) {
                fscanf(fin, "%s %s %d", source, destination, &cost);
                add_edge(graf, source, destination, cost, i);
            }

            // Obținerea indexului orașului de pornire
            start = get_index_for_city(graf, start_city);

            // Alocarea memoriei pentru distanțe și părinți
            distances = (int *) malloc(graf->cities_nb * sizeof(int));
            if (distances == NULL) {
                exit(1);
            }

            graf->parents = (TParent *) malloc(graf->cities_nb * sizeof(TParent *));
            if (graf->parents == NULL) {
                exit(1);
            }

            for (i = 0; i < graf->cities_nb; i++) {
                distances[i] = INT_MAX;
                graf->parents[i] = (TParent) malloc(sizeof(TPCell));
                if (graf->parents[i] == NULL) {
                    exit(1);
                }
                memset(graf->parents[i], 0, sizeof(TPCell));
            }

            // Executarea cerinței 2
            cerinta_2(graf, start, distances);

            // Calcularea muchiilor lowed
            nr = 0;
            for (i = 0; i < graf->cities_nb; i++) {
                if (distances[i] != 0) {
                    lowed[graf->parents[i]->edge_id] = 1;
                    nr++;
                }
            }

            // Ajustarea muchiilor lowed până când nr <= k
            for (j = nr; j > k; j--) {
                max_dist = 0;
                max_id = 0;
                i = 0;
                while (i < graf->cities_nb) {
                    if (lowed[graf->parents[i]->edge_id] && max_dist < distances[i]) {
                        max_dist = distances[i];
                        max_id = graf->parents[i]->edge_id;
                    }
                    i++;
                }
                lowed[max_id] = 0;
            }
            nr = j;

            fprintf(fout, "%d\n", nr);
            i = 0;
            while (i < m) {
                if (lowed[i]) {
                    get_edge_by_id(graf, i, fout);
                }
                i++;
            }

            // Eliberarea memoriei alocate
            for (i = 0; i < graf->cities_nb; i++) {
                free(graf->parents[i]);
            }
            free(start_city);
            free(source);
            free(destination);
            free(distances);
            free(graf->parents);
            free(lowed);
            distruge_graph_2(graf);

            break;
        }
    }

    // Inchid fd pentru fisierele de citire si afisare
    fclose(fin);
    fclose(fout);
    return 0;
}
