#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Include for isdigit function

#define MAX 50

// Structures for storing Symbol Table, Intermediate Code, and Machine Code
typedef struct {
    char symbol[10];
    int address;
} Symbol;

typedef struct {
    char type[5];
    char opcode[5];
    char operand1[20];
    char operand2[20];
} Intermediate;

typedef struct {
    int address;
    char opcode[5];
    char operand1[5];
    char operand2[5];
} MachineCode;

Symbol symbolTable[MAX];
Intermediate intermediateCode[MAX];
MachineCode machineCode[MAX];
int symCount = 0, intermCount = 0, machineCount = 0;

// Function to add entries to Symbol Table
void addSymbol(char *symbol, int address) {
    strcpy(symbolTable[symCount].symbol, symbol);
    symbolTable[symCount].address = address;
    symCount++;
}

// Function to add entries to Intermediate Code
void addIntermediate(char *type, char *opcode, char *operand1, char *operand2) {
    strcpy(intermediateCode[intermCount].type, type);
    strcpy(intermediateCode[intermCount].opcode, opcode);
    strcpy(intermediateCode[intermCount].operand1, operand1);
    strcpy(intermediateCode[intermCount].operand2, operand2);
    intermCount++;
}

// Function to generate Machine Code
void generateMachineCode() {
    int lc = 100; // Location counter
    for (int i = 0; i < intermCount; i++) {
        machineCode[machineCount].address = lc++;
        strcpy(machineCode[machineCount].opcode, intermediateCode[i].opcode);

        // Handle operands
        if (isdigit(intermediateCode[i].operand1[0])) {
            strcpy(machineCode[machineCount].operand1, intermediateCode[i].operand1);
        } else {
            strcpy(machineCode[machineCount].operand1, "000");
        }

        if (isdigit(intermediateCode[i].operand2[0])) {
            strcpy(machineCode[machineCount].operand2, intermediateCode[i].operand2);
        } else {
            strcpy(machineCode[machineCount].operand2, "000");
        }
        machineCount++;
    }
}

// Function to display Symbol Table to a file
void writeSymbolTableToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Unable to open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "OPCODE | SYMBOL         | ADDRESS\n");
    for (int i = 0; i < symCount; i++) {
        fprintf(file, "%-6d | %-14s | %03d\n", i+1, symbolTable[i].symbol, symbolTable[i].address);
    }

    fclose(file);
}

// Function to display Intermediate Code to a file
void writeIntermediateCodeToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Unable to open file %s for writing\n", filename);
        return;
    }

    for (int i = 0; i < intermCount; i++) {
        fprintf(file, "(%s, %s) (%s) (%s)\n", intermediateCode[i].type, intermediateCode[i].opcode,
                intermediateCode[i].operand1, intermediateCode[i].operand2);
    }

    fclose(file);
}

// Function to display Machine Code to a file
void writeMachineCodeToFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Unable to open file %s for writing\n", filename);
        return;
    }

    for (int i = 0; i < machineCount; i++) {
        fprintf(file, "%03d %s %s %s\n", machineCode[i].address, machineCode[i].opcode,
                machineCode[i].operand1, machineCode[i].operand2);
    }

    fclose(file);
}

int main() {
    // Example Input: Reading symbol table and intermediate code
    addSymbol("A", 107);
    addSymbol("LOOP", 101);
    addSymbol("B", 504);
    addSymbol("D", 507);
    addSymbol("LABEL", 101);
    addSymbol("L1", 500);

    addIntermediate("AD", "01", "C", "100");
    addIntermediate("IS", "01", "01", "S,01");
    addIntermediate("IS", "09", "S", "03");
    addIntermediate("IS", "03", "02", "L,01");
    addIntermediate("IS", "04", "02", "S,04");
    addIntermediate("IS", "08", "03", "L,02");
    addIntermediate("AD", "05", "009", "");
    addIntermediate("AD", "05", "023", "");
    addIntermediate("DL", "01", "003(Bytes)","");

    // Generate machine code
    generateMachineCode();

    // Write outputs to separate text files
    writeSymbolTableToFile("symbol_table.txt");
    writeIntermediateCodeToFile("intermediate_code.txt");
    writeMachineCodeToFile("machine_code.txt");

    printf("Files generated successfully!\n");

    return 0;
      }