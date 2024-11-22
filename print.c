#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <output_directory>\n", argv[0]);
        return 1;
    }

    // Get the output directory from the command line arguments
    char *outputDir = argv[1];

    // Create full paths for the output files
    char symbolPath[256], intermediatePath[256], machinePath[256];
    snprintf(symbolPath, sizeof(symbolPath), "%s/symbol_table.txt", outputDir);
    snprintf(intermediatePath, sizeof(intermediatePath), "%s/intermediate_code.txt", outputDir);
    snprintf(machinePath, sizeof(machinePath), "%s/machine_code.txt", outputDir);

    // Open the files
    FILE *symbolFile = fopen(symbolPath, "w");
    FILE *intermediateFile = fopen(intermediatePath, "w");
    FILE *machineFile = fopen(machinePath, "w");

    // Check for errors in opening the files
    if (!symbolFile || !intermediateFile || !machineFile) {
        perror("Error opening output files");
        return 1;
    }

    // Write the Symbol Table (Example)
    fprintf(symbolFile, "Symbol Table\n");
    fprintf(symbolFile, "---------------------------------------\n");
    fprintf(symbolFile, "| Symbol | Address |\n");
    fprintf(symbolFile, "|--------|---------|\n");
    fprintf(symbolFile, "| var1   | 1000    |\n");
    fprintf(symbolFile, "| var2   | 1004    |\n");
    fprintf(symbolFile, "| func1  | 2000    |\n");
    fprintf(symbolFile, "| func2  | 3000    |\n");

    // Write the Intermediate Code (Example)
    fprintf(intermediateFile, "Intermediate Code\n");
    fprintf(intermediateFile, "---------------------------------------\n");
    fprintf(intermediateFile, "MOV R0, #100\n");
    fprintf(intermediateFile, "ADD R1, R0, #4\n");
    fprintf(intermediateFile, "CALL func1\n");
    fprintf(intermediateFile, "MOV R2, #200\n");

    // Write the Machine Code (Example)
    fprintf(machineFile, "Machine Code\n");
    fprintf(machineFile, "---------------------------------------\n");
    fprintf(machineFile, "0001 1010 0100 0000\n");  // MOV R0, #100 (Example)
    fprintf(machineFile, "0101 1011 0100 0000\n");  // ADD R1, R0, #4 (Example)
    fprintf(machineFile, "1100 1011 0001 0000\n");  // CALL func1 (Example)
    fprintf(machineFile, "0010 1100 0100 0000\n");  // MOV R2, #200 (Example)

    // Close the files after writing
    fclose(symbolFile);
    fclose(intermediateFile);
    fclose(machineFile);

    printf("Output files generated in %s\n", outputDir);
    return 0;
}
