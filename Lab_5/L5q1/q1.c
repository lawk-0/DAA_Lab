#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the structure for an item
typedef struct {
    int id;
    float profit;
    float weight;
    float ratio; // profit / weight ratio
    float amount_taken; // Amount of the item taken (0.0 to 1.0)
} ITEM;

// Function to maintain the Max-Heap property
// We build a Max-Heap based on the 'ratio' for non-increasing order sorting.
void max_heapify(ITEM arr[], int n, int i) {
    int largest = i; // Initialize largest as root
    int left = 2 * i + 1; // Left child index
    int right = 2 * i + 2; // Right child index

    // If left child is larger than root
    if (left < n && arr[left].ratio > arr[largest].ratio)
        largest = left;

    // If right child is larger than current largest
    if (right < n && arr[right].ratio > arr[largest].ratio)
        largest = right;

    // If largest is not root
    if (largest != i) {
        // Swap arr[i] and arr[largest]
        ITEM temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        // Recursively heapify the affected sub-tree
        max_heapify(arr, n, largest);
    }
}

// Main function to perform Heap Sort
void heap_sort(ITEM arr[], int n) {
    // Build a max-heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        max_heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root (largest element by ratio) to end
        ITEM temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        // Call max_heapify on the reduced heap
        max_heapify(arr, i, 0);
    }
}

// Function to solve the Fractional Knapsack Problem
void fractional_knapsack(ITEM items[], int n, float capacity) {
    float current_weight = 0.0;
    float total_profit = 0.0;

    // 1. Sort items by profit/weight ratio (non-increasing order)
    heap_sort(items, n);

    printf("\n--- Items Sorted by Profit/Weight Ratio ---\n");
    printf("Item No | Profit | Weight | Ratio\n");
    printf("--------------------------------------\n");
    for(int i=0; i<n; i++) {
        printf("%7d | %6.2f | %6.2f | %5.2f\n", items[i].id, items[i].profit, items[i].weight, items[i].ratio);
    }
    printf("--------------------------------------\n");
    printf("Knapsack Capacity: %.2f\n\n", capacity);


    // 2. Greedy selection
    for (int i = 0; i < n; i++) {
        items[i].amount_taken = 0.0; // Initialize amount taken

        if (current_weight + items[i].weight <= capacity) {
            // Take the whole item
            current_weight += items[i].weight;
            total_profit += items[i].profit;
            items[i].amount_taken = 1.0;
        } else {
            // Take a fraction of the item
            float remaining_capacity = capacity - current_weight;
            items[i].amount_taken = remaining_capacity / items[i].weight;
            current_weight += remaining_capacity;
            total_profit += items[i].profit * items[i].amount_taken;
            break; // Knapsack is full
        }
    }

    // 3. Display results
    printf("Item No | Profit | Weight | Amount to be taken\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%7d | %6.2f | %6.2f | %18.6f\n",
               items[i].id, items[i].profit, items[i].weight, items[i].amount_taken);
    }
    printf("-------------------------------------------------\n");
    printf("Maximum profit: %.6f\n", total_profit);
}

int main() {
    int n;
    float capacity;

    printf("--- Fractional Knapsack Problem ---\n");
    printf("Enter the number of items: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid number of items.\n");
        return 1;
    }

    ITEM *items = (ITEM *)malloc(n * sizeof(ITEM));
    if (items == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Input item details
    for (int i = 0; i < n; i++) {
        items[i].id = i + 1;
        printf("Enter the profit and weight of item no %d: ", items[i].id);
        if (scanf("%f %f", &items[i].profit, &items[i].weight) != 2 || items[i].weight <= 0) {
            printf("Invalid profit or weight.\n");
            free(items);
            return 1;
        }
        items[i].ratio = items[i].profit / items[i].weight;
    }

    printf("Enter the capacity of knapsack: ");
    if (scanf("%f", &capacity) != 1 || capacity < 0) {
        printf("Invalid capacity.\n");
        free(items);
        return 1;
    }

    fractional_knapsack(items, n, capacity);

    free(items);
    return 0;
}
