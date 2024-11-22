#include <stdio.h>
#include <string.h>

#define MAX_LINES 50
#define MAX_LINE_LENGTH 100
#define MAX_ARGS 10

typedef struct {
    char name[10];
    char args[MAX_ARGS][10];
    char body[MAX_LINES][MAX_LINE_LENGTH];
    int numArgs;
    int numLines;
} Macro;

Macro macros[10]; // Store defined macros
int macroCount = 0; // Count of defined macros

// Function to define a new macro
void defineMacro(char *name, char args[][10], int argCount, char body[][MAX_LINE_LENGTH], int bodyLineCount) {
    strcpy(macros[macroCount].name, name);
    macros[macroCount].numArgs = argCount;
    macros[macroCount].numLines = bodyLineCount;

    for (int i = 0; i < argCount; i++) {
        strcpy(macros[macroCount].args[i], args[i]);
    }
    for (int i = 0; i < bodyLineCount; i++) {
        strcpy(macros[macroCount].body[i], body[i]);
    }
    macroCount++;
}

// Function to find a macro by name
int findMacro(char *name) {
    for (int i = 0; i < macroCount; i++) {
        if (strcmp(macros[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to expand the macro
void expandMacro(int macroIndex, char actualArgs[][10]) {
    Macro macro = macros[macroIndex];

    for (int i = 0; i < macro.numLines; i++) {
        char expandedLine[MAX_LINE_LENGTH];
        strcpy(expandedLine, macro.body[i]);

        for (int j = 0; j < macro.numArgs; j++) {
            char placeholder[10] = "&";
            strcat(placeholder, macro.args[j]);
            char *pos = strstr(expandedLine, placeholder);

            if (pos != NULL) {
                char tempLine[MAX_LINE_LENGTH];
                strncpy(tempLine, expandedLine, pos - expandedLine);
                tempLine[pos - expandedLine] = '\0';
                strcat(tempLine, actualArgs[j]);
                strcat(tempLine, pos + strlen(placeholder));
                strcpy(expandedLine, tempLine);
            }
        }
        printf("%s\n", expandedLine); // Output the expanded line
    }
}

int main() {
    // Define the SWAP macro
    char macroName[] = "SWAP";
    char macroArgs[2][10] = {"X", "Y"};
    char macroBody[3][MAX_LINE_LENGTH] = {
        "TEMP = &X",
        "&X = &Y",
        "&Y = TEMP"
    };
    defineMacro(macroName, macroArgs, 2, macroBody, 3);

    // Define variables for testing
    printf("VAR1 = 10\n");
    printf("VAR2 = 20\n");

    // Call the SWAP macro with arguments VAR1 and VAR2
    char callArgs[2][10] = {"VAR1", "VAR2"};
    int macroIndex = findMacro("SWAP");

    if (macroIndex != -1) {
        expandMacro(macroIndex, callArgs);
    } else {
        printf("Error: Macro not found.\n");
    }

    printf("END\n");
    return 0;
}
