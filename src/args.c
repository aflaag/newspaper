#include "args.h"

#include "string_tools.h"

// TODO: mettere nella doc che se il file è NULL restituisce true
bool is_file_empty(FILE* file) {
    if (file != NULL) {
        fseek(file, 0, SEEK_END);

        if (ftell(file) != 0) {
            return false;
        }
    }

    return true;
}

int parse_args(int argc, char* argv[], char** input_path, char** output_path, int* cols, int* h_col, int* w_col, int* spacing) {
    if (argc < 7) {
        if (argv[1] == "--help") {
            return PRINT_HELP;
        }

        return NOT_ENOUGH_ARGS;
    }

    *input_path = argv[1];
    *output_path = argv[2];

    char* raw_cols = argv[3];
    char* raw_h_col = argv[4];
    char* raw_w_col = argv[5];
    char* raw_spacing = argv[6];

    if (ATOI_INVALID_CHAR == pos_atoi(raw_cols, strlen(raw_cols), cols)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (ATOI_INVALID_CHAR == pos_atoi(raw_h_col, strlen(raw_h_col), h_col)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (ATOI_INVALID_CHAR == pos_atoi(raw_w_col, strlen(raw_w_col), w_col)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (ATOI_INVALID_CHAR == pos_atoi(raw_spacing, strlen(raw_spacing), spacing)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (*cols == 0 || *h_col == 0 || *w_col == 0 || *spacing == 0) {
        return ZERO_INTEGER;
    }

    return ARGS_SUCCESS;
}