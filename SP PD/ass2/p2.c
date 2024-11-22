#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

FILE *symbolTable, *declarativeTable, *registerTable, *literalTable, *directiveTable, *imperativeTable;

#define MAX_WORDS 100

// Structures to hold the processed words
char processedSymbols[MAX_WORDS][50];
char processedDirectives[MAX_WORDS][50];
char processedRegisters[MAX_WORDS][50];
char processedLiterals[MAX_WORDS][50];
char processedImperatives[MAX_WORDS][50];
char processedDeclaratives[MAX_WORDS][50];

int imperativeCounter = 1;
int registerCounter = 1;
int directiveCounter = 1;
int declarativeCounter = 1;
int literalCounter = 1;
int symbolCounter = 1;

int symbolCount = 0, directiveCount = 0, registerCount = 0, literalCount = 0, imperativeCount = 0, declarativeCount = 0;

int isRegister(char *word) {
    return (strcmp(word, "AREG") == 0 || strcmp(word, "BREG") == 0 || strcmp(word, "CREG") == 0 || strcmp(word, "DREG") == 0);
}

int isDirective(char *word) {
    return (strcmp(word, "START") == 0 || strcmp(word, "END") == 0 || strcmp(word, "LTORG") == 0 || strcmp(word, "ORIGIN") == 0 || strcmp(word, "EQU") == 0);
}

int isImperative(char *word) {
    return (strcmp(word, "MOVEM") == 0 || strcmp(word, "MOVER") == 0 || strcmp(word, "ADD") == 0 || strcmp(word, "SUB") == 0 ||
            strcmp(word, "MULT") == 0 || strcmp(word, "COMP") == 0 || strcmp(word, "PRINT") == 0);
}

int isDeclarative(char *word) {
    return (strcmp(word, "DS") == 0 || strcmp(word, "DC") == 0);
}

int isLiteral(char *word) {
    return word[0] == '=';
}

int isSymbol(char *word) {
    // A symbol starts with a letter, isn't a keyword (register, directive, imperative, declarative, literal), and isn't a number
    return isalpha(word[0]) && !isRegister(word) && !isDirective(word) && !isImperative(word) && 
           !isDeclarative(word) && !isLiteral(word);
}

int isProcessed(char word[50], char processed[MAX_WORDS][50], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word, processed[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void processWord(char *word) {

      // Remove unwanted characters (like quotes)
    if (word[0] == '\'' || word[0] == '"') {
        memmove(word, word+1, strlen(word));
    }
    if (word[strlen(word)-1] == '\'' || word[strlen(word)-1] == '"') {
        word[strlen(word)-1] = '\0';
    }
    // processing words
    if (isRegister(word) && !isProcessed(word, processedRegisters, registerCount)) {
        strcpy(processedRegisters[registerCount++], word);
        fprintf(registerTable, "%s - %d\n", word, registerCounter++);
    } else if (isDirective(word) && !isProcessed(word, processedDirectives, directiveCount)) {
        strcpy(processedDirectives[directiveCount++], word);
        fprintf(directiveTable, "%s - %d\n", word, directiveCounter++);
    } else if (isImperative(word) && !isProcessed(word, processedImperatives, imperativeCount)) {
        strcpy(processedImperatives[imperativeCount++], word);
        fprintf(imperativeTable, "%s - %d\n", word, imperativeCounter++);
    } else if (isDeclarative(word) && !isProcessed(word, processedDeclaratives, declarativeCount)) {
        strcpy(processedDeclaratives[declarativeCount++], word);
        fprintf(declarativeTable, "%s - %d\n", word, declarativeCounter++);
    } else if (isLiteral(word) && !isProcessed(word, processedLiterals, literalCount)) {
        strcpy(processedLiterals[literalCount++], word);
        fprintf(literalTable, "%s - %d\n", word, literalCounter++);
    } else if (isSymbol(word) && !isProcessed(word, processedSymbols, symbolCount)) {
        strcpy(processedSymbols[symbolCount++], word);
        fprintf(symbolTable, "%s - %d\n", word, symbolCounter++);
    }
}

int main() {
    char word[50];
    FILE *inputFile = fopen("exmp.txt", "r");

    if (!inputFile) {
        printf("File could not be opened.\n");
        return 1;
    }

    // Open files to store the tables
    symbolTable = fopen("symbol_table.txt", "w");
    declarativeTable = fopen("declarative_table.txt", "w");
    registerTable = fopen("register_table.txt", "w");
    literalTable = fopen("literal_table.txt", "w");
    directiveTable = fopen("directive_table.txt", "w");
    imperativeTable = fopen("imperative_table.txt", "w");

    if (!symbolTable || !declarativeTable || !registerTable || !literalTable || !directiveTable || !imperativeTable) {
        printf("Error opening output files.\n");
        return 1;
    }

    // Read the input file word by word
    while (fscanf(inputFile, "%s", word) != EOF) {
        processWord(word);
    }

    // Close all files
    fclose(inputFile);
    fclose(symbolTable);
    fclose(declarativeTable);
    fclose(registerTable);
    fclose(literalTable);
    fclose(directiveTable);
    fclose(imperativeTable);

    printf("Tables have been successfully generated with unique entries and serialized opcodes.\n");

    return 0;
}
