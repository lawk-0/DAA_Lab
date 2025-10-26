#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 10
#define INF INT_MAX

// Function to find the vertex with minimum distance value, from the set of vertices not yet included in shortest path tree
int min_distance_vertex(int dist[], int spt_set[], int V) {
    int min = INF, min_index = -1;

    for (int v = 0; v < V; v++) {
        if (spt_set[v] == 0 && dist[v] < min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// Function to print the shortest path from source to j using the parent array
void print_path(int parent[], int j) {
    // Base case: If j is the source
    if (parent[j] == -1) {
        printf("%d", j + 1);
        return;
    }
    print_path(parent, parent[j]);
    printf("->%d", j + 1);
}

// Function that implements Dijkstra's single source shortest path algorithm
void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int V, int src) {
    // Distance array. dist[i] will hold the shortest distance from src to i
    int dist[MAX_VERTICES];
    // spt_set[i] will be 1 if vertex i is included in shortest path tree
    int spt_set[MAX_VERTICES];
    // Parent array to store shortest path tree
    int parent[MAX_VERTICES];

    // Convert 1-based source to 0-based index
    int source_idx = src - 1;

    // Initialize all distances as INFINITE and spt_set[] as 0 (false)
    for (int i = 0; i < V; i++) {
        dist[i] = INF;
        spt_set[i] = 0;
        parent[i] = -1; // -1 indicates no predecessor
    }

    // Distance of source vertex from itself is always 0
    dist[source_idx] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < V; count++) {
        // Pick the minimum distance vertex from the set of vertices not yet processed.
        int u = min_distance_vertex(dist, spt_set, V);

        // If u is -1, it means no reachable, unprocessed vertex exists, so break
        if (u == -1) break;

        // Mark the picked vertex as processed
        spt_set[u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++) {
            // Update dist[v] only if:
            // 1. It is not in spt_set (not yet processed)
            // 2. There is an edge from u to v
            // 3. Total weight of path from source to v through u is smaller than current dist[v]
            if (!spt_set[v] && graph[u][v] != 0 && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Display the results
    printf("\n--- Shortest Paths from Source Vertex %d ---\n", src);
    printf("Source | Destination | Cost | Path\n");
    printf("------------------------------------------\n");

    for (int i = 0; i < V; i++) {
        printf("%6d | %11d | %4d | ", src, i + 1, dist[i]);

        if (dist[i] == INF) {
            printf("Unreachable\n");
        } else {
            printf("$");
            print_path(parent, i);
            printf("$\n");
        }
    }
}

// Function to read graph from file (similar to Lab 6)
int read_graph(int graph[MAX_VERTICES][MAX_VERTICES], int *V, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening input file inDiAdjMat1.dat");
        return 0;
    }

    // Read the V x V matrix (assuming max V is known or determined dynamically)
    // For simplicity, we'll try to read up to MAX_VERTICES x MAX_VERTICES
    *V = 0;
    int i, j;
    for (i = 0; i < MAX_VERTICES; i++) {
        long pos = ftell(file); // Save current position
        int count = 0;
        // Check if there's enough data for a new row
        for (j = 0; j < MAX_VERTICES; j++) {
            if (fscanf(file, "%d", &graph[i][j]) == 1) {
                count++;
            } else {
                break;
            }
        }

        if (i > 0 && count != *V && count > 0) {
            printf("Error: Matrix is not square. Line %d has %d elements, expected %d.\n", i + 1, count, *V);
            fclose(file);
            return 0;
        }

        if (count > 0) {
            *V = count;
        } else {
            // No more data to read for a full row
            break;
        }

        // Reset pointer to the beginning of the next line, or stay if at end
        if (!feof(file)) {
            fseek(file, pos, SEEK_SET);
            for (j = 0; j < *V; j++) {
                fscanf(file, "%d", &graph[i][j]);
                // Replace 0 with INF for non-existent edges, but keep 0 for (i, i)
                if (i != j && graph[i][j] == 0) {
                    graph[i][j] = INF;
                }
            }
        }
    }
    
    // Final check for V consistency
    if (*V == 0 || i != *V) {
        printf("Error: Could not read a valid square matrix or matrix size exceeds %d. Detected V: %d, Rows Read: %d.\n", MAX_VERTICES, *V, i);
        fclose(file);
        return 0;
    }
    
    fclose(file);
    return 1;
}

int main() {
    int graph[MAX_VERTICES][MAX_VERTICES];
    int V, src;

    // Read the graph from the input file
    if (!read_graph(graph, &V, "inDiAdjMat1.dat")) {
        return 1;
    }

    printf("--- Dijkstra's Algorithm for SSSP ---\n");
    printf("Number of Vertices (Read from file): %d\n", V);

    printf("Enter the Source Vertex (1 to %d): ", V);
    if (scanf("%d", &src) != 1 || src < 1 || src > V) {
        printf("Invalid source vertex.\n");
        return 1;
    }

    dijkstra(graph, V, src);

    return 0;
}
