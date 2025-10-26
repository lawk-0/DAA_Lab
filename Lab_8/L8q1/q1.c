#include <stdio.h>
#include <limits.h>

#define MAX_MATRICES 10

// Function to find the optimal parenthesization
void print_optimal_parens(int s[][MAX_MATRICES], int i, int j) {
    if (i == j) {
        printf("A%d", i);
    } else {
        printf("(");
        print_optimal_parens(s, i, s[i][j]);
        printf(" ");
        print_optimal_parens(s, s[i][j] + 1, j);
        printf(")");
    }
}

// Function to implement Matrix Chain Order (MCO)
int matrix_chain_order(int p[], int n, int m[][MAX_MATRICES], int s[][MAX_MATRICES]) {
    int num_matrices = n - 1; // Number of matrices A1...An is derived from dimensions p[0...n]

    // Initialize m[i][i] = 0 for single matrix
    for (int i = 1; i <= num_matrices; i++) {
        m[i][i] = 0;
    }

    // l is the chain length (l=2 for A_i * A_{i+1}, l=3 for A_i * A_{i+1} * A_{i+2}, etc.)
    for (int l = 2; l <= num_matrices; l++) {
        for (int i = 1; i <= num_matrices - l + 1; i++) {
            int j = i + l - 1;
            m[i][j] = INT_MAX; // Initialize to a large value

            // Try all possible split points k
            for (int k = i; k <= j - 1; k++) {
                // q = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j]
                // p[i-1] is rows of A_i, p[k] is cols of A_k/rows of A_{k+1}, p[j] is cols of A_j
                int q = m[i][k] + m[k+1][j] + p[i-1] * p[k] * p[j];

                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k; // Store the optimal split point
                }
            }
        }
    }

    return m[1][num_matrices];
}

// Function to display the tables
void display_tables(int table[][MAX_MATRICES], int n, const char *name) {
    int num_matrices = n - 1;
    printf("\n--- %s Table (Size %dx%d) ---\n", name, num_matrices, num_matrices);

    // Print column indices
    printf("    |");
    for (int j = 1; j <= num_matrices; j++) {
        printf(" %3d", j);
    }
    printf("\n----|");
    for (int j = 1; j <= num_matrices; j++) {
        printf("----");
    }
    printf("\n");

    // Print rows
    for (int i = 1; i <= num_matrices; i++) {
        printf("%3d |", i);
        for (int j = 1; j <= num_matrices; j++) {
            if (i <= j) {
                printf(" %3d", table[i][j]);
            } else {
                printf("   0"); // Lower triangular part is 0/unused
            }
        }
        printf("\n");
    }
}

int main() {
    int n, p[MAX_MATRICES + 1]; // p array has size n+1 for n matrices
    int m[MAX_MATRICES][MAX_MATRICES] = {0}; // M-table (min scalar multiplications)
    int s[MAX_MATRICES][MAX_MATRICES] = {0}; // S-table (optimal split point)

    printf("--- Matrix Chain Multiplication Problem ---\n");

    printf("Enter number of matrices (max %d): ", MAX_MATRICES - 1);
    if (scanf("%d", &n) != 1 || n < 1 || n >= MAX_MATRICES) {
        printf("Invalid number of matrices.\n");
        return 1;
    }

    // Input dimensions p[i-1] x p[i] for matrix Ai
    for (int i = 1; i <= n; i++) {
        int rows, cols;
        printf("Enter row and col size of A%d: ", i);
        if (scanf("%d %d", &rows, &cols) != 2 || rows <= 0 || cols <= 0) {
            printf("Invalid dimension.\n");
            return 1;
        }

        if (i == 1) {
            p[i - 1] = rows;
            p[i] = cols;
        } else {
            // Check dimension compatibility: previous matrix's columns must match current matrix's rows
            if (p[i - 1] != rows) {
                printf("Dimension mismatch! Columns of A%d (%d) must match rows of A%d (%d).\n",
                       i - 1, p[i - 1], i, rows);
                return 1;
            }
            p[i] = cols;
        }
    }

    int min_mults = matrix_chain_order(p, n + 1, m, s); // n+1 is the size of the p array

    // Display results
    display_tables(m, n + 1, "M");
    display_tables(s, n + 1, "S");

    printf("\nOptimal parenthesization: ");
    print_optimal_parens(s, 1, n);
    printf("\n");

    printf("The optimal ordering of the given matrices requires %d scalar multiplications.\n", min_mults);

    return 0;
}
