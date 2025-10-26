#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_BITS 30
#define MAX_FIB MAX_BITS + 5 // Max index for Fibonacci numbers

// Global array to store Fibonacci sequence (F0=0, F1=1, F2=1, F3=2, ...)
long long fib[MAX_FIB];

// Global array to store the fitstring (1: fit included, 0: not included)
// Note: We represent the string in reverse order for easier array indexing, 
// i.e., fitstring[0] is the coefficient of F2, fitstring[i] is for F_{i+2}
// Example: 101110 -> 011101 (F2, F3, F4, F5, F6)
int fitstring[MAX_BITS] = {0}; 
int current_size = 0; // Number of Fibonacci coefficients used (starting from F2)

// --- Utility Functions ---

// Pre-calculate Fibonacci numbers up to MAX_FIB
void pre_calculate_fib() {
    fib[0] = 0; // F0
    fib[1] = 1; // F1
    fib[2] = 1; // F2
    for (int i = 3; i < MAX_FIB; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
}

// Convert canonical fitstring to decimal value
long long fitstring_to_decimal() {
    long long value = 0;
    for (int i = 0; i < current_size; i++) {
        if (fitstring[i] == 1) {
            // Index i corresponds to F_{i+2}
            value += fib[i + 2]; 
        }
    }
    return value;
}

// Display the fitstring in the standard format (most significant bit first)
void display_fitstring() {
    if (current_size == 0) {
        printf("0\n");
        return;
    }
    printf("Fitstring (F_n...F_2): ");
    for (int i = current_size - 1; i >= 0; i--) {
        printf("%d", fitstring[i]);
    }
    printf(" (Decimal: %lld)\n", fitstring_to_decimal());
}

// Function to maintain canonical form (no consecutive 1s)
// Applies the rules: 11 -> 001 and 100 -> 011 (as part of increment/decrement)
void canonize() {
    // 1. Convert consecutive 1s (F_i + F_{i-1} = F_{i+1})
    // In our array (F2 at index 0): 11 -> 001
    for (int i = 0; i < current_size - 1; i++) {
        if (fitstring[i] == 1 && fitstring[i+1] == 1) {
            fitstring[i] = 0;
            fitstring[i+1] = 0;
            if (i + 2 < MAX_BITS) {
                fitstring[i + 2] += 1; // Carry to F_{i+2}
            }
        }
    }
    // Handle potential overflow/new most significant bit
    if (fitstring[current_size] == 1) {
        current_size++;
    }
    if (fitstring[current_size] == 1 && fitstring[current_size+1] == 1) {
         // Recursive call or loop again if a carry created a new '11'
         canonize(); 
         return;
    }
    
    // Ensure current_size is correct (trim trailing zeros)
    while (current_size > 0 && fitstring[current_size - 1] == 0) {
        current_size--;
    }
}


// --- Main Operations ---

// Operation: Increment fitstring by 1 (F2)
void increment() {
    // 1. Add F2 (which is 1)
    if (current_size == 0) current_size = 1;

    fitstring[0] += 1; // Add 1 to F2 coefficient

    // 2. Resolve $2 F_i$ (if any, $2 F_i = F_i + F_{i-1} + F_{i-2}$)
    // Not strictly needed in the canonical implementation.
    
    // 3. Canonize (resolve consecutive 1s: $F_{i+1} = F_i + F_{i-1}$)
    canonize();

    printf("\n--- After INCREMENT ---\n");
    display_fitstring();
}

// Operation: Decrement fitstring by 1
void decrement() {
    if (current_size == 0) {
        printf("\nCannot decrement 0.\n");
        return;
    }

    // Find the least significant '1' bit
    int i = 0;
    while (i < current_size && fitstring[i] == 0) {
        i++;
    }

    if (i >= current_size) {
        // Should only happen if the number was 0, but we checked for that.
        printf("\nError: Fitstring is non-zero but contains no '1's.\n");
        return;
    }

    // Case 1: The '1' is F2 (index 0)
    if (i == 0) {
        // We have 1 = F2. Decrementing results in 0.
        fitstring[0] = 0;
        current_size = 0; // The result is 0
    }
    // Case 2: The '1' is Fk (k > 2)
    else {
        // Convert the '1' at F_{i+2} to $F_{i+1} + F_{i}$
        // Array index i corresponds to F_{i+2}.
        // F_{i+2} -> F_{i+1} + F_{i}
        // Array indices: i -> i-1, i-2
        
        fitstring[i] = 0; // Remove F_{i+2}
        
        // Add F_{i+1} and F_{i} (indices i-1 and i-2)
        if (i - 1 >= 0) fitstring[i - 1] = 1;
        if (i - 2 >= 0) fitstring[i - 2] = 1;

        // The representation is now guaranteed to be canonical after decrement.
        // The property $100 \rightarrow 011$ (which is $F_k \rightarrow F_{k-1} + F_{k-2}$)
        // ensures that no new '11' is created because the original was canonical.
    }
    
    // Ensure current_size is correct (trim trailing zeros)
    while (current_size > 0 && fitstring[current_size - 1] == 0) {
        current_size--;
    }

    printf("\n--- After DECREMENT ---\n");
    display_fitstring();
}

// Function to set the initial fitstring based on a decimal number
void initialize_fitstring(long long num) {
    memset(fitstring, 0, sizeof(fitstring));
    current_size = 0;

    if (num <= 0) {
        printf("Initialized to 0.\n");
        return;
    }

    // Find the largest Fibonacci number less than or equal to num
    int max_idx = 0;
    for (int i = MAX_FIB - 1; i >= 2; i--) {
        if (fib[i] <= num) {
            max_idx = i;
            break;
        }
    }

    // Greedy representation (which is canonical - Zeckendorf's Theorem)
    for (int i = max_idx; i >= 2; i--) {
        if (fib[i] <= num) {
            // F_i is at index i-2 in our array
            fitstring[i - 2] = 1;
            num -= fib[i];
            
            // Update current_size
            if (i - 2 >= current_size) {
                current_size = i - 1;
            }
        }
    }
}

int main() {
    pre_calculate_fib();
    long long initial_num;
    int choice;

    printf("--- Fibonacci Fitstring Operations (Increment/Decrement) ---\n");
    printf("Enter an initial positive decimal integer (max %lld): ", fib[MAX_BITS - 1] - 1);
    if (scanf("%lld", &initial_num) != 1 || initial_num < 0) {
        printf("Invalid input.\n");
        return 1;
    }

    initialize_fitstring(initial_num);
    
    printf("\nInitial State:\n");
    display_fitstring();

    do {
        printf("\nMAIN MENU\n");
        printf("1. Increment\n");
        printf("2. Decrement\n");
        printf("3. Display Current Fitstring\n");
        printf("4. Exit\n");
        printf("Enter option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                increment();
                break;
            case 2:
                decrement();
                break;
            case 3:
                display_fitstring();
                break;
            case 4:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (choice != 4);

    return 0;
}
