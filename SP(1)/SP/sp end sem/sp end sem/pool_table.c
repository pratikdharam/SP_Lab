#include <stdio.h>
#include <string.h>

int main() {
    FILE *file;
    char instruction[100];
    int literalIndex = 1;   // Literal index
    int poolIndex = 1;      // Pool table index
    int literalsInPool = 0; // Count literals in the current pool

    // Open the input file
    file = fopen("read.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file 'read.txt'\n");
        return 1;
    }

    // Print the Pool Table header
    printf("; Pool Table\nPOOL_TABLE:\n");

    // Process each line for pool management
    while (fgets(instruction, sizeof(instruction), file)) {
        // Remove the trailing newline character
        instruction[strcspn(instruction, "\n")] = 0;

        // Check for literals
        if (strstr(instruction, "=")) {
            literalsInPool++;
        }

        // Check for `LTORG` or `END` directives
        if (strcmp(instruction, "LTORG") == 0 || strcmp(instruction, "END") == 0) {
            for (int i = 0; i < literalsInPool; i++) {
                printf("    %d    ; Pool entry for literal %d\n", poolIndex++, literalIndex++);
            }
            literalsInPool = 0;
        }
    }

    fclose(file);
    return 0;
}
