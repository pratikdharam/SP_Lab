#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 100
#define MAX_TOKEN_LENGTH 50

// Define registers for this hypothetical assembly language
const char *registers[] = {"AX", "BX", "CX", "DX"};
const int num_registers = 4;

// Function to check if a token is a register
int is_register(const char *token) {
    for (int i = 0; i < num_registers; i++) {
        if (strcmp(token, registers[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to check if a token is a constant
int is_constant(const char *token) {
    for (int i = 0; token[i] != '\0'; i++) {
        if (!isdigit(token[i])) {
            return 0;
        }
    }
    return 1;
}

// Function to write a token to a file
void write_to_file(const char *filename, const char *token) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s\n", token);
    fclose(file);
}

// Function to process a single line of input
void process_line(const char *line) {
    char token[MAX_TOKEN_LENGTH];
    int i = 0, j = 0;

    while (line[i] != '\0') {
        // Split tokens on spaces, commas, or colons
        if (isspace(line[i]) || line[i] == ',' || line[i] == ':') {
            if (j > 0) {
                token[j] = '\0'; // Null-terminate the token
                if (is_register(token)) {
                    write_to_file("register.txt", token);
                } else if (is_constant(token)) {
                    write_to_file("constant.txt", token);
                } else {
                    write_to_file("symbol.txt", token);
                }
                j = 0;
            }
        } else {
            token[j++] = line[i];
        }
        i++;
    }

    // Process the last token in the line (if any)
    if (j > 0) {
        token[j] = '\0';
        if (is_register(token)) {
            write_to_file("register.txt", token);
        } else if (is_constant(token)) {
            write_to_file("constant.txt", token);
        } else {
            write_to_file("symbol.txt", token);
        }
    }
}

int main() {
    FILE *input_file = fopen("input.asm", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    // Clear the output files before writing new data
    fclose(fopen("register.txt", "w"));
    fclose(fopen("symbol.txt", "w"));
    fclose(fopen("constant.txt", "w"));

    // Process each line from the input file
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), input_file)) {
        process_line(line);
    }

    fclose(input_file);
    printf("Token processing complete. Check register.txt, constant.txt, and symbol.txt for results.\n");
    return 0;
}
