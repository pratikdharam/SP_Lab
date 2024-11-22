#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SYMBOLS 100
#define MAX_LINES 100

// Structures for symbol table
typedef struct {
    char symbol[10];
    int address;
} Symbol;

Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
int locationCounter = 0;

// Function to check if a string is an opcode or directive
int isOpcode(char *str) {
    const char *opcodes[] = {"START", "READ", "MOVER", "MOVEM", "COMP", "BC", "PRINT", "SUB", "XCHG", "END", "DC", "DS"};
    int numOpcodes = sizeof(opcodes) / sizeof(opcodes[0]);
    for (int i = 0; i < numOpcodes; i++) {
        if (strcmp(str, opcodes[i]) == 0) return 1;
    }
    return 0;
}

// Add symbol to the symbol table
void addSymbol(char *symbol, int address) {
    strcpy(symbolTable[symbolCount].symbol, symbol);
    symbolTable[symbolCount].address = address;
    symbolCount++;
}

// Function to process the source file for Pass 1
void pass1(FILE *source) {
    char line[100], label[10], opcode[10], operand[10];
    while (fgets(line, sizeof(line), source)) {
        if (sscanf(line, "%s %s %s", label, opcode, operand) >= 2) {
            if (strcmp(opcode, "START") == 0) {
                locationCounter = atoi(operand);
                continue;
            }
            if (strcmp(opcode, "END") == 0) break;
            if (label[0] != '\0') addSymbol(label, locationCounter);
            locationCounter++;
        }
    }
}

// Function to get symbol index from the symbol table
int getSymbolIndex(char *symbol) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].symbol, symbol) == 0) {
            return i;  // Return the index in the symbol table
        }
    }
    return -1;  // Symbol not found
}

// Function to generate Intermediate Code and Machine Code
void pass2(FILE *source, FILE *intermediate, FILE *machine) {
    char line[100], label[10], opcode[10], operand[10];
    while (fgets(line, sizeof(line), source)) {
        if (sscanf(line, "%s %s %s", label, opcode, operand) >= 2) {
            // Writing intermediate code in the correct format
            if (strcmp(opcode, "START") == 0) {
                fprintf(intermediate, "(AD, 01) (C, %s)\n", operand);
            } else if (strcmp(opcode, "END") == 0) {
                fprintf(intermediate, "(AD, 02)\n");
            } else if (strcmp(opcode, "MOVER") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                fprintf(intermediate, "(IS, 04) (R, 1) (S, %d)\n", symbolIndex);
            } else if (strcmp(opcode, "MOVEM") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                fprintf(intermediate, "(IS, 05) (R, 1) (S, %d)\n", symbolIndex);
            } else if (strcmp(opcode, "SUB") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                fprintf(intermediate, "(IS, 03) (R, 1) (S, %d)\n", symbolIndex);
            } else if (strcmp(opcode, "COMP") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                fprintf(intermediate, "(IS, 10) (R, 1) (S, %d)\n", symbolIndex);
            } else if (strcmp(opcode, "BC") == 0) {
                fprintf(intermediate, "(IS, 07) (C, %s)\n", operand);
            } else if (strcmp(opcode, "PRINT") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                fprintf(intermediate, "(IS, 08) (S, %d)\n", symbolIndex);
            }
            
            // Generate Machine Code (simplified)
            if (strcmp(opcode, "MOVER") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                if (symbolIndex != -1) {
                    fprintf(machine, "%d 04 R1, %d\n", locationCounter++, symbolTable[symbolIndex].address);
                }
            } else if (strcmp(opcode, "MOVEM") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                if (symbolIndex != -1) {
                    fprintf(machine, "%d 05 R1, %d\n", locationCounter++, symbolTable[symbolIndex].address);
                }
            } else if (strcmp(opcode, "SUB") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                if (symbolIndex != -1) {
                    fprintf(machine, "%d 03 R1, %d\n", locationCounter++, symbolTable[symbolIndex].address);
                }
            } else if (strcmp(opcode, "COMP") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                if (symbolIndex != -1) {
                    fprintf(machine, "%d 10 R1, %d\n", locationCounter++, symbolTable[symbolIndex].address);
                }
            } else if (strcmp(opcode, "BC") == 0) {
                fprintf(machine, "%d 07 C, %s\n", locationCounter++, operand);
            } else if (strcmp(opcode, "PRINT") == 0) {
                int symbolIndex = getSymbolIndex(operand);
                if (symbolIndex != -1) {
                    fprintf(machine, "%d 08 S, %d\n", locationCounter++, symbolTable[symbolIndex].address);
                }
            }
        }
    }
}

// Function to print the symbol table to a file
void writeSymbolTable(FILE *symbolFile) {
    fprintf(symbolFile, "Symbol\tAddress\n");
    for (int i = 0; i < symbolCount; i++) {
        fprintf(symbolFile, "%s\t%d\n", symbolTable[i].symbol, symbolTable[i].address);
    }
}

// Main function
int main() {
    FILE *source = fopen("source.txt", "r");
    FILE *intermediate = fopen("intermediate.txt", "w");
    FILE *machine = fopen("machine.txt", "w");
    FILE *symbolFile = fopen("symbol.txt", "w");

    if (!source || !intermediate || !machine || !symbolFile) {
        perror("Error opening files");
        return 1;
    }

    // Pass 1
    pass1(source);
    rewind(source); // Go back to the start of the file

    // Write symbol table to file
    writeSymbolTable(symbolFile);

    // Pass 2
    pass2(source, intermediate, machine);

    fclose(source);
    fclose(intermediate);
    fclose(machine);
    fclose(symbolFile);

    return 0;
}
