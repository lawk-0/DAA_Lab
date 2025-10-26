#include <stdio.h>
#include <stdlib.h>

// Define the structure for an edge
typedef struct {
    int u, v; // Vertices
    int weight;
} Edge;

// Define the structure for Disjoint Set Union (DSU) elements
typedef struct {
    int parent[100];
    int rank[100];
} DSU;

// Global counter for edges, assuming max 100 vertices
Edge edges[100 * 100]; 
int edge_count = 0;

// --- DSU Functions ---

// Initialize DSU structure
void dsu_initialize(DSU *dsu, int n) {
    for (int i = 1; i <= n; i++) {
        dsu->parent[i] = i; // Every element is its own parent
        dsu->rank[i] = 0;   // Rank is 0
    }
}

// Find the representative (root) of the set containing element i (with path compression)
int dsu_find(DSU *dsu, int i) {
    if (dsu->parent[i] == i) {
        return i;
    }
    // Path compression: set parent[i] to the root of the tree
    dsu->parent[i] = dsu_find(dsu, dsu->parent[i]);
    return dsu->parent[i];
}

// Union of two sets (by rank)
void dsu_union(DSU *dsu, int i, int j) {
    int root_i = dsu_find(dsu, i);
    int root_j = dsu_find(dsu, j);

    if (root_i != root_j) {
        // Union by rank: Attach smaller rank tree under root of high rank tree
        if (dsu->rank[root_i] < dsu->rank[root_j]) {
            dsu->parent[root_i] = root_j;
        } else if (dsu->rank[root_i] > dsu->rank[root_j]) {
            dsu->parent[root_j] = root_i;
        } else {
            // If ranks are the same, make one as root and increment its rank
            dsu->parent[root_j] = root_i;
            dsu->rank[root_i]++;
        }
    }
}

// --- Edge Sorting Functions (for Kruskal's) ---

// Comparison function for qsort (sort by weight in non-decreasing order)
int compare_edges(const void *a, const void *b) {
    Edge *edge_a = (Edge *)a;
    Edge *edge_b = (Edge *)b;
    return edge_a->weight - edge_b->weight;
}

// --- Kruskal's Algorithm Implementation ---

void kruskal_mst(int V, int E) {
    DSU dsu;
    dsu_initialize(&dsu, V);
    long long total_cost = 0;
    int edges_selected = 0;

    // 1. Sort all edges by weight
    qsort(edges, E, sizeof(Edge), compare_edges);

    printf("\n--- Edges Selected for Minimum Spanning Tree ---\n");
    printf("Edge | Cost\n");
    printf("-----------\n");

    // 2. Iterate through sorted edges
    for (int i = 0; i < E; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        // Check if including this edge creates a cycle (i.e., u and v are already in the same set)
        if (dsu_find(&dsu, u) != dsu_find(&dsu, v)) {
            // No cycle, add the edge to MST
            dsu_union(&dsu, u, v);
            total_cost += w;
            edges_selected++;
            printf("%d--%d | %d\n", u, v, w);

            // Stop when V-1 edges are selected
            if (edges_selected == V - 1) {
                break;
            }
        }
    }

    printf("--------------------------------------\n");
    printf("Total Weight of the Spanning Tree: %lld\n", total_cost);

    if (edges_selected < V - 1) {
        printf("Warning: The graph is disconnected. MST could not be formed.\n");
    }
}

int main() {
    int V, E;

    printf("--- Kruskal's Algorithm for MST ---\n");
    printf("Enter the number of nodes (n) and edges (m): ");
    if (scanf("%d %d", &V, &E) != 2 || V < 1 || E < 0) {
        printf("Invalid input for V or E.\n");
        return 1;
    }

    if (E == 0) {
        printf("No edges, total cost is 0.\n");
        return 0;
    }

    if (E > 100 * 100 || V > 100) {
         printf("Warning: Max allowed vertices/edges exceeded for the defined array sizes.\n");
         // Continue, but this might lead to buffer overflow if arrays are not global/large enough
    }
    
    printf("Enter the edges (u v w):\n");
    for (int i = 0; i < E; i++) {
        if (scanf("%d %d %d", &edges[i].u, &edges[i].v, &edges[i].weight) != 3 ||
            edges[i].u < 1 || edges[i].u > V || edges[i].v < 1 || edges[i].v > V || edges[i].weight < 0) {
            printf("Invalid input for edge %d.\n", i + 1);
            return 1;
        }
    }

    kruskal_mst(V, E);

    return 0;
}
