#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "string_tools.h"

#define ARGS_SUCCESS 0
#define NOT_ENOUGH_ARGS -1
#define CHARS_IN_NUMERIC_ARG -2
#define ZERO_INTEGER -3
#define PRINT_HELP -4

#ifndef ARGS_H
#define ARGS_H

bool is_file_empty(FILE* file);

int parse_args(int argc, char* argv[], char** input_path, char** output_path, int* cols, int* h_col, int* w_col, int* spacing);

#endif