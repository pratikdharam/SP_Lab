#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MNT 10
#define MAX_MDT 100
#define MAX_PNT 10
#define MAX_BUFFER 256

typedef struct {
    char macroName[20];
    int mdtIndex;
    int numParams;
} MNT;

typedef struct {
    char instruction[MAX_BUFFER];
} MDT;

MNT mnt[MAX_MNT];
MDT mdt[MAX_MDT];
char apt[MAX_PNT][20];
char pnt[MAX_PNT][20];

int mntCount = 0, mdtCount = 0, pntCount = 0;

void addToMNT(char *name, int mdtIndex, int numParams) {
    strcpy(mnt[mntCount].macroName, name);
    mnt[mntCount].mdtIndex = mdtIndex;
    mnt[mntCount].numParams = numParams;
    mntCount++;
}

void addToMDT(char *line) {
    strcpy(mdt[mdtCount].instruction, line);
    mdtCount++;
}

void addToPNT(char *param) {
    strcpy(pnt[pntCount], param);
    pntCount++;
}

int findInMNT(char *name) {
    for (int i = 0; i < mntCount; i++) {
        if (strcmp(mnt[i].macroName, name) == 0) {
            return i;
        }
    }
    return -1;
}

void processMacroDefinition(FILE *inputFile) {
    char buffer[MAX_BUFFER];
    char macroName[20];
    
    fscanf(inputFile, "%s", macroName);
    
    pntCount = 0;
    char params[MAX_BUFFER];
    fgets(params, MAX_BUFFER, inputFile);
    char *token = strtok(params, ", ");
    while (token != NULL) {
        addToPNT(token);
        token = strtok(NULL, ", ");
    }
    
    addToMNT(macroName, mdtCount, pntCount);

    while (fgets(buffer, MAX_BUFFER, inputFile)) {
        if (strstr(buffer, "ENDM") != NULL) {
            addToMDT("ENDM\n");
            break;
        }
        addToMDT(buffer);
    }
}

void expandMacro(char *name, char *params) {
    int mntIndex = findInMNT(name);
    if (mntIndex == -1) {
        printf("Error: Macro %s not found!\n", name);
        return;
    }

    int aptCount = 0;
    char *token = strtok(params, ",");
    while (token != NULL) {
        strcpy(apt[aptCount], token);
        aptCount++;
        token = strtok(NULL, ",");
    }

    int mdtIndex = mnt[mntIndex].mdtIndex;
    while (mdtIndex < mdtCount && strstr(mdt[mdtIndex].instruction, "ENDM") == NULL) {
        char line[MAX_BUFFER];
        strcpy(line, mdt[mdtIndex].instruction);
        
        for (int i = 0; i < aptCount; i++) {
            char param[20];
            sprintf(param, "%s", pnt[i]);
            char *pos = strstr(line, param);
            if (pos != NULL) {
                strncpy(pos, apt[i], strlen(apt[i]));
            }
        }
        strcpy(mdt[mdtIndex].instruction, line);
        printf("%s", line);
        mdtIndex++;
    }
}

void printMNT() {
    printf("\nMNT (Macro Name Table):\n");
    printf("Macro Name\tMDT Index\t#Params\n");
    for (int i = 0; i < mntCount; i++) {
        printf("%s\t\t%d\t\t%d\n", mnt[i].macroName, mnt[i].mdtIndex, mnt[i].numParams);
    }
}

void printMDT() {
    printf("\nMDT (Macro Definition Table):\n");
    printf("MDT Index\tInstruction\n");
    for (int i = 0; i < mdtCount; i++) {
        printf("%d\t\t%s", i, mdt[i].instruction);
    }
}
void printPNT() {
    printf("\nPNT (Parameter Name Table):\n");
    for (int i = 0; i < pntCount; i++) {
        printf("&ARG%d -> %s\n", i+1, pnt[i]);
    }
}

int main() {
    FILE *inputFile = fopen("assembly.txt", "r");
    if (inputFile == NULL) {
        printf("Error: Cannot open input file.\n");
        return 1;
    }

    char buffer[MAX_BUFFER];
    while (fgets(buffer, MAX_BUFFER, inputFile)) {
        if (strstr(buffer, "MACRO") != NULL) {
            processMacroDefinition(inputFile);
        } else if (strstr(buffer, "MACRO1") != NULL) {
            char params[MAX_BUFFER];
            sscanf(buffer, "MACRO1 %s", params);
            expandMacro("MACRO1", params);
        } else {
            printf("%s", buffer);
        }
    }

    fclose(inputFile);
    printMNT();
    printMDT();
    printPNT();
    return 0;
}

