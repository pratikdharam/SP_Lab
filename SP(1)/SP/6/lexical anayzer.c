#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Define token types
#define KEYWORD 1
#define IDENTIFIER 2
#define OPERATOR 3
#define DELIMITER 4
#define CONSTANT 5
#define ERROR 6

// Data structures for keywords, operators, and delimiters
const char *keywords[] = {"if", "else", "int", "return", "for", "while", NULL};
const char *operators[] = {"+", "-", "*", "/", "=", ">", "<", ">=", "<=", "==", "!=", NULL};
const char *delimiters[] = {"(", ")", "{", "}", ";", ",", NULL};

// Function to check if a string is a keyword
int isKeyword(const char *str) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return i + 1;
        }
    }
    return 0;
}

// Function to check if a character is an operator
int isOperator(const char *str) {
    for (int i = 0; operators[i] != NULL; i++) {
        if (strcmp(str, operators[i]) == 0) {
            return i + 1;
        }
    }
    return 0;
}

// Function to check if a character is a delimiter
int isDelimiter(const char *str) {
    for (int i = 0; delimiters[i] != NULL; i++) {
        if (strcmp(str, delimiters[i]) == 0) {
            return i + 1;
        }
    }
    return 0;
}

// Function to check if a string is an identifier
int isIdentifier(const char *str) {
    if (!isalpha(str[0]) && str[0] != '_') {
        return 0;
    }
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') {
            return 0;
        }
    }
    return 1;
}

// Function to check if a string is a constant
int isConstant(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Lexical analyzer function
void lexicalAnalyzer(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    int lineNo = 0;
    int openParentheses = 0; // To track unmatched parentheses

    printf("Line No\tLexeme\t\tToken\t\tToken Value\n");

    while (fgets(line, sizeof(line), file)) {
        lineNo++;
        char *token = strtok(line, " \t\n");
        while (token) {
            int tokenValue;

            if ((tokenValue = isKeyword(token))) {
                printf("%d\t%s\t\tKeyword\t\t%d\n", lineNo, token, tokenValue);
            } else if ((tokenValue = isDelimiter(token))) {
                printf("%d\t%s\t\tDelimiter\t%d\n", lineNo, token, tokenValue);
                if (strcmp(token, "(") == 0) openParentheses++;
                if (strcmp(token, ")") == 0) openParentheses--;
            } else if ((tokenValue = isOperator(token))) {
                printf("%d\t%s\t\tOperator\t%d\n", lineNo, token, tokenValue);
            } else if (isConstant(token)) {
                printf("%d\t%s\t\tConstant\t-\n", lineNo, token);
            } else if (isIdentifier(token)) {
                printf("%d\t%s\t\tIdentifier\t-\n", lineNo, token);
            } else {
                printf("%d\t%s\t\tError\t\tInvalid Token\n", lineNo, token);
            }

            token = strtok(NULL, " \t\n");
        }
    }

    fclose(file);

    // Error checking: unmatched parentheses
    if (openParentheses != 0) {
        printf("\nError: Unmatched parentheses detected.\n");
    }
}

int main() {
    const char *filename = "ass.c";

    printf("\nLexical Analysis for %s:\n", filename);
    lexicalAnalyzer(filename);

    return 0;
}
