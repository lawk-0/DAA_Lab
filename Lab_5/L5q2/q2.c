#include <stdio.h>
#include <stdlib.h>

// Define the structure for a node in the Huffman Tree and the SYMBOL structure
typedef struct Node {
    char alphabet;          // Character itself (for leaf nodes)
    int frequency;          // Frequency of the character/subtree
    struct Node *left, *right; // Pointers to left and right children
} Node;

// Min-Priority Queue structure (implemented as a Min-Heap)
typedef struct {
    Node** array;
    int size;
    int capacity;
} MinPriorityQueue;

// --- Utility Functions for Node and Min-Heap ---

// Helper function to create a new tree node
Node* new_node(char alphabet, int frequency, Node* left, Node* right) {
    Node* temp = (Node*)malloc(sizeof(Node));
    if (temp == NULL) {
        perror("Memory allocation failed for new node");
        exit(EXIT_FAILURE);
    }
    temp->alphabet = alphabet;
    temp->frequency = frequency;
    temp->left = left;
    temp->right = right;
    return temp;
}

// Function to swap two heap nodes
void swap_node(Node** a, Node** b) {
    Node* t = *a;
    *a = *b;
    *b = t;
}

// Function to maintain the Min-Heap property
void min_heapify(MinPriorityQueue* min_heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < min_heap->size && min_heap->array[left]->frequency < min_heap->array[smallest]->frequency) {
        smallest = left;
    }

    if (right < min_heap->size && min_heap->array[right]->frequency < min_heap->array[smallest]->frequency) {
        smallest = right;
    }

    if (smallest != idx) {
        swap_node(&min_heap->array[smallest], &min_heap->array[idx]);
        min_heapify(min_heap, smallest);
    }
}

// Function to create a Min-Priority Queue
MinPriorityQueue* create_min_priority_queue(int capacity) {
    MinPriorityQueue* min_heap = (MinPriorityQueue*)malloc(sizeof(MinPriorityQueue));
    if (min_heap == NULL) {
        perror("Memory allocation failed for MinPriorityQueue");
        exit(EXIT_FAILURE);
    }
    min_heap->size = 0;
    min_heap->capacity = capacity;
    min_heap->array = (Node**)malloc(min_heap->capacity * sizeof(Node*));
    if (min_heap->array == NULL) {
        perror("Memory allocation failed for MinPriorityQueue array");
        free(min_heap);
        exit(EXIT_FAILURE);
    }
    return min_heap;
}

// Function to extract the minimum frequency node (root)
Node* extract_min(MinPriorityQueue* min_heap) {
    if (min_heap->size == 0) return NULL;

    // Store the root node
    Node* temp = min_heap->array[0];

    // Replace root with the last node
    min_heap->array[0] = min_heap->array[min_heap->size - 1];
    min_heap->size--;

    // Heapify the new root
    min_heapify(min_heap, 0);

    return temp;
}

// Function to insert a new node into the Min-Heap
void insert_min_heap(MinPriorityQueue* min_heap, Node* node) {
    if (min_heap->size == min_heap->capacity) {
        fprintf(stderr, "Error: Min-Heap capacity exceeded.\n");
        return;
    }

    int i = min_heap->size++;
    min_heap->array[i] = node;

    // Fix the Min-Heap property if it is violated
    while (i > 0 && min_heap->array[(i - 1) / 2]->frequency > min_heap->array[i]->frequency) {
        swap_node(&min_heap->array[i], &min_heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Function to build the Min-Heap from an array of nodes
void build_min_heap(MinPriorityQueue* min_heap, int n) {
    int i;
    for (i = (n - 2) / 2; i >= 0; --i) {
        min_heapify(min_heap, i);
    }
}

// Function to build the Huffman Tree
Node* build_huffman_tree(char alphabet[], int frequency[], int n) {
    Node *left, *right, *top;

    // 1. Create a min heap of capacity n
    MinPriorityQueue* min_heap = create_min_priority_queue(n);

    // 2. Initialize heap with leaf nodes
    for (int i = 0; i < n; i++) {
        min_heap->array[i] = new_node(alphabet[i], frequency[i], NULL, NULL);
    }
    min_heap->size = n;
    build_min_heap(min_heap, n);

    // 3. Repeat until size of heap becomes 1
    while (min_heap->size > 1) {
        // Extract the two minimum frequency items from min heap
        left = extract_min(min_heap);
        right = extract_min(min_heap);

        // Create a new internal node with frequency equal to the sum of the two nodes.
        // It's not a leaf node, so its alphabet is 0.
        top = new_node(0, left->frequency + right->frequency, left, right);

        // Add the new node to the min heap
        insert_min_heap(min_heap, top);
    }

    // The remaining node is the root of the Huffman tree
    Node* root = extract_min(min_heap);
    free(min_heap->array);
    free(min_heap);
    return root;
}

// Function for In-order traversal
void in_order_traversal(Node* root) {
    if (root) {
        in_order_traversal(root->left);

        // Only print alphabet for leaf nodes
        if (root->alphabet != 0) {
            printf("%c ", root->alphabet);
        }

        in_order_traversal(root->right);
    }
}

// Function to free the memory of the tree
void free_tree(Node* root) {
    if (root == NULL) return;
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}

int main() {
    int n;
    printf("--- Huffman Tree Construction ---\n");
    printf("Enter the number of distinct alphabets: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input.\n");
        return 1;
    }

    char *alphabet = (char*)malloc(n * sizeof(char));
    int *frequency = (int*)malloc(n * sizeof(int));
    if (!alphabet || !frequency) {
        perror("Memory allocation failed");
        free(alphabet); free(frequency);
        return 1;
    }

    printf("Enter the alphabets (one by one):\n");
    // Clear buffer after reading n
    while (getchar() != '\n');
    for (int i = 0; i < n; i++) {
        printf("Alphabet %d: ", i + 1);
        if (scanf(" %c", &alphabet[i]) != 1) { // Note the space before %c to skip whitespace
             printf("Invalid input.\n");
             free(alphabet); free(frequency);
             return 1;
        }
    }

    printf("Enter their frequencies:\n");
    for (int i = 0; i < n; i++) {
        printf("Frequency for '%c': ", alphabet[i]);
        if (scanf("%d", &frequency[i]) != 1 || frequency[i] < 0) {
            printf("Invalid frequency.\n");
            free(alphabet); free(frequency);
            return 1;
        }
    }

    // Build the Huffman Tree
    Node* root = build_huffman_tree(alphabet, frequency, n);

    printf("\nIn-order traversal of the tree (Huffman): ");
    in_order_traversal(root);
    printf("\n");

    // Clean up
    free_tree(root);
    free(alphabet);
    free(frequency);

    return 0;
}
