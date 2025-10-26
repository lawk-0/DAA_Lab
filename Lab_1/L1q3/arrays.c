#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

int main() {
    int n, i, j, count = 0;
    int arr[MAX], freq[MAX] = {0};
    char filename[100];
    FILE *fp;

    printf("Enter the filename to read integers from: ");
    scanf("%s", filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    printf("Enter how many numbers you want to read from file: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        if (fscanf(fp, "%d", &arr[i]) != 1) {
            printf("Error reading integer from file.\n");
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);

    // Display array
    printf("The content of the array: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Count frequency
    for (i = 0; i < n; i++) {
        int isDuplicate = 0;
        for (j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                freq[i]++;
                isDuplicate = 1;
            }
        }
        if (isDuplicate) {
            count++;
        }
    }

    printf("Total number of duplicate values = %d\n", count);

    // Find most frequent element
    int maxFreq = 0, mostRepeated;
    for (i = 0; i < n; i++) {
        int tempFreq = 1;
        for (j = i + 1; j < n; j++) {
            if (arr[i] == arr[j]) {
                tempFreq++;
            }
        }
        if (tempFreq > maxFreq) {
            maxFreq = tempFreq;
            mostRepeated = arr[i];
        }
    }

    printf("The most repeating element in the array = %d\n", mostRepeated);

    return 0;
}
