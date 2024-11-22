#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 1000
#define MAX_MACROS 50
#define MAX_MACRO_BODY 5000
#define MAX_PARAMS 10
#define MAX_EXTERN_FILES 10
#define MAX_EXTERN_PROCS 50

// Structure to store external procedures
struct ExternProc
{
    char name[100];
    char filename[256];
    char body[MAX_MACRO_BODY];
    int processed;
};

// Structure to store external files
struct ExternFile
{
    char filename[256];
    int processed;
};

struct ExternProc extern_procs[MAX_EXTERN_PROCS];
struct ExternFile extern_files[MAX_EXTERN_FILES];
int extern_proc_count = 0;
int extern_file_count = 0;

struct Macro
{
    char name[100];
    char body[MAX_MACRO_BODY];
    char params[MAX_PARAMS][100];
    int param_count;
    int active;
};

struct Macro macros[MAX_MACROS];
int macro_count = 0;

// Function to check if a procedure is external
int is_extern_proc(const char *name)
{
    for (int i = 0; i < extern_proc_count; i++)
    {
        if (strcmp(extern_procs[i].name, name) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void trim(char *str)
{
    char *start = str;
    while (isspace(*start))
        start++;

    if (start != str)
    {
        memmove(str, start, strlen(start) + 1);
    }

    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
    {
        *end = '\0';
        end--;
    }
}

int starts_with(const char *str, const char *prefix)
{
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Function to process external file
void process_extern_file(const char *filename, FILE *output)
{
    FILE *extern_file = fopen("extern.asm", "r");
    if (!extern_file)
    {
        printf("Error: Cannot open external file %s\n", filename);
        return;
    }

    char line[MAX_LINE];
    int in_proc = 0;
    char current_proc[100] = "";

    while (fgets(line, MAX_LINE, extern_file))
    {
        trim(line);

        // Check for PUBLIC declaration
        if (starts_with(line, "PUBLIC"))
        {
            char proc_name[100];
            sscanf(line + 7, "%s", proc_name);
            strcpy(current_proc, proc_name);
            continue;
        }

        // Check for procedure start
        if (strstr(line, "PROC") && strlen(current_proc) > 0)
        {
            in_proc = 1;
            continue;
        }

        // Check for procedure end
        if (strcmp(line, "ENDP") == 0)
        {
            in_proc = 0;
            current_proc[0] = '\0';
            continue;
        }

        // Store procedure body
        if (in_proc && strlen(current_proc) > 0)
        {
            for (int i = 0; i < extern_proc_count; i++)
            {
                if (strcmp(extern_procs[i].name, current_proc) == 0)
                {
                    strcat(extern_procs[i].body, line);
                    strcat(extern_procs[i].body, "\n");
                    break;
                }
            }
        }
    }

    fclose(extern_file);
}


void parse_macro_params(char *line, struct Macro *macro)
{
    char *token;
    char *rest = strchr(line, ' ');
    if (!rest)
        return;

    macro->param_count = 0;
    token = strtok(rest, ",");
    while (token && macro->param_count < MAX_PARAMS)
    {
        trim(token);
        strcpy(macro->params[macro->param_count], token);
        macro->param_count++;
        token = strtok(NULL, ",");
    }
}

char *expand_macro_text(struct Macro *macro, char *call_line)
{
    static char expanded_body[MAX_MACRO_BODY];
    char *args[MAX_PARAMS];
    int arg_count = 0;

    // Parse actual parameters from macro call
    char call_copy[MAX_LINE];
    strcpy(call_copy, call_line);
    char *token = strtok(call_copy, " ,");
    token = strtok(NULL, ","); // Skip macro name
    while (token && arg_count < MAX_PARAMS)
    {
        trim(token);
        args[arg_count] = strdup(token);
        arg_count++;
        token = strtok(NULL, ",");
    }

    // Copy macro body and replace parameters
    strcpy(expanded_body, macro->body);
    char temp_body[MAX_MACRO_BODY];

    for (int i = 0; i < arg_count; i++)
    {
        char param_marker[10];
        sprintf(param_marker, "%%%d", i + 1);

        while (strstr(expanded_body, param_marker) != NULL)
        {
            char *pos = strstr(expanded_body, param_marker);
            *pos = '\0';
            strcpy(temp_body, expanded_body);
            strcat(temp_body, args[i]);
            strcat(temp_body, pos + strlen(param_marker));
            strcpy(expanded_body, temp_body);
        }
        free(args[i]);
    }

    return expanded_body;
}

void expand_macro(FILE *output, char *line)
{
    char expanded[MAX_MACRO_BODY];
    char *lines[MAX_LINE];
    int line_count = 0;

    // First, expand the current macro
    for (int i = 0; i < macro_count; i++)
    {
        if (starts_with(line, macros[i].name))
        {
            char *expanded_text = expand_macro_text(&macros[i], line);

            // Split expanded text into lines
            char *token = strtok(expanded_text, "\n");
            while (token)
            {
                lines[line_count] = strdup(token);
                line_count++;
                token = strtok(NULL, "\n");
            }
            break;
        }
    }

    // Then process each line for nested macros
    for (int i = 0; i < line_count; i++)
    {
        int is_nested_macro = 0;
        for (int j = 0; j < macro_count; j++)
        {
            if (starts_with(lines[i], macros[j].name))
            {
                expand_macro(output, lines[i]);
                is_nested_macro = 1;
                break;
            }
        }
        if (!is_nested_macro)
        {
            fprintf(output, "%s\n", lines[i]);
        }
        free(lines[i]);
    }
}

int is_extrn_declaration(const char *line)
{
    return starts_with(line, "EXTRN");
}

int main()
{
    FILE *input = fopen("spro.asm", "r");
    if (!input)
    {
        printf("Error: Cannot open input file\n");
        return 1;
    }

    FILE *output = fopen("expanded.asm", "w");
    if (!output)
    {
        printf("Error: Cannot create output file\n");
        fclose(input);
        return 1;
    }

    char line[MAX_LINE];
    int in_macro = 0;
    int current_macro = -1;

    // First pass: collect macros and external declarations
    while (fgets(line, MAX_LINE, input))
    {
        trim(line);

        // Check for EXTRN declarations
        if (starts_with(line, "EXTRN"))
        {
            char proc_name[100];
            sscanf(line + 6, "%[^:]", proc_name);
            trim(proc_name);

            strcpy(extern_procs[extern_proc_count].name, proc_name);
            extern_procs[extern_proc_count].processed = 0;
            extern_proc_count++;
            continue;
        }

        if (starts_with(line, "MACRO"))
        {
            in_macro = 1;
            current_macro = macro_count;

            char *name_start = line + 6;
            trim(name_start);
            char macro_name[100];
            sscanf(name_start, "%s", macro_name);

            strcpy(macros[macro_count].name, macro_name);
            macros[macro_count].body[0] = '\0';
            macros[macro_count].active = 1;
            parse_macro_params(name_start, &macros[macro_count]);
            macro_count++;
            continue;
        }

        if (in_macro)
        {
            if (strcmp(line, "ENDM") == 0)
            {
                in_macro = 0;
                continue;
            }

            if (current_macro >= 0)
            {
                strcat(macros[current_macro].body, line);
                strcat(macros[current_macro].body, "\n");
            }
            continue;
        }
    }

    // Process external files
    for (int i = 0; i < extern_proc_count; i++)
    {
        char extern_filename[256];
        sprintf(extern_filename, "%s.asm", extern_procs[i].name);
        process_extern_file(extern_filename, output);
    }

    // Reset file pointer for second pass
    rewind(input);
    in_macro = 0;

    // Second pass: expand macros
    while (fgets(line, MAX_LINE, input))
    {
        trim(line);

        // Skip EXTRN declarations in the output
        // if (is_extrn_declaration(line))
        // {
        //     continue;
        // }

        if (starts_with(line, "MACRO"))
        {
            in_macro = 1;
            continue;
        }

        if (in_macro)
        {
            if (strcmp(line, "ENDM") == 0)
            {
                in_macro = 0;
            }
            continue;
        }

        // Check if line is calling an external procedure
        // int is_extern_call = 0;
        // for (int i = 0; i < extern_proc_count; i++)
        // {
        //     char call_pattern[100];
        //     sprintf(call_pattern, "CALL %s", extern_procs[i].name);

        //     if (starts_with(line, call_pattern))
        //     {
        //         fprintf(output, "%s\n", extern_procs[i].body);
        //         is_extern_call = 1;
        //         break;
        //     }
        // }

        // If not an extern call, check for macro expansion
        // if (!is_extern_call)
        // {
            int is_macro = 0;
            for (int i = 0; i < macro_count; i++)
            {
                if (starts_with(line, macros[i].name))
                {
                    expand_macro(output, line);
                    is_macro = 1;
                    break;
                }
            }

            // If not a macro, write the line as is
            if (!is_macro && strlen(line) > 0)
            {
                fprintf(output, "%s\n", line);
            }
       // }
    }

    fclose(input);
    fclose(output);
    printf("Macro expansion completed successfully.\n");
    return 0;
}