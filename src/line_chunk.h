#include <stdio.h>
#include <stdlib.h>

#ifndef LINE_CHUNK_H
#define LINE_CHUNK_H

/*
    Struttura dati che rappresenta le righe delle colonen dell'output del programma
    attraverso una lista puntata (all'interno di ogni riga).
*/
typedef struct {
    char* content; // la stringa contenuta nel chunk

    struct LineChunk* next_line_chunk; // il puntatore al prossimo chunk della lista puntata
} LineChunk;

LineChunk* new_line_chunk(char* content);
LineChunk* append_line_chunk(LineChunk* curr_line_chunk, char* content);

void print_line_chunk(FILE* output_file, LineChunk* line_chunk);
void free_line_chunks(LineChunk* line_chunk);

#endif