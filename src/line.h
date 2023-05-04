#include <stdio.h>
#include <stdlib.h>

#include "line_chunk.h"

#ifndef LINE_H
#define LINE_H

/*
    Struttura dati che rappresenta le righe dell'output del programa
    attraverso una lista puntata (all'interno di ogni pagina).
*/
typedef struct {
    struct LineChunk* line_chunks_head; // il puntatore alla testa della lista puntata dei chunk della riga
    struct LineChunk* curr_line_chunk; // il puntatore al chunk corrente

    struct Line* next_line; // il puntatore alla prossima riga
} Line;

Line* new_line(LineChunk* line_chunk);
Line* append_line(Line* curr_line, LineChunk* line_chunk);
LineChunk* append_line_chunk_and_advance(Line** line_ptr, char* line_chunk_content);

void set_line_chunks_head(Line* line);

void print_line(FILE* output_file, Line* line, int spacing, char spacing_char);
void free_lines(Line* line);

#endif