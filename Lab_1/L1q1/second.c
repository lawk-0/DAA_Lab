#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    FILE *inputFile, *outputFile;
    int count = 0, num;
    
    // Open input file
    inputFile = fopen("numbers.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    // First, count total numbers in file
    while (fscanf(inputFile, "%d", &num) == 1)
        count++;

    if (count < 2) {
        printf("Array must have at least two elements.\n");
        fclose(inputFile);
        return 1;
    }

    // Allocate memory for array
    int *arr = (int *)malloc(count * sizeof(int));
    if (arr == NULL) {
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

    int smallest = INT_MAX, second_smallest = INT_MAX;
    int largest = INT_MIN, second_largest = INT_MIN;

    for (int i = 0; i < count; i++) {
        int num = arr[i];

        // For smallest and second smallest
        if (num < smallest) {
            second_smallest = smallest;
            smallest = num;
        } else if (num < second_smallest && num != smallest) {
            second_smallest = num;
        }

        // For largest and second largest
        if (num > largest) {
            second_largest = largest;
            largest = num;
        } else if (num > second_largest && num != largest) {
            second_largest = num;
        }
    }

    // Open output file
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error: Could not open output file.\n");
        free(arr);
        return 1;
    }

    // Write results to file
    if (second_smallest == INT_MAX)
        fprintf(outputFile, "No distinct second smallest element.\n");
    else
        fprintf(outputFile, "Second Smallest Element: %d\n", second_smallest);

    if (second_largest == INT_MIN)
        fprintf(outputFile, "No distinct second largest element.\n");
    else
        fprintf(outputFile, "Second Largest Element: %d\n", second_largest);

    fclose(outputFile);
    free(arr);

    printf("Results written to output.txt successfully.\n");
    return 0;
}
