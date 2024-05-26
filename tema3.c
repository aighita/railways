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
            int R, K, i;
            float L;
            // Citesc:
            fscanf(fin, "%d %d %f", &R, &K, &L);

            TGL* g;

            char **rute = (char **) malloc(sizeof(char *) * (R * 2));
            if (rute == NULL) exit(1);
            for (i = 0; i < R * 2; i++) {
                rute[i] = (char *) malloc(30 * sizeof(char));
                if (rute[i] == NULL) exit(1);
            }

            g = read_graph(fin, rute, R);

            for (i = 0; i < K; i++) {
                cerinta1(g);
            }
            actualizeaza_graph(g);

            afiseaza_rute(g, rute, R, fout);
            pastreaza_rute(g, rute, R, L);

            int route_id = 0;
            for (i = 0; i < 2 * R; i += 2) {
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
            int *critical_edges = (int *) calloc(100, sizeof(int));
            int *distances = NULL;
            Graph network = NULL;
            int max_edges, num_edges, price, edge_count, i, j, start, max_dist, max_id;

            if (start_city == NULL || source == NULL || destination == NULL || critical_edges == NULL) {
                exit(1);
            }

            fscanf(fin, "%s", start_city);
            fscanf(fin, "%d %d", &max_edges, &num_edges);

            network = init_network();
            if (network == NULL) {
                exit(1);
            }

            for (i = 0; i < num_edges; i++) {
                fscanf(fin, "%s %s %d", source, destination, &price);
                add_connection(network, source, destination, price, i);
            }

            start = find_node_index(network, start_city);

            distances = (int *) malloc(network->total_nodes * sizeof(int));
            if (distances == NULL) {
                exit(1);
            }

            network->ancestors = (Ancestor *) malloc(network->total_nodes * sizeof(Ancestor *));
            if (network->ancestors == NULL) {
                exit(1);
            } else {
                for (i = 0; i < network->total_nodes; i++) {
                distances[i] = INT_MAX;
                network->ancestors[i] = (Ancestor) malloc(sizeof(Ancestry));
                if (network->ancestors[i] == NULL) {
                    exit(1);
                }
                memset(network->ancestors[i], 0, sizeof(Ancestry));
            }
            }

            calculate_distances(network, start, distances);
            edge_count = 0;
            i = 0;
            while (i < network->total_nodes) {
                if (distances[i] != 0) {
                    critical_edges[network->ancestors[i]->link_id] = 1;
                    edge_count++;
                }
                i++;
            }

            for (j = edge_count; j > max_edges; j--) {
                max_dist = 0;
                max_id = 0;
                i = 0;
                while (i < network->total_nodes) {
                    if (critical_edges[network->ancestors[i]->link_id]) {
                        if (max_dist < distances[i]) {
                            max_dist = distances[i];
                            max_id = network->ancestors[i]->link_id;
                        }
                    }
                    i++;
                }
                critical_edges[max_id] = 0;
            }
            edge_count = j;

            fprintf(fout, "%d\n", edge_count);
            i = 0;
            while (i < num_edges) {
                if (critical_edges[i]) {
                    get_connection_by_id(network, i, fout);
                }
                i++;
            }

            for (i = 0; i < network->total_nodes; i++) {
                free(network->ancestors[i]);
            }
            free(start_city);
            free(source);
            free(destination);
            free(distances);
            free(network->ancestors);
            free(critical_edges);
            destroy_network(network);

            break;
        }
    }

    fclose(fin);
    fclose(fout);
    return 0;
}
