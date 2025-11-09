#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *inputFile, *outputFile;
    int n, count = 0;

    // Open input file
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    // Read how many numbers to read
    fscanf(inputFile, "%d", &n);
    if (n <= 0) {
        printf("Invalid size of array.\n");
        fclose(inputFile);
        return 1;
    }

    // Allocate memory
    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        fclose(inputFile);
        return 1;
    }

    // Read numbers into array
    for (int i = 0; i < n; i++) {
        if (fscanf(inputFile, "%d", &arr[i]) != 1) {
            printf("Error reading number %d from file.\n", i + 1);
            free(arr);
            fclose(inputFile);
            return 1;
        }
    }

    fclose(inputFile);

    // Open output file
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error: Could not open output file.\n");
        free(arr);
        return 1;
    }

    // Display array contents
    fprintf(outputFile, "The content of the array: ");
    for (int i = 0; i < n; i++) {
        fprintf(outputFile, "%d ", arr[i]);
    }
    fprintf(outputFile, "\n");

    // Count duplicates and find most repeating element
    int duplicateCount = 0, maxCount = 0, mostRepeating = arr[0];

    for (int i = 0; i < n; i++) {
        int freq = 1;
        if (arr[i] == -99999)  // Skip already counted duplicates
            continue;
        for (int j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                freq++;
                arr[j] = -99999;  // Mark as counted
            }
        }
        if (freq > 1)
            duplicateCount++;

        if (freq > maxCount) {
            maxCount = freq;
            mostRepeating = arr[i];
        }
    }

    fprintf(outputFile, "Total number of duplicate values = %d\n", duplicateCount);
    fprintf(outputFile, "The most repeating element in the array = %d\n", mostRepeating);

    fclose(outputFile);
    free(arr);

    printf("Results written to output.txt successfully.\n");
    return 0;
}
