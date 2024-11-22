#include <stdio.h>
#include <string.h>

int main() {
    FILE *file;
    char instruction[100];
    int literalIndex = 1;

    // Open the input file
    file = fopen("read.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file 'read.txt'\n");
        return 1;
    }

    // Print the Literal Table header
    printf("; Literal Table\nLIT_TABLE:\n");

    // Process each line for literals
    while (fgets(instruction, sizeof(instruction), file)) {
        // Remove the trailing newline character
        instruction[strcspn(instruction, "\n")] = 0;

        // Look for literals in the instruction
        char *literal = strstr(instruction, "=");  // Find '=' in the line
        if (literal != NULL) {
            printf("    %d    %s    ; Literal value\n", literalIndex++, literal + 1);
        }
    }

    fclose(file);
    return 0;
}
