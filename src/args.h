#include <stdio.h>
#include <stdbool.h>

#include "string_tools.h"

#define ARGS_SUCCESS_PAR 2 // restituita se il parsing degli argomenti è andato a buon fine, e il prorgamma va eseguito multi-processo
#define PRINT_HELP 1 // restituita se la prima opzione fornita al programma era '--help'
#define ARGS_SUCCESS 0 // restituita se non si sono verificati problemi nel parsing degli argomenti
#define NOT_ENOUGH_ARGS -1 // restituita se il numero di argomenti passati al programma non era sufficiente
#define CHARS_IN_NUMERIC_ARG -2 // restituita se gli argomenti che dovrebbero contenere solamente cifre, non erano validi
#define ZERO_INTEGER -3 // restituita se almeno un intero in input conteneva un valore nullo
#define TOO_MANY_ARGS -4 // restituita se il numero di argomenti fornito era superiore al numero consentito

#ifndef ARGS_H
#define ARGS_H

// il messaggio di help
static const char* HELP_MESSAGE =
    "Usage:\n"
    "\n"
    "    newspaper <INPUT_FILE> <OUTPUT_FILE> <COLS> <H_COL> <W_COL> <SPACING>\n"
    "\n"
    "    INPUT_FILE:\n"
    "        the path of the input file; a string.\n"
    "    OUTPUT_FILE:\n"
    "        the path of the output file; a string.\n"
    "    COLS:\n"
    "        the number of columns per page; a strictly positive integer.\n"
    "    H_COL:\n"
    "        the number of rows per page; a strictly positive integer.\n"
    "    W_COL:\n"
    "        the number of characters per column; a strictly positive integer.\n"
    "    SPACING:\n"
    "        the number of characters between each column; a strictly positive integer.\n"
    "\n"
    "Options:\n"
    "\n"
    "    --help | -h: prints this message.\n"
    "    --par | -p: runs the program with multiple processes.\n"
;

bool is_file_empty(FILE* file);

int find_flag(int argc, char* argv[], char* flag, char* flag_short);
int set_index(int index, int flag_index);
int parse_args(int argc, char* argv[], char** input_path, char** output_path, unsigned int* cols, unsigned int* h_col, unsigned int* w_col, unsigned int* spacing);

#endif