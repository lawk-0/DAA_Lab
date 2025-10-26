#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_VERTICES 10
#define INF 99999 // A large value to represent infinity (since weights can be negative, INT_MAX might overflow)

// Function to find the minimum of two integers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Function to print the shortest path
void print_path(int P[][MAX_VERTICES], int i, int j) {
    if (P[i][j] == -1) {
        printf("%d", j + 1); // Destination node
        return;
    }

    // Recursively print the path from i to P[i][j]
    print_path(P, i, P[i][j]);
    printf("-->%d", j + 1); // Then print the destination node j
}

// Function to implement the Floyd-Warshall algorithm
void floyd_warshall(int D[][MAX_VERTICES], int P[][MAX_VERTICES], int V) {
    // D[i][j] is the shortest distance, P[i][j] is the predecessor
    int i, j, k;

    // Initialize P matrix
    for (i = 0; i < V; i++) {
        for (j = 0; j < V; j++) {
            if (i == j || D[i][j] == INF) {
                P[i][j] = -1; // No intermediate vertex
            } else {
                P[i][j] = i; // Predecessor of j is i
            }
        }
    }

    // Main loop for intermediate vertices k
    for (k = 0; k < V; k++) {
        // Iterate through all source vertices i
        for (i = 0; i < V; i++) {
            // Iterate through all destination vertices j
            for (j = 0; j < V; j++) {
                // If vertex k is on the shortest path from i to j, then update D[i][j]
                if (D[i][k] != INF && D[k][j] != INF && D[i][k] + D[k][j] < D[i][j]) {
                    D[i][j] = D[i][k] + D[k][j];
                    P[i][j] = P[k][j]; // Update predecessor
                }
            }
        }
    }
}

// Function to display the distance matrix
void display_matrix(int matrix[][MAX_VERTICES], int V, const char *name) {
    printf("\n--- %s Matrix ---\n", name);

    // Print column indices
    printf("    |");
    for (int j = 1; j <= V; j++) {
        printf(" %4d", j);
    }
    printf("\n----|");
    for (int j = 1; j <= V; j++) {
        printf("-----");
    }
    printf("\n");

    // Print rows
    for (int i = 0; i < V; i++) {
        printf("%3d |", i + 1);
        for (int j = 0; j < V; j++) {
            if (matrix[i][j] == INF) {
                printf("  INF");
            } else {
                printf(" %4d", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

// Function to read graph from file
int read_graph(int D[][MAX_VERTICES], int *V, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening input file inDiAdjMat2.dat");
        return 0;
    }

    *V = 0;
    int i, j;
    for (i = 0; i < MAX_VERTICES; i++) {
        long pos = ftell(file);
        int count = 0;
        for (j = 0; j < MAX_VERTICES; j++) {
            if (fscanf(file, "%d", &D[i][j]) == 1) {
                count++;
            } else {
                break;
            }
        }

        if (i > 0 && count != *V && count > 0) {
            printf("Error: Matrix is not square.\n");
            fclose(file);
            return 0;
        }

        if (count > 0) {
            *V = count;
        } else {
            break;
        }

        // Reset pointer to the beginning of the next line and populate array
        if (!feof(file)) {
            fseek(file, pos, SEEK_SET);
            for (j = 0; j < *V; j++) {
                fscanf(file, "%d", &D[i][j]);
                // Replace 0 with INF for non-existent edges, but keep 0 for (i, i)
                if (i != j && D[i][j] == 0) {
                    D[i][j] = INF;
                }
            }
        }
    }

    if (*V == 0 || i != *V) {
        printf("Error: Could not read a valid square matrix.\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int main() {
    int D[MAX_VERTICES][MAX_VERTICES]; // Distance matrix
    int P[MAX_VERTICES][MAX_VERTICES]; // Predecessor/Path matrix
    int V, src, dest;

    // Read the graph from the input file
    if (!read_graph(D, &V, "inDiAdjMat2.dat")) {
        return 1;
    }

    printf("--- Floyd-Warshall Algorithm for All-Pairs Shortest Path ---\n");
    printf("Number of Vertices (Read from file): %d\n", V);

    // Apply the Floyd-Warshall algorithm
    floyd_warshall(D, P, V);

    // Display the final shortest-path weight matrix
    display_matrix(D, V, "Shortest-Path Weight");

    printf("\n--- Path Reconstruction ---\n");
    printf("Enter the source and destination vertex (u v, 1 to %d): ", V);
    if (scanf("%d %d", &src, &dest) != 2 || src < 1 || src > V || dest < 1 || dest > V) {
        printf("Invalid source or destination vertex.\n");
        return 1;
    }

    int src_idx = src - 1;
    int dest_idx = dest - 1;

    printf("\nShortest Path from vertex %d to vertex %d: ", src, dest);
    if (D[src_idx][dest_idx] == INF) {
        printf("No Path Exists (Weight: INF)\n");
    } else {
        printf("%d", src);
        if (src_idx != dest_idx) {
            printf("-->");
            print_path(P, src_idx, dest_idx);
        }
        printf("\nPath weight: %d\n", D[src_idx][dest_idx]);
    }

    return 0;
}
