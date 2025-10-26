#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000

long long comparisons = 0;

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        comparisons++;
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
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

    printf("MAIN MENU (MERGE SORT)\n");
    printf("1. Ascending Data\n");
    printf("2. Descending Data\n");
    printf("3. Random Data\n");
    printf("4. ERROR (EXIT)\n");
    printf("Enter option: ");
    scanf("%d", &choice);

    if (choice == 1) {
        inFile = "inAsce.dat";
        outFile = "outMergeAsce.dat";
    } else if (choice == 2) {
        inFile = "inDesc.dat";
        outFile = "outMergeDesc.dat";
    } else if (choice == 3) {
        inFile = "inRand.dat";
        outFile = "outMergeRand.dat";
    } else {
        printf("Exiting...\n");
        return 0;
    }

    readFromFile(inFile, arr, &n);

    printf("Before Sorting: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    clock_t start = clock();
    mergeSort(arr, 0, n - 1);
    clock_t end = clock();

    printf("After Sorting: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    printf("Number of Comparisons: %lld\n", comparisons);
    printf("Execution Time: %lld nanoseconds\n",
           (long long)((end - start) * 1000000000LL / CLOCKS_PER_SEC));

    writeToFile(outFile, arr, n);

    return 0;
}
