#include <stdio.h>
#include <stdlib.h>

void decimalToBinary(int n, char *binary, int index) {
    if (index == 0) return;
    decimalToBinary(n / 2, binary, index - 1);
    binary[16 - index] = (n % 2) + '0';
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <n> <input file> <output file>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    char *inputFile = argv[2];
    char *outputFile = argv[3];

    FILE *in = fopen(inputFile, "r");
    FILE *out = fopen(outputFile, "w");

    if (!in || !out) {
        perror("File error");
        return 1;
    }

    int num, count = 0;
    while (fscanf(in, "%d", &num) == 1 && count < n) {
        char binary[17] = {0};  // 16 bits + null terminator
        decimalToBinary(num, binary, 16);
        fprintf(out, "The binary equivalent of %d is %s\n", num, binary);
        count++;
    }

    fclose(in);
    fclose(out);

    // Display output file content
    out = fopen(outputFile, "r");
    char line[100];
    while (fgets(line, sizeof(line), out)) {
        printf("%s", line);
    }
    fclose(out);
    return 0;
}
