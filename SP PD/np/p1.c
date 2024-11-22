#include <stdio.h>

void createFile(const char *filename, const char *content) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error creating file: %s\n", filename);
        return;
    }
    fprintf(file, "%s\n", content);
    fclose(file);
    printf("File '%s' ", filename);
}

void createSymbolTable() {
    FILE *file = fopen("symbol_table.txt", "w");
    if (file == NULL) {
        printf("Error creating symbol_table.txt file.\n");
        return;
    }
    // Add the header   the Symbol Table
    fprintf(file, "SYMBOL TABLE\n");
    fprintf(file, " OPCODE   SYMBOL   ADDRESS\n");
    // Add some predefined entries
    fprintf(file, " LOAD     num1     100\n");
    fprintf(file, " ADD      num2     104\n");
    fprintf(file, " STORE    result   108\n");
    fprintf(file, " END      -        112\n");
    fclose(file);
    printf("File 'symbol_table.txt' ");
}

void createInputFile() {
    char input[1024]; // Buffer to store user input
    printf("Enter the s input.txt: ");
    fgets(input, sizeof(input), stdin); // Read input from the user

    FILE *file = fopen("input.txt", "w");
    if (file == NULL) {
        printf("Error creating input.txt file.\n");
        return;
    }
  

}

int main() {
    createInputFile(); 
    createSymbolTable();
    createFile("symbol_table.txt", "T");
    createFile("literal_table.txt", "");
    createFile("intermediate_code.txt", "");
    createFile("machine_code.txt", "");

    return 0;
}