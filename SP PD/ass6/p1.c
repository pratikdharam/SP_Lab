#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
// Token types
enum TokenType {
KEYWORD, IDENTIFIER, OPERATOR, LITERAL, SPECIAL_SYMBOL
};
// Keywords in C
const char *keywords[] = {
"int", "float", "char", "if", "else", "for", "while", "return", "void",
"double", "struct", "union", "const", "unsigned", "signed", "switch",
"case", "break", "default", "continue", "do", "goto", "enum", "static",
"sizeof", "typedef", "volatile", "extern", "auto", NULL
};
// Function to check if a string is a keyword
int isKeyword(const char *str) {
for (int i = 0; keywords[i] != NULL; i++) {
if (strcmp(str, keywords[i]) == 0) {
return 1;
}
}
return 0;
}
// Function to check if a character is an operator
int isOperator(char ch) {
return strchr("+-*/%=<>!&|^", ch) != NULL;
}
// Function to check if a character is a special symbol
int isSpecialSymbol(char ch) {
return strchr("(){}[],;:", ch) != NULL;
}
// Function to classify tokens
void classifyToken(const char *token, enum TokenType type) {
switch (type) {
case KEYWORD: printf("KEYWORD, %s\n", token); break;
case IDENTIFIER: printf("IDENTIFIER, %s\n", token); break;
case OPERATOR: printf("OPERATOR, %s\n", token); break;
case LITERAL: printf("LITERAL, %s\n", token); break;
case SPECIAL_SYMBOL: printf("SPECIAL_SYMBOL, %s\n", token); break;
}
}
// Main function for lexical analysis
void lexicalAnalyzer(const char *filename) {
FILE *file = fopen(filename, "r");
if (!file) {
perror("File opening failed");
return;
}
char ch, token[100];
int idx = 0;
while ((ch = fgetc(file)) != EOF) {
// Skip whitespace
if (isspace(ch)) continue;
// Skip single-line comments
if (ch == '/' && (ch = fgetc(file)) == '/') {
while ((ch = fgetc(file)) != '\n' && ch != EOF);
continue;
}
// Skip multi-line comments
if (ch == '/' && ch == '*') {
while ((ch = fgetc(file)) != EOF) {
if (ch == '*' && (ch = fgetc(file)) == '/') break;
}
continue;
}
// Process keywords and identifiers
if (isalpha(ch) || ch == '_') {
idx = 0;
token[idx++] = ch;
while (isalnum((ch = fgetc(file))) || ch == '_') {
token[idx++] = ch;
}
ungetc(ch, file);
token[idx] = '\0';
if (isKeyword(token)) classifyToken(token, KEYWORD);
else classifyToken(token, IDENTIFIER);
}
// Process numbers (literals)
else if (isdigit(ch)) {
idx = 0;
token[idx++] = ch;
while (isdigit((ch = fgetc(file))) || ch == '.') {
token[idx++] = ch;
}
ungetc(ch, file);
token[idx] = '\0';
classifyToken(token, LITERAL);
}
// Process operators
else if (isOperator(ch)) {
idx = 0;
token[idx++] = ch;
char next_ch = fgetc(file);
// Check for two-character operators like ==, !=, <=, >=
if ((ch == '=' && next_ch == '=') ||
(ch == '!' && next_ch == '=') ||
(ch == '<' && next_ch == '=') ||
(ch == '>' && next_ch == '=')) {
token[idx++] = next_ch;
} else {
ungetc(next_ch, file);
}
token[idx] = '\0';
classifyToken(token, OPERATOR);
}
// Process special symbols
else if (isSpecialSymbol(ch)) {
token[0] = ch;
token[1] = '\0';
classifyToken(token, SPECIAL_SYMBOL);
}
}
fclose(file);
}
// Main entry point
int main() {
lexicalAnalyzer("Sample.c"); // Replace with your C source file name
return 0;
}