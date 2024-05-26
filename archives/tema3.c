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
            char *start_city = (char *) malloc(100 * sizeof(char));
            char *source = (char *) malloc(30 * sizeof(char));
            char *destination = (char *) malloc(100 * sizeof(char));
            int *lowed = (int *) calloc(100, sizeof(int));
            if (start_city == NULL || source == NULL || destination == NULL || lowed == NULL) exit(1);

            fscanf(fin, "%s", start_city);
            int k, m;
            fscanf(fin, "%d %d", &k, &m);

            TGraph graf = init_graph();
            if (graf == NULL) exit(1);

            for (int i = 0; i < m; i++) {
                int cost;
                fscanf(fin, "%s %s %d", source, destination, &cost);
                add_edge(graf, source, destination, cost, i);
            }

            int start = get_index_for_city(graf, start_city);
            int *distances = (int *) malloc(graf->cities_nb * sizeof(int));
            if (distances == NULL) exit(1);

            graf->parents = (TParent *) malloc(graf->cities_nb * sizeof(TParent *));
            if (graf->parents == NULL) exit(1);

            for (int i = 0; i < graf->cities_nb; i++) {
                distances[i] = INT_MAX;
                graf->parents[i] = (TParent) malloc(sizeof(TPCell));
                memset(graf->parents[i], 0, sizeof(TPCell));
                if (graf->parents[i] == NULL) exit(1);
            }

            cerinta_2(graf, start, distances);

            int nr = 0;
            for (int i = 0; i < graf->cities_nb; i++) {
                if (distances[i] != 0) {
                    lowed[graf->parents[i]->edge_id] = 1;
                    nr++;
                }
            }

            while (nr > k) {
                int max_dist = 0;
                int max_id = 0;
                for (int i = 0; i < graf->cities_nb; i++) {
                    if (lowed[graf->parents[i]->edge_id] && max_dist < distances[i]) {
                        max_dist = distances[i];
                        max_id = graf->parents[i]->edge_id;
                    }
                }
                lowed[max_id] = 0;
                nr--;
            }

            fprintf(fout, "%d\n", nr);
            for (int i = 0; i < m; i++) {
                if (lowed[i]) {
                    get_edge_by_id(graf, i, fout);
                }
            }

            // Eliberez memoria alocata
            for (int i = 0; i < graf->cities_nb; i++) {
                free(graf->parents[i]);
            }
            free(start_city);
            free(distances);
            free(source);
            free(destination);
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
