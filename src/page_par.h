#include "page.h"

#define READ_ERROR -5
#define WRITE_ERROR -6

#ifndef PAGE_PAR_H
#define PAGE_PAR_H

int read_input_file_par(int* pipefd_rs, FILE* input_file, int cols, int h_col, int w_col);
int build_pages_par(int* pipefd_rs, int* pipefd_sw, int cols, int h_col, int spacing, char spacing_char);
int write_output_file_par(int* pipefd_sw, FILE* output_file, int h_col, int spacing, char* pages_separator);

#endif