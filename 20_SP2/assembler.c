#include <stdio.h>
#include <string.h>

typedef struct {
    char label[10];
    int address;
} Symbol;

// Function to generate relocation and link tables
void generateRelocationAndLinkTables(Symbol symbols[], int symbolCount) {
    FILE *relocationTableOutput = fopen("relocation_table.txt", "w");
    FILE *linkTableOutput = fopen("link_table.txt", "w");
    FILE *objectModuleOutput = fopen("object_module.txt", "w");

    if (!relocationTableOutput || !linkTableOutput || !objectModuleOutput) {
        perror("Error creating output files");
        return;
    }

    // Location factor (200) to be added
    int locationFactor = 200;

    // Write Relocation Table Header (Addresses Only)
    fprintf(relocationTableOutput, "Address\n");

    // Only output addresses for the relocatable symbols
    for (int i = 0; i < symbolCount; i++) {
        // Write to the relocation table if it is a relocatable label
        if (strcmp(symbols[i].label, "LOOP") == 0 || 
            strcmp(symbols[i].label, "INNER") == 0 || 
            strcmp(symbols[i].label, "SKIP") == 0 || 
            strcmp(symbols[i].label, "ENDLOOP") == 0) {
            fprintf(relocationTableOutput, "%d\n", symbols[i].address);
        }
    }

    // Write Link Table Header (Only include ARR as the globally defined variable)
    fprintf(linkTableOutput, "Label\tType\tAddress\n");
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbols[i].label, "ARR") == 0) {
            fprintf(linkTableOutput, "%s\tPD\t%d\n", symbols[i].label, symbols[i].address);
        }
    }

    // Write Object Module with Machine Code adjusted for the Location Factor
    fprintf(objectModuleOutput, "Object Module\n");
    fprintf(objectModuleOutput, "Header: Location Factor = %d\n", locationFactor);  // Include location factor in header

    // Original machine code for reference (address adjustments)
    int addresses[] = {100, 103, 106, 109, 104, 110, 113, 116, 118, 121, 124, 127, 130, 133, 136, 139, 142, 145, 200, 201, 202, 203, 204};
    char machineCode[][20] = {   // Increased the size of the array elements to 20 characters
        "05 00 202", "01 01 202", "02 01 200", "04 01 203", "01 01 203", "03 01 201",
        "07 01 128", "01 02 202", "01 01 204", "03 01 203", "07 01 118", "08 01 204",
        "02 01 200", "07 01 110", "02 01 203", "07 01 104", "09 00 204", "00 00 000",
        "00 00 001", "00 00 000", "00 00 005", "00 00 000", "00 09 05 08 07 04"
    };

    // Generating the machine code and adjusting addresses
    fprintf(objectModuleOutput, "Machine Code Section:\n");
    for (int i = 0; i < sizeof(addresses) / sizeof(addresses[0]); i++) {
        // Adjust address by adding the location factor
        int adjustedAddress = addresses[i] + locationFactor;
        fprintf(objectModuleOutput, "%d %s\n", adjustedAddress, machineCode[i]);
    }

    fclose(relocationTableOutput);
    fclose(linkTableOutput);
    fclose(objectModuleOutput);
}

int main() {
    // Define the symbols and their addresses (before adjustment)
    Symbol symbols[] = {
        {"LOOP", 104},
        {"INNER", 110},
        {"SKIP", 118},
        {"ENDLOOP", 128},
        {"ONE", 200},
        {"ZERO", 201},
        {"N", 202},
        {"COUNT", 203},
        {"ARR", 204},      // ARR is the global variable to be added to the link table
    };

    int symbolCount = sizeof(symbols) / sizeof(symbols[0]);

    // Generate the relocation and link tables, and object module with adjusted addresses
    generateRelocationAndLinkTables(symbols, symbolCount);

    printf("Relocation table, link table, and object module generated.\n");
    return 0;
}
