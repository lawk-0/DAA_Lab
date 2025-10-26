#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 100 // Maximum length for input strings

// Global table for lengths (C) and direction/predecessor (B)
int C[MAX_LEN + 1][MAX_LEN + 1];
char B[MAX_LEN + 1][MAX_LEN + 1]; // 'D' Diagonal, 'U' Up, 'L' Left

// Function to find the length and direction table for LCS
void lcs_length(const char *X, const char *Y, int m, int n) {
    // Initialize C[0][j] and C[i][0] to 0
    for (int i = 0; i <= m; i++) C[i][0] = 0;
    for (int j = 0; j <= n; j++) C[0][j] = 0;

    // Fill the C and B tables
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                C[i][j] = C[i - 1][j - 1] + 1;
                B[i][j] = 'D'; // Diagonal (match)
            } else if (C[i - 1][j] >= C[i][j - 1]) {
                C[i][j] = C[i - 1][j];
                B[i][j] = 'U'; // Up (from X[1..i-1], Y[1..j])
            } else {
                C[i][j] = C[i][j - 1];
                B[i][j] = 'L'; // Left (from X[1..i], Y[1..j-1])
            }
        }
    }
}

// Function to print the LCS string by backtracking using the B table
void print_lcs(const char *X, int i, int j) {
    if (i == 0 || j == 0) {
        return;
    }

    if (B[i][j] == 'D') {
        // Match: move diagonally and print the character
        print_lcs(X, i - 1, j - 1);
        printf("%c", X[i - 1]);
    } else if (B[i][j] == 'U') {
        // Mismatch, move up (skip character in X)
        print_lcs(X, i - 1, j);
    } else {
        // Mismatch, move left (skip character in Y)
        print_lcs(X, i, j - 1);
    }
}

int main() {
    char X[MAX_LEN], Y[MAX_LEN];
    int m, n;

    printf("--- Longest Common Subsequence (LCS) ---\n");

    printf("Enter the first string (max %d chars): ", MAX_LEN - 1);
    if (scanf("%s", X) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    m = strlen(X);

    printf("Enter the second string (max %d chars): ", MAX_LEN - 1);
    if (scanf("%s", Y) != 1) {
        printf("Invalid input.\n");
        return 1;
    }
    n = strlen(Y);
    
    if (m >= MAX_LEN || n >= MAX_LEN) {
        printf("Input string length exceeds maximum allowed. Aborting.\n");
        return 1;
    }

    // Compute the LCS length and direction tables
    lcs_length(X, Y, m, n);

    printf("\nLCS: ");
    print_lcs(X, m, n);
    printf("\n");

    printf("LCS Length: %d\n", C[m][n]);

    // Optional: Display the C table
    // printf("\nC Table (Lengths):\n");
    // for (int i = 0; i <= m; i++) {
    //     for (int j = 0; j <= n; j++) {
    //         printf("%3d", C[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}
