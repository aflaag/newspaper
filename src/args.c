#include "args.h"

/*
    La funzione prende in input un file, e restituisce true se il puntatore al file fornito è vuoto;
    si noti che la funzione restituisce true anche se il puntatore è NULL, e restituisce false anche se
    la 'fseek' ha restituito errore.
*/
bool is_file_empty(FILE* file) {
    if (file != NULL) {
        if (fseek(file, 0, SEEK_END)) {
            return false;
        }

        if (ftell(file) != 0) {
            return false;
        }
    }

    return true;
}

/*
    La funzione prende in input i puntatori agli argomenti, ed effettua il parsing degli argomenti
    forniti in input al programma; consultare la documentazione per maggiori informazioni.
*/
int parse_args(int argc, char* argv[], char** input_path, char** output_path, int* cols, int* h_col, int* w_col, int* spacing) {
    if (argc < 7) {
        if (!strcmp(argv[1], "--help")) { // gli altri argomenti verranno ignorati
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