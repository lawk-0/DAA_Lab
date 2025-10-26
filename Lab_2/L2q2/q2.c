#include <stdio.h>
#include <stdlib.h>

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    if (!in || !out) {
        perror("File error");
        return 1;
    }

    int a, b;
    while (fscanf(in, "%d %d", &a, &b) == 2) {
        int result = gcd(a, b);
        fprintf(out, "The GCD of %d and %d is %d\n", a, b, result);
    }

    fclose(in);
    fclose(out);

    // Display output file content
    out = fopen(argv[2], "r");
    char line[100];
    while (fgets(line, sizeof(line), out)) {
        printf("%s", line);
    }
    fclose(out);
    return 0;
}
