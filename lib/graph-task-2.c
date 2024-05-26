#include "graph-task-2.h"

#define MAXIMUM INT_MAX

Graph init_network() {
    Graph g = (Graph) malloc(sizeof(GraphCell));
    if (g == NULL) return NULL;
    else memset(g, 0, sizeof(GraphCell));

    g->num_nodes = 0;
    g->connections = NULL;
    g->node_names = NULL;
    g->ancestors = NULL;
    g->total_nodes = 0;
    return g;
}

PConnection create_connection(char *target, int value, int identifier) {
    PConnection conn = (PConnection) malloc(sizeof(Connection));
    if (conn == NULL) return NULL;

    size_t target_len = strlen(target);
    conn->label = (char *) malloc((target_len + 1) * sizeof(char));
    if (conn->label == NULL) {
        free(conn);
        return NULL;
    }

    strcpy(conn->label, target);
    conn->price = value;
    conn->next = NULL;
    conn->id = identifier;
    conn->is_reversed = false;

    return conn;
}

void add_connection(Graph graph, char *source, char *destination, int cost, int connection_id) {
    PConnection connection = create_connection(destination, cost, connection_id);
    int id = find_node_index(graph, source);

    if (id == -1) {
        graph->total_nodes++;

        char **new_node_names = malloc(graph->total_nodes * sizeof(char *));
        if (new_node_names) {
            int i = 0;
            for (i = 0; i < graph->total_nodes - 1; i++) {
                new_node_names[i] = graph->node_names[i];
            }
            new_node_names[graph->total_nodes - 1] = malloc(strlen(source) + 1);
            if (new_node_names[graph->total_nodes - 1]) {
                strcpy(new_node_names[graph->total_nodes - 1], source);
            }
            free(graph->node_names);
            graph->node_names = new_node_names;
        }

        PConnection *new_connections = malloc(graph->total_nodes * sizeof(PConnection));
        if (new_connections) {
            int i = 0;
            for (i = 0; i < graph->total_nodes - 1; i++) {
                new_connections[i] = graph->connections[i];
            }
            new_connections[graph->total_nodes - 1] = connection;
            free(graph->connections);
            graph->connections = new_connections;
        }

        id = graph->total_nodes - 1;
    } else {
        PConnection temp = graph->connections[id];
        while (temp && temp->next != NULL) {
            temp = temp->next;
        }
        if (temp) {
            temp->next = connection;
        }
    }

    id = find_node_index(graph, destination);
    PConnection reverse_connection = create_connection(source, cost, connection_id);
    reverse_connection->is_reversed = true;

    if (id != -1) {
        reverse_connection->next = graph->connections[id];
        graph->connections[id] = reverse_connection;
    } else {
        int new_size = graph->total_nodes + 1;
        char **new_node_names = (char **) malloc(new_size * sizeof(char *));
        PConnection *new_connections = (PConnection *) malloc(new_size * sizeof(PConnection));

        if (new_node_names && new_connections) {
            memcpy(new_node_names, graph->node_names, graph->total_nodes * sizeof(char *));
            memcpy(new_connections, graph->connections, graph->total_nodes * sizeof(PConnection));

            char *new_destination = (char *) malloc(strlen(destination) + 1);
            if (new_destination) {
                strcpy(new_destination, destination);
            }

            new_node_names[graph->total_nodes] = new_destination;
            new_connections[graph->total_nodes] = reverse_connection;

            free(graph->node_names);
            free(graph->connections);

            graph->node_names = new_node_names;
            graph->connections = new_connections;
            graph->total_nodes = new_size;

            id = new_size - 1;
        } else {
            free(new_node_names);
            free(new_connections);
        }
    }
}

int find_node_index(GraphCell *graph, char *toFind) {
    if (graph == NULL) exit(-1);

    int i = 0;
    while (i < graph->total_nodes) {
        if (!strcmp(graph->node_names[i], toFind)) {
            return i;
        }
        i++;
    }

    return -1;
}

void get_connection_by_id(GraphCell *graph, int id, FILE *output) {
    int nodeIndex = 0;
    bool found = false;

    while (nodeIndex < graph->total_nodes && !found) {
        PConnection link;
        for (link = graph->connections[nodeIndex]; link != NULL; link = link->next) {
            if (link->id == id && !link->is_reversed) {
                fprintf(output, "%s %s\n", graph->node_names[nodeIndex], link->label);
                found = true;
                break;
            }
        }
        nodeIndex++;
    }
}

void destroy_network(Graph graph) {
    if (graph) {
        if (graph->node_names) {
            int i = 0;
            for (i = 0; i < graph->total_nodes; i++) {
                free(graph->node_names[i]);
            }
            free(graph->node_names);
        }
        if (graph->connections) {
            int i = 0;
            for (i = 0; i < graph->total_nodes; i++) {
                PConnection connection = graph->connections[i];
                while (connection) {
                    PConnection temp = connection;
                    connection = connection->next;
                    free(temp->label);
                    free(temp);
                }
            }
            free(graph->connections);
        }
        free(graph);
    }
}

void calculate_distances(Graph graph, int start, int *distances) {
    int *processed = (int *) calloc(graph->total_nodes, sizeof(int));
    if (!processed) return;

    int node = 0;
    for (node = 0; node < graph->total_nodes; node++) {
        distances[node] = MAXIMUM;
    }
    distances[start] = 0;

    while (1) {
        int smallestDistance = MAXIMUM;
        int currentNode = -1;

        int i = 0;
        for (i = 0; i < graph->total_nodes; i++) {
            if (!processed[i] && distances[i] < smallestDistance) {
                smallestDistance = distances[i];
                currentNode = i;
            }
        }

        if (currentNode == -1) break;

        processed[currentNode] = 1;
        PConnection edge = graph->connections[currentNode];

        while (edge != NULL) {
            int neighbour = find_node_index(graph, edge->label);
            if (!processed[neighbour]) {
                int newDist = distances[currentNode] + edge->price;
                if (newDist < distances[neighbour]) {
                    distances[neighbour] = newDist;
                    graph->ancestors[neighbour]->link_id = edge->id;
                    graph->ancestors[neighbour]->parent_node = currentNode;
                }
            }
            edge = edge->next;
        }
    }

    free(processed);
}
