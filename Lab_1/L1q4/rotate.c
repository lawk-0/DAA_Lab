#include <stdio.h>
#include <stdlib.h>

// Function to exchange two integers
void EXCHANGE(int *p, int *q) {
    int temp = *p;
    *p = *q;
    *q = temp;
}

// Function to rotate first p2 elements of array right by one position
void ROTATE_RIGHT(int *p1, int p2) {
    int temp = p1[p2 - 1];  // store last element among first p2
    for (int i = p2 - 1; i > 0; i--) {
        EXCHANGE(&p1[i], &p1[i - 1]);
    }
    p1[0] = temp;
}

int main() {
    FILE *inputFile, *outputFile;
    int n;

    // Open input file
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    // Read size of array
    fscanf(inputFile, "%d", &n);
    if (n <= 0) {
        printf("Invalid array size.\n");
        fclose(inputFile);
        return 1;
    }

    int *A = (int *)malloc(n * sizeof(int));
    if (A == NULL) {
        printf("Memory allocation failed.\n");
        fclose(inputFile);
        return 1;
    }

    // Read array elements
    for (int i = 0; i < n; i++) {
        fscanf(inputFile, "%d", &A[i]);
    }

    fclose(inputFile);

    // Open output file
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error: Could not open output file.\n");
        free(A);
        return 1;
    }

    // Print before rotation
    fprintf(outputFile, "Before ROTATE: ");
    for (int i = 0; i < n; i++)
        fprintf(outputFile, "%d ", A[i]);
    fprintf(outputFile, "\n");

    // Call ROTATE_RIGHT for first 5 elements
    ROTATE_RIGHT(A, 5);

    // Print after rotation
    fprintf(outputFile, "After  ROTATE: ");
    for (int i = 0; i < n; i++)
        fprintf(outputFile, "%d ", A[i]);
    fprintf(outputFile, "\n");

    fclose(outputFile);
    free(A);

    printf("Results written to output.txt successfully.\n");
    return 0;
}
