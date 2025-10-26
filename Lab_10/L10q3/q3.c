#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BITS 32

// Global array to represent the binary counter
int A[MAX_BITS] = {0};
// Index of the high-order 1 (or -1 if the counter is 0)
int high_order_1_index = -1; 
// Total cost paid for the sequence of operations
long long total_actual_cost = 0;

// Function to display the binary counter
void display_counter() {
    printf("Counter: [");
    // Display from MSB (MAX_BITS-1) down to LSB (0)
    for (int i = MAX_BITS - 1; i >= 0; i--) {
        printf("%d", A[i]);
        if (i > 0) printf(" ");
    }
    printf("]\n");
    printf("High-Order 1 Index: %d\n", high_order_1_index);
}

// Function to implement the INCREMENT operation
void increment() {
    // Start flipping from the LSB (index 0)
    int i = 0; 
    
    // Find the first '0' bit (which is where the carry stops)
    while (i < MAX_BITS && A[i] == 1) {
        A[i] = 0; // Flip 1 to 0
        total_actual_cost++;
        i++;
    }

    // Flip the first '0' to '1' (the new least significant 1)
    if (i < MAX_BITS) {
        A[i] = 1; // Flip 0 to 1
        total_actual_cost++;

        // Update the high-order 1 index if necessary
        if (i > high_order_1_index) {
            high_order_1_index = i;
        }
    } else {
        // Counter Overflow
        printf("Warning: Counter overflowed (all bits are 1 and wrapped around to 0).\n");
        high_order_1_index = -1; // Counter is effectively 0 again
    }

    printf("Operation: Increment | Actual Cost: %d\n", i + (i < MAX_BITS ? 1 : 0));
}

// Function to implement the RESET operation
void reset() {
    int bits_to_reset = 0;
    
    // Reset all bits from index 0 up to the high_order_1_index
    for (int i = 0; i <= high_order_1_index; i++) {
        if (A[i] == 1) {
            A[i] = 0; // Flip 1 to 0
            total_actual_cost++;
            bits_to_reset++;
        }
    }

    // The entire counter is now 0, so reset the pointer
    high_order_1_index = -1; 

    printf("Operation: Reset | Actual Cost: %d\n", bits_to_reset);
}

int main() {
    int choice, n_ops = 0;

    printf("--- Amortized Binary Counter (Max %d bits) ---\n", MAX_BITS);
    printf("Initial State:\n");
    display_counter();

    do {
        printf("\nMAIN MENU (Operation %d)\n", n_ops + 1);
        printf("1. INCREMENT\n");
        printf("2. RESET\n");
        printf("3. Display State\n");
        printf("4. Exit\n");
        printf("Enter option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        
        n_ops++;

        switch (choice) {
            case 1:
                increment();
                break;
            case 2:
                reset();
                break;
            case 3:
                printf("\nCurrent State:\n");
                break;
            case 4:
                printf("Exiting program.\n");
                n_ops--; // Don't count exit as an operation
                break;
            default:
                printf("Invalid option.\n");
                n_ops--;
        }

        if (choice != 4) {
            display_counter();
        }

    } while (choice != 4);

    if (n_ops > 0) {
        printf("\nTotal operations performed: %d\n", n_ops);
        printf("Total actual cost: %lld\n", total_actual_cost);
        // Calculate average amortized cost: total_cost / total_ops
        printf("Amortized Cost per Operation: %.2f\n", (double)total_actual_cost / n_ops);
        printf("Conclusion: The total time complexity for n operations is O(n), so the amortized cost per operation is O(1).\n");
    }

    return 0;
}
