#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct person {
    int id;
    char name[50];
    int age;
    int height;
    int weight;
};

struct person *people = NULL;
int n = 0;

void swap(struct person *a, struct person *b) {
    struct person temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct person arr[], int size, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && arr[left].age < arr[smallest].age)
        smallest = left;
    if (right < size && arr[right].age < arr[smallest].age)
        smallest = right;

    if (smallest != i) {
        swap(&arr[i], &arr[smallest]);
        minHeapify(arr, size, smallest);
    }
}

void buildMinHeap(struct person arr[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--)
        minHeapify(arr, size, i);
}

void maxHeapify(struct person arr[], int size, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < size && arr[left].weight > arr[largest].weight)
        largest = left;
    if (right < size && arr[right].weight > arr[largest].weight)
        largest = right;

    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        maxHeapify(arr, size, largest);
    }
}

void buildMaxHeap(struct person arr[], int size) {
    for (int i = size / 2 - 1; i >= 0; i--)
        maxHeapify(arr, size, i);
}

void readDataFromFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    fscanf(fp, "%d", &n);
    people = (struct person *)malloc(n * sizeof(struct person));

    for (int i = 0; i < n; i++) {
        fscanf(fp, "%d %s %d %d %d",
               &people[i].id,
               people[i].name,
               &people[i].age,
               &people[i].height,
               &people[i].weight);
    }

    fclose(fp);
    printf("Data loaded successfully (%d records).\n", n);
}

void displayData(struct person arr[], int size) {
    printf("Id  Name      Age  Height  Weight\n");
    for (int i = 0; i < size; i++) {
        printf("%-3d %-10s %-3d %-6d %-6d\n",
               arr[i].id, arr[i].name,
               arr[i].age, arr[i].height,
               arr[i].weight);
    }
}

void displayYoungestWeight() {
    if (n == 0) {
        printf("No data.\n");
        return;
    }
    double weightKg = people[0].weight * 0.453592;
    printf("Weight of youngest student: %.2f kg\n", weightKg);
}

void insertMinHeap(struct person *arr, int *size, struct person newPerson) {
    (*size)++;
    arr = realloc(arr, (*size) * sizeof(struct person));
    arr[*size - 1] = newPerson;

    int i = *size - 1;
    while (i != 0 && arr[(i - 1) / 2].age > arr[i].age) {
        swap(&arr[i], &arr[(i - 1) / 2]);
        i = (i - 1) / 2;
    }

    people = arr;
}

void deleteOldest() {
    if (n == 0) {
        printf("No data.\n");
        return;
    }

    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (people[i].age > people[idx].age)
            idx = i;
    }

    printf("Deleting oldest person: %s (Age %d)\n",
           people[idx].name, people[idx].age);

    people[idx] = people[n - 1];
    n--;
    people = realloc(people, n * sizeof(struct person));
}

int main() {
    int choice;
    do {
        printf("\nMAIN MENU (HEAP)\n");
        printf("1. Read Data\n");
        printf("2. Create a Min-heap based on the age\n");
        printf("3. Create a Max-heap based on the weight\n");
        printf("4. Display weight of the youngest person\n");
        printf("5. Insert a new person into the Min-heap\n");
        printf("6. Delete the oldest person\n");
        printf("7. Exit\n");
        printf("Enter option: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char filename[50];
            printf("Enter filename: ");
            scanf("%s", filename);
            readDataFromFile(filename);
            displayData(people, n);
        } else if (choice == 2) {
            buildMinHeap(people, n);
            printf("Min-heap created (by age).\n");
            displayData(people, n);
        } else if (choice == 3) {
            buildMaxHeap(people, n);
            printf("Max-heap created (by weight).\n");
            displayData(people, n);
        } else if (choice == 4) {
            displayYoungestWeight();
        } else if (choice == 5) {
            struct person newP;
            printf("Enter id name age height weight: ");
            scanf("%d %s %d %d %d",
                  &newP.id, newP.name,
                  &newP.age, &newP.height, &newP.weight);
            insertMinHeap(people, &n, newP);
            printf("Inserted successfully.\n");
            displayData(people, n);
        } else if (choice == 6) {
            deleteOldest();
            displayData(people, n);
        }

    } while (choice != 7);

    free(people);
    return 0;
}