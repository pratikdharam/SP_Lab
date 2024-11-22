#include <stdio.h>
#include <string.h>

int main() {
    FILE *file;
    char instruction[100];
    int codeAddress = 1000;

    // Open the input file
    file = fopen("read.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open file 'read.txt'\n");
        return 1;
    }

    // Print the Intermediate Code header
    printf("; Intermediate Code\n");

    // Process each line to generate intermediate code
    while (fgets(instruction, sizeof(instruction), file)) {
        // Remove the trailing newline character
        instruction[strcspn(instruction, "\n")] = 0;

        // Print the intermediate code
        printf("%d    %s\n", codeAddress++, instruction);
    }

    fclose(file);
    return 0;
}
