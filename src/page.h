#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "line.h"
#include "string_tools.h"

#define ENDED_TEXT 0
#define NOT_ENDED_TEXT 1
#define ENDED_PARAGRAPH 2
#define FLINE_NEXT_PAR 3

#ifndef PAGE_H
#define PAGE_H

typedef struct {
    struct Line* lines_head;
    struct Line* curr_line;

    struct Page* next_page;
} Page;

Page* new_page(Line* line) {
    Page* new_page;
    new_page = calloc(1, sizeof(Page));

    if (new_page == NULL) {
        return NULL;
    }

    new_page->lines_head = (struct Line*) line;
    new_page->curr_line = (struct Line*) line;

    new_page->next_page = NULL;

    return new_page;
}

Page* append_page(Page* curr_page, int cols, int h_col, int w_col, Line* line) {
    Page* next_page;
    next_page = new_page(line);

    if (next_page == NULL) {
        return NULL;
    }

    if (curr_page == NULL) {
        curr_page = next_page;
    } else {
        curr_page->next_page = (struct Page*) next_page;
    }

    return next_page;
}

void advance_curr_line(Page* page) {
    Line* curr_line = (Line*) page->curr_line;

    page->curr_line = curr_line->next_line;
}

void set_lines_head(Page* page) {
    if (page->lines_head == NULL) {
        page->lines_head = page->curr_line;
    }
}

void reset_lines_head(Page* page) {
    page->curr_line = page->lines_head;
}

void print_pages(Page* page, int spacing, char* pages_separator, char spacing_char) {
    if (page == NULL) {
        return;
    }

    Line* curr_line = (Line*) page->lines_head;

    while (curr_line != NULL) {
        print_line(curr_line, spacing, spacing_char);

        curr_line = (Line*) curr_line->next_line;
    }

    if (page->next_page != NULL) {
        printf("%s", pages_separator);

        print_pages((Page*) page->next_page, spacing, pages_separator, spacing_char);
    }
}

int read_chunk(char* input_content, char* line_chunk_content, int w_col, int* base_idx) {
    bool is_text_started = false;

    int finish = *base_idx + w_col;

    for (int j = *base_idx; j < finish; j++) {
        char curr_char = input_content[j];
        char next_char = input_content[j + 1];

        if (curr_char == '\n' && next_char == '\n') {
            pad_string(line_chunk_content, j - finish + w_col, w_col, ' ');

            *base_idx += j - finish + 2;

            return line_chunk_content[0] != ' ' ? ENDED_PARAGRAPH : NOT_ENDED_TEXT;
        }
        
        // TODO: provare a vedere se il tutto funziona lasciando spazi multipli dentro una riga, sempre che io lo voglia fare
        if (curr_char != '\0') {
            if ((!is_text_started && !is_char(curr_char)) || (is_text_started && !is_char(curr_char) && !is_char(next_char))) {
                finish++;
                *base_idx += 1;
            } else {
                is_text_started = true;

                line_chunk_content[j - finish + w_col] = curr_char; // j - i - (finish - i - w_col)
            }
        } else {
            return ENDED_TEXT;
        }
    }

    return NOT_ENDED_TEXT;
}

// TODO: dovrebbe prendere un puntatore a cui collegare le pagine finite
// restituendo un intero per fare un po di error code dal chiamante
Page* build_pages(char* input_content, int cols, int h_col, int w_col) {
    Page* curr_page = new_page(NULL);
    Page* first_page = curr_page; // backup

    int line_counter = 0;
    bool h_col_reached = false;

    int col_counter = 0;

    bool is_new_par = false;

    for (int i = 0; input_content[i] != '\0'; i += w_col) {
        char* line_chunk_content = calloc(w_col + 1, sizeof(char));

        int end_value = NOT_ENDED_TEXT;

        if (!is_new_par) {
            end_value = read_chunk(input_content, line_chunk_content, w_col, &i);
        } else {
            is_new_par = false;

            pad_string(line_chunk_content, 0, w_col, ' ');

            i -= w_col;
        }

        printf("%s\n", line_chunk_content);

        if (check_truncated_end(line_chunk_content, w_col, input_content[i + w_col])) {
            if (no_spaces(line_chunk_content, w_col)) {
                return curr_page; // TODO: free vari, inoltre questo dovrebbe essere NULL probabilmente
            }

            i -= replace_truncated_chars(line_chunk_content, w_col);
        }

        string_replace(line_chunk_content, '\n', ' ');

        if (end_value != ENDED_PARAGRAPH) {
            justify_string(line_chunk_content, w_col);
        } else {
            is_new_par = true;
        }

        if (!h_col_reached) {
            curr_page->curr_line = (struct Line*) append_line((Line*) curr_page->curr_line, NULL);

            set_lines_head(curr_page);
        }

        Line* curr_line = (Line*) curr_page->curr_line;

        curr_line->curr_line_chunk = (struct LineChunk*) append_line_chunk((LineChunk*) curr_line->curr_line_chunk, line_chunk_content);

        set_line_chunks_head(curr_line);

        if (end_value == ENDED_TEXT) {
            break;
        }

        if (h_col_reached) {
            advance_curr_line(curr_page);
        }

        line_counter++;

        if (line_counter == h_col) {
            line_counter = 0;
            h_col_reached = true;

            col_counter++;

            reset_lines_head(curr_page); 
        }

        if (col_counter == cols) {
            curr_page = append_page(curr_page, cols, h_col, w_col, NULL);

            h_col_reached = false;

            col_counter = 0;
        }
    }

    return first_page;
}

#endif