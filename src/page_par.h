#include <unistd.h>

#include "page.h"

#define READ_ERROR -5 // restituita se si è verificato un errore durante la lettura da una pipe
#define WRITE_ERROR -6 // resituita se si è verificato un errore durante la scrittura su una pipe

#ifndef PAGE_PAR_H
#define PAGE_PAR_H

int read_input_file_par(int* pipefd_rs, FILE* input_file, int cols, int h_col, int w_col);

int send_page(Page* curr_page, int* pipefd_sw, int cols, int w_col, int spacing, char spacing_char, bool is_last_page);
int build_pages_par(int* pipefd_rs, int* pipefd_sw, int cols, int h_col, int w_col, int spacing, char spacing_char);

int write_output_file_par(int* pipefd_sw, FILE* output_file, int h_col, int spacing, char* pages_separator);

#endif