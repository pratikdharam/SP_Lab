#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 100
#define MAX_CODE 200

// Data Structures
typedef struct {
    char symbol[20];
    int address;
} Symbol;

typedef struct {
    int lc;
    char mnemonic[10];
    char operand1[20];
    char operand2[20];
} IntermediateCode;

// Global Variables
Symbol symbolTable[MAX_SYMBOLS];
IntermediateCode intermediateCode[MAX_CODE];
int symbolCount = 0, icCount = 0;

// Function Prototypes
int searchSymbol(const char *symbol);
void generateMachineCode();
void displaySymbolTable();
void displayIntermediateCode();
void error(const char *message);

// Main Function
int main() {
    // Initialize symbol table (example)
    symbolCount = 5;
    strcpy(symbolTable[0].symbol, "ALPHA");
    symbolTable[0].address = 200;
    strcpy(symbolTable[1].symbol, "BETA");
    symbolTable[1].address = 204;
    strcpy(symbolTable[2].symbol, "ONE");
    symbolTable[2].address = 208;
    strcpy(symbolTable[3].symbol, "TWO");
    symbolTable[3].address = 212;
    strcpy(symbolTable[4].symbol, "THREE");
    symbolTable[4].address = 216;

    // Initialize intermediate code (example)
    icCount = 5;
    intermediateCode[0].lc = 100;
    strcpy(intermediateCode[0].mnemonic, "MOVER");
    strcpy(intermediateCode[0].operand1, "AREG");
    strcpy(intermediateCode[0].operand2, "ALPHA");

    intermediateCode[1].lc = 101;
    strcpy(intermediateCode[1].mnemonic, "MOVEM");
    strcpy(intermediateCode[1].operand1, "AREG");
    strcpy(intermediateCode[1].operand2, "BETA");

    intermediateCode[2].lc = 102;
    strcpy(intermediateCode[2].mnemonic, "ADD");
    strcpy(intermediateCode[2].operand1, "AREG");
    strcpy(intermediateCode[2].operand2, "ONE");

    intermediateCode[3].lc = 103;
    strcpy(intermediateCode[3].mnemonic, "SUB");
    strcpy(intermediateCode[3].operand1, "BREG");
    strcpy(intermediateCode[3].operand2, "TWO");

    intermediateCode[4].lc = 104;
    strcpy(intermediateCode[4].mnemonic, "MULT");
    strcpy(intermediateCode[4].operand1, "CREG");
    strcpy(intermediateCode[4].operand2, "THREE");

    // Display Symbol Table and Intermediate Code
    displaySymbolTable();
    displayIntermediateCode();

    // Generate Machine Code
    generateMachineCode();

    return 0;
}

// Function to Search for Symbol in Symbol Table
int searchSymbol(const char *symbol) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].symbol, symbol) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to Generate Machine Code
void generateMachineCode() {
    printf("\nMachine Code:\n");
    printf("LC      Mnemonic   Operand1   Operand2\n");

    for (int i = 0; i < icCount; i++) {
        int symIndex = searchSymbol(intermediateCode[i].operand2);

        printf("%-8d %-10s %-8s ", 
            intermediateCode[i].lc, 
            intermediateCode[i].mnemonic, 
            intermediateCode[i].operand1);

        if (symIndex != -1) {
            printf("%d\n", symbolTable[symIndex].address);
        } else {
            printf("Error: Undefined symbol '%s'\n", intermediateCode[i].operand2);
        }
    }
}

// Function to Display Symbol Table
void displaySymbolTable() {
    printf("\nSymbol Table:\n");
    printf("Symbol     Address\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%-10s %d\n", symbolTable[i].symbol, symbolTable[i].address);
    }
}

// Function to Display Intermediate Code
void displayIntermediateCode() {
    printf("\nIntermediate Code:\n");
    printf("LC      Mnemonic   Operand1   Operand2\n");
    for (int i = 0; i < icCount; i++) {
        printf("%-8d %-10s %-8s %-8s\n", 
            intermediateCode[i].lc, 
            intermediateCode[i].mnemonic, 
            intermediateCode[i].operand1, 
            intermediateCode[i].operand2);
    }
}

// Function to Handle Errors
void error(const char *message) {
    printf("Error: %s\n", message);
}
