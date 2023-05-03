#include <stdio.h>
#include <stdlib.h>

#include "line_chunk.h"

#ifndef LINE_H
#define LINE_H

typedef struct {
    struct LineChunk* line_chunks_head;
    struct LineChunk* curr_line_chunk;

    struct Line* next_line;
} Line;

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

void set_line_chunks_head(Line* line) {
    if (line != NULL && line->line_chunks_head == NULL) {
        line->line_chunks_head = line->curr_line_chunk;
    }
}

void append_line_chunk_and_advance(Line** line_ptr, char* line_chunk_content) {
    if (line_ptr != NULL && *line_ptr != NULL) {
        (*line_ptr)->curr_line_chunk = (struct LineChunk*) append_line_chunk((LineChunk*) (*line_ptr)->curr_line_chunk, line_chunk_content);
    }
}

void print_line(FILE* output_file, Line* line, int spacing, char spacing_char) {
    if (output_file == NULL || line == NULL || spacing == 0 || spacing_char == '\0') {
        return;
    }

    LineChunk* curr_line_chunk = (LineChunk*) line->line_chunks_head;

    while (curr_line_chunk != NULL) {
        print_line_chunk(output_file, curr_line_chunk);

        LineChunk* next_line_chunk = (LineChunk*) curr_line_chunk->next_line_chunk;

        if (next_line_chunk != NULL) {
            for (int i = 0; i < spacing; i++) {
                fprintf(output_file, "%c", spacing_char);
            }
        }

        curr_line_chunk = next_line_chunk;
    }

    fprintf(output_file, "\n");
}

void free_lines(Line* line) {
    if (line == NULL) {
        return;
    }

    free_lines((Line*) line->next_line);

    free_line_chunks((LineChunk*) line->line_chunks_head);

    free(line);
}

#endif