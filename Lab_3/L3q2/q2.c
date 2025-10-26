#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

long long comparisons = 0;
int best_case = 1, worst_case = 1;

int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    int i = low + 1, j = high;

    while (1) {
        while (i <= high && arr[i] <= pivot) { comparisons++; i++; }
        while (arr[j] > pivot) { comparisons++; j--; }
        if (i < j) {
            int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
        } else break;
    }
    int temp = arr[low]; arr[low] = arr[j]; arr[j] = temp;

    int left_size = j - low;
    int right_size = high - j;

    if (left_size == 0 || right_size == 0)
        best_case = 0; // Worst case possible
    if (left_size > high / 2 || right_size > high / 2)
        worst_case = 0; // Not worst case, more balanced

    return j;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void readFromFile(const char *filename, int arr[], int *n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    *n = 0;
    while (fscanf(fp, "%d", &arr[*n]) == 1) {
        (*n)++;
    }
    fclose(fp);
}

void writeToFile(const char *filename, int arr[], int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d ", arr[i]);
    }
    fclose(fp);
}

int main() {
    int choice, arr[MAX], n;
    char *inFile, *outFile;

    printf("MAIN MENU (QUICK SORT)\n");
    printf("1. Ascending Data\n");
    printf("2. Descending Data\n");
    printf("3. Random Data\n");
    printf("4. ERROR (EXIT)\n");
    printf("Enter option: ");
    scanf("%d", &choice);

    if (choice == 1) {
        inFile = "inAsce.dat";
        outFile = "outQuickAsce.dat";
    } else if (choice == 2) {
        inFile = "inDesc.dat";
        outFile = "outQuickDesc.dat";
    } else if (choice == 3) {
        inFile = "inRand.dat";
        outFile = "outQuickRand.dat";
    } else {
        printf("Exiting...\n");
        return 0;
    }

    readFromFile(inFile, arr, &n);

    printf("Before Sorting: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    clock_t start = clock();
    quickSort(arr, 0, n - 1);
    clock_t end = clock();

    printf("After Sorting: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("Number of Comparisons: %lld\n", comparisons);

    if (best_case && !worst_case)
        printf("Scenario: Best-case partitioning\n");
    else if (!best_case)
        printf("Scenario: Worst-case partitioning\n");
    else
        printf("Scenario: Mixed partitioning\n");

    printf("Execution Time: %lld nanoseconds\n",
           (long long)((end - start) * 1000000000LL / CLOCKS_PER_SEC));

    writeToFile(outFile, arr, n);

    return 0;
}
