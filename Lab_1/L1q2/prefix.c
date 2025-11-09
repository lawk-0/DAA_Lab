#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *inputFile, *outputFile;
    int count = 0, num;

    // Open input file
    inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    // Count number of integers in input file
    while (fscanf(inputFile, "%d", &num) == 1)
        count++;

    if (count == 0) {
        printf("Input file is empty.\n");
        fclose(inputFile);
        return 1;
    }

    // Allocate memory for array
    int *arr = (int *)malloc(count * sizeof(int));
    int *prefixSum = (int *)malloc(count * sizeof(int));

    if (arr == NULL || prefixSum == NULL) {
        printf("Memory allocation failed.\n");
        fclose(inputFile);
        return 1;
    }

    // Reset file pointer to beginning
    rewind(inputFile);

    // Read numbers into array
    for (int i = 0; i < count; i++)
        fscanf(inputFile, "%d", &arr[i]);

    fclose(inputFile);

    // Compute prefix sum
    prefixSum[0] = arr[0];
    for (int i = 1; i < count; i++)
        prefixSum[i] = prefixSum[i - 1] + arr[i];

    // Open output file
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error: Could not open output file.\n");
        free(arr);
        free(prefixSum);
        return 1;
    }

    // Write prefix sum array to output file
    fprintf(outputFile, "Output Array: ");
    for (int i = 0; i < count; i++)
        fprintf(outputFile, "%d ", prefixSum[i]);

    fclose(outputFile);

    printf("Prefix sum array written to output.txt successfully.\n");

    free(arr);
    free(prefixSum);
    return 0;
}
