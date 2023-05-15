#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "line.h"
#include "string_tools.h"

#define PAGE_SUCCESS 4 // restituita se non si sono verificati errori nella costruzione della struttura delle pagine
#define TRUNCATED_HANDLING_SUCCESS 3 // restituita se non si sono verificati errori nella gestione di una riga troncata
#define ENDED_PARAGRAPH 2 // restituita se la riga corrente era l'ultima di un paragrafo
#define ENDED_TEXT 1 // restituita se il chunk corrente era l'ultimo del file
#define NOT_ENDED_TEXT 0 // restituita se il file non è ancora terminato
#define INSUFFICIENT_WIDTH -1 // restituita se un chunk contiene una parola più larga della larghezza di colonna fornita
#define ALLOC_ERROR -2 // restituita se si sono verificati errori di allocazione della memoria
#define INVALID_INPUT -3 // restituita se si sono verificati degli errori nel fornire input ad alcune funzioni
#define FSEEK_ERROR -4 // restituita se la funzione fseek non è stata in grado di leggere il carattere alla posizione specificata

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

int read_chunk(FILE* input_file, char** line_chunk_content, int* w_col, long* base_idx, int* unicode_offset, bool is_first_line_first_col);
int handle_truncated_string(FILE* input_file, char** line_chunk_content, int* w_col, long* base_idx);
int build_pages(FILE* input_file, Page* curr_page, int cols, int h_col, int w_col);

void print_pages(FILE* output_file, Page* page, int spacing, char* pages_separator, char spacing_char);
void free_pages(Page* page);

int read_input_file_par(int* pipefd_rs, FILE* input_file, int cols, int h_col, int w_col);
int build_pages_par(int* pipefd_rs, int* pipefd_sw, int cols, int h_col, int spacing);
void write_output_file_par(int* pipefd_sw, FILE* output_file, int h_col, int spacing, char* pages_separator);

#endif