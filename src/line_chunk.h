#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef LINE_CHUNK_H
#define LINE_CHUNK_H

typedef struct {
    char* content;

    struct LineChunk* next_line_chunk;
} LineChunk;

LineChunk* new_line_chunk(char* content) {
    LineChunk* new_line_chunk;
    new_line_chunk = calloc(1, sizeof(LineChunk));

    if (new_line_chunk == NULL) {
        return NULL;
    }

    new_line_chunk->content = content;
    new_line_chunk->next_line_chunk = NULL;

    return new_line_chunk;
}

LineChunk* append_line_chunk(LineChunk* curr_line_chunk, char* content) {
    LineChunk* next_line_chunk = new_line_chunk(content);

    if (next_line_chunk == NULL) {
        return NULL;
    }

    if (curr_line_chunk == NULL) {
        curr_line_chunk = next_line_chunk;
    } else {
        curr_line_chunk->next_line_chunk = (struct LineChunk*) next_line_chunk;
    }

    return next_line_chunk;
}

void print_line_chunk(LineChunk* line_chunk) {
    if (line_chunk == NULL) {
        return;
    }

    printf("%s", line_chunk->content);
}

#endif