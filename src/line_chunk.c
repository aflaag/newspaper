#include "line_chunk.h"

/*
    La funzione crea un nuovo chunk, a partire dalla stringa fornita in input;
    la funzione restituisce NULL se non è stato possibile allocare il nuovo chunk.
*/
LineChunk* new_line_chunk(char* content) {
    LineChunk* new_line_chunk = calloc(1, sizeof(LineChunk));

    if (new_line_chunk == NULL) {
        return NULL;
    }

    new_line_chunk->content = content;
    new_line_chunk->next_line_chunk = NULL;

    return new_line_chunk;
}

/*
    La funzione crea un nuovo chunk, e aggiorna l'attributo 'next_line_chunk' del chunk
    fornito in input con il chunk appena creato; restituisce NULL se non è stato possibile
    allocare il nuovo chunk.
*/
LineChunk* append_line_chunk(LineChunk* curr_line_chunk, char* content) {
    LineChunk* next_line_chunk = new_line_chunk(content);

    if (next_line_chunk == NULL) {
        return NULL;
    }

    if (curr_line_chunk != NULL) {
        curr_line_chunk->next_line_chunk = (struct LineChunk*) next_line_chunk;
    }

    return next_line_chunk;
}

/*
    La funzione, se non ha ricevuto in input puntatori a NULL, stampa sul file di output fornito
    la stringa contenuta all'interno del chunk in input (se non è NULL anch'esso).
*/
void print_line_chunk(FILE* output_file, LineChunk* line_chunk) {
    if (output_file == NULL || line_chunk == NULL) {
        return;
    }

    if (line_chunk->content != NULL) {
        fprintf(output_file, "%s", line_chunk->content);
    }
}

// TODO: TESTALA PRIMA
void free_line_chunks(LineChunk* line_chunk) {
    if (line_chunk == NULL) {
        return;
    }

    free_line_chunks((LineChunk*) line_chunk->next_line_chunk);

    free(line_chunk->content);

    free(line_chunk);
}