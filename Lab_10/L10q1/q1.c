#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global variable to count comparisons
long long comparison_count = 0;

// Function to swap two elements
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Generates a random pivot index between low and high (inclusive)
int random_pivot_index(int low, int high) {
    return low + rand() % (high - low + 1);
}

// Partitions the array around the pivot (Lomuto Partition Scheme)
// Returns the index of the pivot element after partitioning
int partition(int arr[], int low, int high) {
    // 1. Choose a random pivot and move it to the end
    int random_index = random_pivot_index(low, high);
    swap(&arr[random_index], &arr[high]);

    int pivot = arr[high]; // Pivot is the last element
    int i = (low - 1);      // Index of smaller element

    for (int j = low; j <= high - 1; j++) {
        // Increment comparison count for checking against the pivot
        comparison_count++;

        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++; // Increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    // Place the pivot element in its correct sorted position
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// The main function that implements Randomized Quick Sort
void randomized_quick_sort(int arr[], int low, int high) {
    if (low < high) {
        // pi is partitioning index, arr[pi] is now at right place
        int pi = partition(arr, low, high);

        // Separately sort elements before partition and after partition
        randomized_quick_sort(arr, low, pi - 1);
        randomized_quick_sort(arr, pi + 1, high);
    }
}

// Utility function to print an array
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int n;

    // Seed the random number generator
    srand(time(NULL));

    printf("--- Randomized Quicksort Implementation ---\n");
    printf("Enter the number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of elements.\n");
        return 1;
    }

    int *arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d elements: ", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input.\n");
            free(arr);
            return 1;
        }
    }

    printf("\nGiven array is: ");
    print_array(arr, n);

    // Perform the sort and measure comparisons
    randomized_quick_sort(arr, 0, n - 1);

    printf("Sorted array is: ");
    print_array(arr, n);

    printf("\nTotal number of comparisons: %lld\n", comparison_count);

    printf("\n--- Expected Time Complexity Explanation ---\n");
    printf("The expected time complexity of Randomized Quicksort is O(n log n).\n");
    printf("The randomization comes from selecting the pivot uniformly at random.\n");
    printf("This selection strategy ensures that, regardless of the input data arrangement,\n");
    printf("the probability of having a bad (worst-case) split is low. The analysis\n");
    printf("relies on the fact that the expected number of comparisons is bounded by 2n ln n, which is O(n log n).\n");
    
    free(arr);
    return 0;
}
