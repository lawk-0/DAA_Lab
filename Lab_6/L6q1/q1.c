#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 10
#define INF INT_MAX

// Function to find the vertex with minimum key value, from the set of vertices not yet included in MST
int min_key_vertex(int key[], int mst_set[], int V) {
    int min = INF, min_index;

    for (int v = 0; v < V; v++) {
        if (mst_set[v] == 0 && key[v] < min) {
            min = key[v];
            min_index = v;
        }
    }
    return min_index;
}

// Function to implement Prim's algorithm
void prim_mst(int graph[MAX_VERTICES][MAX_VERTICES], int V, int start_vertex) {
    // Array to store constructed MST
    int parent[MAX_VERTICES];
    // Key values used to pick minimum weight edge in cut
    int key[MAX_VERTICES];
    // To represent set of vertices not yet included in MST
    int mst_set[MAX_VERTICES];
    // Cost Adjacency Matrix of the MST
    int mst_matrix[MAX_VERTICES][MAX_VERTICES] = {0};
    int total_cost = 0;

    // Initialize all keys as INFINITE and mst_set as false
    for (int i = 0; i < V; i++) {
        key[i] = INF;
        mst_set[i] = 0;
        parent[i] = -1; // No parent initially
    }

    // Key value for the starting vertex (0-indexed) is 0
    int start_node_idx = start_vertex - 1;
    key[start_node_idx] = 0;

    // The MST will have V vertices
    for (int count = 0; count < V; count++) {
        // Pick the minimum key vertex from the set of vertices not yet included in MST
        int u = min_key_vertex(key, mst_set, V);

        // Add the picked vertex to the MST set
        mst_set[u] = 1;

        // Update key value and parent index of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++) {
            // Check if (u, v) is an edge, v is not in MST, and weight(u, v) is smaller than current key[v]
            if (graph[u][v] != 0 && mst_set[v] == 0 && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }

    // Construct the MST Adjacency Matrix and calculate total cost
    for (int i = 0; i < V; i++) {
        if (parent[i] != -1) {
            int u = parent[i];
            int v = i;
            int cost = graph[u][v];
            mst_matrix[u][v] = cost;
            mst_matrix[v][u] = cost; // Undirected graph
            total_cost += cost;
        }
    }

    // Display the results
    printf("\n--- Cost Adjacency Matrix of the Minimum Spanning Tree (MST) ---\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                printf(" 0 ");
            } else if (mst_matrix[i][j] == 0) {
                printf(" - "); // Use '-' for no direct edge to distinguish from 0 cost edge (not expected in this problem)
            } else {
                printf("%2d ", mst_matrix[i][j]);
            }
        }
        printf("\n");
    }

    printf("\nTotal Weight of the Spanning Tree: %d\n", total_cost);
}

// Function to read graph from file
int read_graph(int graph[MAX_VERTICES][MAX_VERTICES], int *V, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening input file inUnAdjMat.dat");
        return 0;
    }

    // Determine the number of vertices by counting numbers in the first row
    *V = 0;
    int temp_cost;
    while (fscanf(file, "%d", &temp_cost) == 1) {
        (*V)++;
        if (fgetc(file) == '\n' || feof(file)) break;
    }
    rewind(file); // Go back to the beginning of the file

    if (*V > MAX_VERTICES || *V == 0) {
        printf("Error: Number of vertices is out of range (1-%d) or file is empty.\n", MAX_VERTICES);
        fclose(file);
        return 0;
    }

    // Read the V x V matrix
    for (int i = 0; i < *V; i++) {
        for (int j = 0; j < *V; j++) {
            if (fscanf(file, "%d", &graph[i][j]) != 1) {
                printf("Error reading matrix element at (%d, %d).\n", i+1, j+1);
                fclose(file);
                return 0;
            }
            // Replace 0 with INF for non-existent edges, but keep 0 for (i, i)
            if (i != j && graph[i][j] == 0) {
                graph[i][j] = INF;
            }
        }
    }

    fclose(file);
    return 1;
}

int main() {
    int graph[MAX_VERTICES][MAX_VERTICES];
    int V, start_vertex;

    // Read the graph from the input file
    if (!read_graph(graph, &V, "inUnAdjMat.dat")) {
        return 1;
    }

    printf("--- Prim's Algorithm for Minimum Spanning Tree ---\n");
    printf("Number of Vertices (Read from file): %d\n", V);

    printf("Enter the Starting Vertex (1 to %d): ", V);
    if (scanf("%d", &start_vertex) != 1 || start_vertex < 1 || start_vertex > V) {
        printf("Invalid starting vertex.\n");
        return 1;
    }

    prim_mst(graph, V, start_vertex);

    return 0;
}
