#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "line.h"
#include "string_tools.h"

#define NOT_ENDED_TEXT 0
#define ENDED_TEXT 1
#define ENDED_PARAGRAPH 2

#define PAGE_SUCCESS -1
#define INSUFFICIENT_WIDTH -2
#define REALLOC_ERROR -3
#define TRUNCATED_HANDLING_SUCCESS -4

#ifndef PAGE_H
#define PAGE_H

typedef struct {
    struct Line* lines_head;
    struct Line* curr_line;

    struct Page* next_page;
} Page;

Page* new_page(Line* line) {
    Page* new_page = calloc(1, sizeof(Page));

    if (new_page == NULL) {
        return NULL;
    }

    new_page->lines_head = (struct Line*) line;
    new_page->curr_line = (struct Line*) line;

    new_page->next_page = NULL;

    return new_page;
}

Page* append_page(Page* curr_page, Line* line) {
    Page* next_page = new_page(line);

    if (next_page == NULL) {
        return NULL;
    }

    if (curr_page != NULL) {
        curr_page->next_page = (struct Page*) next_page;
    }

    return next_page;
}

void advance_curr_line(Page* page) {
    if (page == NULL) {
        return;
    }

    Line* curr_line = (Line*) page->curr_line;

    page->curr_line = curr_line->next_line;
}

void set_lines_head(Page* page) {
    if (page != NULL && page->lines_head == NULL) {
        page->lines_head = page->curr_line;
    }
}

void reset_lines_head(Page* page) {
    if (page == NULL) {
        return;
    }

    page->curr_line = page->lines_head;
}

void append_line_to_page(Page** page_ptr, LineChunk* line_chunk) {
    if (page_ptr != NULL && *page_ptr != NULL) {
        (*page_ptr)->curr_line = (struct Line*) append_line((Line*) (*page_ptr)->curr_line, line_chunk);
    }
}

void print_pages(FILE* output_file, Page* page, int spacing, char* pages_separator, char spacing_char) {
    if (page == NULL) { // TODO: testa che succede con pages_separator a stringa vuota oppure spacing_char == '\0'
        return;
    }

    Line* curr_line = (Line*) page->lines_head;

    while (curr_line != NULL) {
        print_line(output_file, curr_line, spacing, spacing_char);

        curr_line = (Line*) curr_line->next_line;
    }

    if (page->next_page != NULL) {
        fprintf(output_file, "%s", pages_separator);

        print_pages(output_file, (Page*) page->next_page, spacing, pages_separator, spacing_char);
    }
}

int read_chunk(FILE* input_file, char* line_chunk_content, int w_col, long* base_idx, int* unicode_offset) {
    bool is_text_started = false;

    int invalid_offset = 0;

    for (int j = 0; j < w_col + invalid_offset + *unicode_offset; j++) {
        char curr_char = fgetc(input_file);

        long next_pos = ftell(input_file);

        char next_char = fgetc(input_file);

        if (curr_char == '\n' && next_char == '\n') {
            pad_string(line_chunk_content, j - invalid_offset, w_col + *unicode_offset, ' ');

            *base_idx += j - (w_col + invalid_offset + *unicode_offset) + 2;

            return line_chunk_content[0] != ' ' ? ENDED_PARAGRAPH : NOT_ENDED_TEXT;
        }
        
        if (curr_char != EOF) {
            if ((!is_text_started && !is_char(curr_char)) || (is_text_started && !is_char(curr_char) && !is_char(next_char))) {
                invalid_offset++;
                *base_idx += 1;
            } else {
                is_text_started = true;

                line_chunk_content[j - invalid_offset] = curr_char; // j - i - (finish - i - w_col)

                if (is_utf8((unsigned char) curr_char)) {
                    *unicode_offset += 1;

                    char* larger_chunk = realloc(line_chunk_content, w_col + *unicode_offset + 1);

                    if (larger_chunk == NULL) {
                        return REALLOC_ERROR; // TODO: HANDLE THIS
                    }

                    larger_chunk[w_col + *unicode_offset] = '\0';

                    line_chunk_content = larger_chunk;
                }
            }
        } else {
            return ENDED_TEXT;
        }

        fseek(input_file, next_pos, SEEK_SET);
    }

    return NOT_ENDED_TEXT;
}

int handle_truncated_string(FILE* input_file, char* line_chunk_content, int* w_col, long* curr_pos) {
    fseek(input_file, *curr_pos + *w_col, SEEK_SET);

    char fchar_next_line = fgetc(input_file);

    fseek(input_file, *curr_pos, SEEK_SET);

    if (check_truncated_end(line_chunk_content, *w_col, fchar_next_line)) {
        if (no_spaces(line_chunk_content, *w_col)) { 
            return INSUFFICIENT_WIDTH;
        }

        *curr_pos -= replace_truncated_chars(line_chunk_content, w_col);

        line_chunk_content = truncate_string(line_chunk_content, *w_col);

        if (line_chunk_content == NULL) {
            return REALLOC_ERROR;
        }
    }

    return TRUNCATED_HANDLING_SUCCESS;
}

int build_pages(FILE* input_file, Page* curr_page, int cols, int h_col, int w_col) {
    Page* first_page = curr_page; // backup

    int line_counter = 0;
    int col_counter = 0;

    bool h_col_reached = false;
    bool is_new_par = false;

    while (!feof(input_file)) {
        long curr_pos = ftell(input_file);

        char* line_chunk_content = calloc(w_col + 1, sizeof(char));

        int end_value = NOT_ENDED_TEXT;
        int unicode_offset = 0;

        if (!is_new_par) {
            end_value = read_chunk(input_file, line_chunk_content, w_col, &curr_pos, &unicode_offset);

            w_col += unicode_offset;

            int w_col_backup = w_col;

            int trunc_err = handle_truncated_string(input_file, line_chunk_content, &w_col, &curr_pos);

            if (trunc_err != TRUNCATED_HANDLING_SUCCESS) {
                free(line_chunk_content); // TODO: debuggare anche questo
                
                curr_page = first_page;

                return trunc_err;
            }

            string_replace(line_chunk_content, '\n', ' '); // TODO: attenzione che vanno ignorati anche altri caratteri strani tipo \t e non so se c'è altro

            if (end_value != ENDED_PARAGRAPH) {
                justify_string(line_chunk_content, w_col);
            } else {
                is_new_par = true;
            }

            w_col = w_col_backup;
        } else {
            is_new_par = false;

            pad_string(line_chunk_content, 0, w_col, ' ');

            curr_pos -= w_col;
        }

        if (!h_col_reached) {
            append_line_to_page(&curr_page, NULL);

            set_lines_head(curr_page);
        }

        Line* curr_line = (Line*) curr_page->curr_line;

        append_line_chunk_to_line(&curr_line, line_chunk_content);

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
            curr_page = append_page(curr_page, NULL);

            h_col_reached = false;

            col_counter = 0;
        }

        fseek(input_file, curr_pos + w_col, SEEK_SET);

        w_col -= unicode_offset;
    }

    curr_page = first_page;

    return PAGE_SUCCESS;
}

void free_pages(Page* page) {
    if (page == NULL) {
        return;
    }

    free_pages((Page*) page->next_page);

    free_lines((Line*) page->lines_head);

    free(page);
}

#endif