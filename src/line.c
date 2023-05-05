#include "line.h"

/*
    La funzione crea una nuova riga, a partire dal chunk fornito in input;
    la funzione restituisce NULL se non è stato possibile allocare la riga.
*/
Line* new_line(LineChunk* line_chunk) {
    Line* new_line = calloc(1, sizeof(Line));

    if (new_line == NULL) {
        return NULL;
    }

    new_line->line_chunks_head = (struct LineChunk*) line_chunk;
    new_line->curr_line_chunk = (struct LineChunk*) line_chunk;

    new_line->next_line = NULL;

    return new_line;
}

/*
    La funzione crea una nuova riga, e aggiorna l'attributo 'next_line' della riga
    fornita in input con la riga appena creata; restituisce NULL se non è stato possibile
    allocare la nuova riga.
*/
Line* append_line(Line* curr_line, LineChunk* line_chunk) {
    Line* next_line = new_line(line_chunk);

    if (next_line == NULL) {
        return NULL;
    }

    if (curr_line != NULL) {
        curr_line->next_line = (struct Line*) next_line;
    }

    return next_line;
}

/*
    La funzione modifica il puntatore alla testa della lista puntata dei chunk,
    col puntatore al chunk corrente, solamente se il puntatore alla testa era NULL.
*/
void set_line_chunks_head(Line* line) {
    if (line != NULL && line->line_chunks_head == NULL) {
        line->line_chunks_head = line->curr_line_chunk;
    }
}

/*
    La funzione prende in input un Line**, ed il contenuto di un chunk, ed effettua
    2 operazioni: viene creato un nuovo chunk alla riga corrente, e viene avanzato
    il puntatore del chunk corrente al nuovo chunk appena aggiunto; la funzione restituisce
    NULL se non è stato possibile allocare il nuovo chunk, altrimenti restituisce il chunk creato.
*/
LineChunk* append_line_chunk_and_advance(Line** line_ptr, char* line_chunk_content) {
    if (line_ptr == NULL || *line_ptr == NULL) {
        return NULL;
    }

    LineChunk* new_line_chunk = (LineChunk*) append_line_chunk((LineChunk*) (*line_ptr)->curr_line_chunk, line_chunk_content);

    if (new_line_chunk == NULL) {
        return NULL;
    }

    (*line_ptr)->curr_line_chunk = (struct LineChunk*) new_line_chunk;

    return new_line_chunk;
}

/*
    La funzione stampa su 'output_file' i chunk contenuti nella lista puntata fornita
    in input, ricorsivamente; non viene stampato nulla se il carattere di separazione
    tra le colonne è '\0'.
*/
void print_line(FILE* output_file, Line* line, int spacing, char spacing_char) {
    if (output_file == NULL || line == NULL || spacing == 0 || spacing_char == '\0') {
        return;
    }

    LineChunk* curr_line_chunk = (LineChunk*) line->line_chunks_head;

    while (curr_line_chunk != NULL) {
        print_line_chunk(output_file, curr_line_chunk);

        LineChunk* next_line_chunk = (LineChunk*) curr_line_chunk->next_line_chunk;

        // questo controllo assicura che non venga
        // stampato il separatore tra chunk anche
        // dopo l'ultimo chunk della lista puntata
        if (next_line_chunk != NULL) {
            for (int i = 0; i < spacing; i++) {
                fprintf(output_file, "%c", spacing_char);
            }
        }

        curr_line_chunk = next_line_chunk;
    }

    fprintf(output_file, "\n");
}

/*
    La funzione libera la memoria della riga fornita in input, liberandone anche la lista puntata
    di chunk che contiene.
*/
void free_lines(Line* line) {
    if (line == NULL) {
        return;
    }

    free_lines((Line*) line->next_line);

    free_line_chunks((LineChunk*) line->line_chunks_head);

    free(line);
}