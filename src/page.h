#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "line.h"
#include "string_tools.h"

#define PAGE_SUCCESS 4
#define TRUNCATED_HANDLING_SUCCESS 3
#define ENDED_PARAGRAPH 2
#define ENDED_TEXT 1
#define NOT_ENDED_TEXT 0
#define INSUFFICIENT_WIDTH -1
#define ALLOC_ERROR -2
#define INVALID_INPUT -3

#ifndef PAGE_H
#define PAGE_H

/*
    Struttura dati che rappresenta le pagine dell'output del programma
    attraverso una lista puntata.
*/
typedef struct {
    struct Line* lines_head; // il puntatore alla testa della lista puntata delle righe della pagina
    struct Line* curr_line; // il puntatore alla riga corrente

    struct Page* next_page; // il puntatore alla prossima pagina
} Page;

Page* new_page(Line* line);
Page* append_page(Page* curr_page, Line* line);

Line* append_line_and_advance(Page** page_ptr, LineChunk* line_chunk);

void advance_curr_line(Page* page);
void set_lines_head(Page* page);
void reset_lines_head(Page* page);

int read_chunk(FILE* input_file, char** line_chunk_content, int* w_col, long* base_idx, int* unicode_offset);
int handle_truncated_string(FILE* input_file, char** line_chunk_content, int* w_col, long* base_idx);
int build_pages(FILE* input_file, Page* curr_page, int cols, int h_col, int w_col);

void print_pages(FILE* output_file, Page* page, int spacing, char* pages_separator, char spacing_char);
void free_pages(Page* page);

#endif