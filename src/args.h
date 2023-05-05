#include <stdio.h>
#include <stdbool.h>

#include "string_tools.h"

#define ARGS_SUCCESS 0 // restituita se non sono stati riscontrati problemi nel parsing degli argomenti
#define NOT_ENOUGH_ARGS -1 // restituita se il numero di argomenti passati al programma non era sufficiente
#define CHARS_IN_NUMERIC_ARG -2 // restituita se gli argomenti che dovrebbero contenere solamente cifre, non erano validi
#define ZERO_INTEGER -3 // restituita se almeno un intero in input conteneva un valore nullo
#define PRINT_HELP -4

#ifndef ARGS_H
#define ARGS_H

bool is_file_empty(FILE* file);

int parse_args(int argc, char* argv[], char** input_path, char** output_path, int* cols, int* h_col, int* w_col, int* spacing);

#endif