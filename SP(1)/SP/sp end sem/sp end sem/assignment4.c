#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_MDT_SIZE 100
#define MAX_MNT_SIZE 20
#define MAX_PNT_SIZE 10
#define MAX_NAME_LENGTH 20

// Data structures
typedef struct {
    char name[MAX_NAME_LENGTH];
    int mdt_index;
} MNT;

typedef struct {
    char definition[MAX_LINE_LENGTH];
} MDT;

typedef struct {
    char params[MAX_PNT_SIZE][MAX_NAME_LENGTH];
    int param_count;
} PNT;

// Global tables
MNT mnt[MAX_MNT_SIZE];
MDT mdt[MAX_MDT_SIZE];
PNT pnt[MAX_MNT_SIZE];
int mnt_count = 0, mdt_count = 0, pnt_count = 0;

// Function prototypes
void addMacroToMNT(char *name, int index);
void addDefinitionToMDT(char *definition);
void addParameterToPNT(int index, char *param);
void writeTablesToFiles();
void processAssemblyProgram(FILE *input_file);

int main() {
    // Open the assembly program file
    FILE *input_file = fopen("assembly_program.asm", "r");
    if (input_file == NULL) {
        perror("Failed to open input file");
        return 1;
    }

    // Process the assembly program
    processAssemblyProgram(input_file);

    // Close the file and write tables to files
    fclose(input_file);
    writeTablesToFiles();

    printf("Macro processing completed. Tables written to files.\n");
    return 0;
}

// Add a macro name to the Macro Name Table (MNT)
void addMacroToMNT(char *name, int index) {
    strcpy(mnt[mnt_count].name, name);
    mnt[mnt_count].mdt_index = index;
    mnt_count++;
}

// Add a line of macro definition to the Macro Definition Table (MDT)
void addDefinitionToMDT(char *definition) {
    strcpy(mdt[mdt_count].definition, definition);
    mdt_count++;
}

// Add a parameter to the Parameter Name Table (PNT)
void addParameterToPNT(int index, char *param) {
    strcpy(pnt[index].params[pnt[index].param_count], param);
    pnt[index].param_count++;
}

// Write the tables to files
void writeTablesToFiles() {
    // Write Macro Name Table (MNT) to file
    FILE *mnt_file = fopen("mnt.txt", "w");
    if (mnt_file == NULL) {
        perror("Failed to open MNT file");
        return;
    }
    fprintf(mnt_file, "Macro Name Table (MNT):\n");
    for (int i = 0; i < mnt_count; i++) {
        fprintf(mnt_file, "Name: %s, MDT Index: %d\n", mnt[i].name, mnt[i].mdt_index);
    }
    fclose(mnt_file);

    // Write Macro Definition Table (MDT) to file
    FILE *mdt_file = fopen("mdt.txt", "w");
    if (mdt_file == NULL) {
        perror("Failed to open MDT file");
        return;
    }
    fprintf(mdt_file, "Macro Definition Table (MDT):\n");
    for (int i = 0; i < mdt_count; i++) {
        fprintf(mdt_file, "MDT[%d]: %s\n", i, mdt[i].definition);
    }
    fclose(mdt_file);

    // Write Parameter Name Table (PNT) to file
    FILE *pnt_file = fopen("pnt.txt", "w");
    if (pnt_file == NULL) {
        perror("Failed to open PNT file");
        return;
    }
    fprintf(pnt_file, "Parameter Name Table (PNT):\n");
    for (int i = 0; i < mnt_count; i++) {
        fprintf(pnt_file, "PNT for macro '%s':\n", mnt[i].name);
        for (int j = 0; j < pnt[i].param_count; j++) {
            fprintf(pnt_file, "PNT[%d]: %s\n", j, pnt[i].params[j]);
        }
    }
    fclose(pnt_file);
}

// Process the assembly program to extract macros and generate tables
void processAssemblyProgram(FILE *input_file) {
    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_NAME_LENGTH];
    int in_macro = 0;
    int current_macro_index = -1;

    while (fgets(line, sizeof(line), input_file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        if (strstr(line, "MACRO")) {
            // Start of a macro definition
            in_macro = 1;
            sscanf(line, "MACRO %s", macro_name);
            addMacroToMNT(macro_name, mdt_count);
            current_macro_index = mnt_count - 1;
        } else if (strstr(line, "MEND")) {
            // End of a macro definition
            in_macro = 0;
            current_macro_index = -1;
        } else if (in_macro) {
            // Inside a macro definition
            char *param_start = strchr(line, '&');
            if (param_start) {
                // Extract parameters
                while (param_start) {
                    char param[MAX_NAME_LENGTH];
                    sscanf(param_start, "%s", param);
                    char *param_end = strchr(param, ' ');
                    if (param_end) *param_end = '\0';
                    addParameterToPNT(current_macro_index, param);
                    param_start = strchr(param_start + 1, '&');
                }
            }
            addDefinitionToMDT(line);
        }
    }
}
