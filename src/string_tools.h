#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define JUSTIFICATION_SUCCESS 3
#define TRUNCATION_NOT_PERFORMED 2
#define TRUNCATION_SUCCESS 1
#define ATOI_SUCCESS 0
#define ATOI_INVALID_CHAR -1
#define TRUNCATION_ERROR -2
#define INVALID_INPUT -3

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

bool is_char(char character);
bool is_utf8(unsigned char character);
bool check_truncated_end(char* line_chunk_content, int w_col, char next_char);
bool no_spaces(char* string, int len);

void string_replace(char* string, char target, char replacement);
void pad_string(char* string, unsigned int start, unsigned int end, char character);

int truncate_string(char** string, int new_size);
long replace_truncated_chars(char* line_chunk_content, int* w_col, char replacement);

int round_division(unsigned int x, unsigned int y);
int count_words(char* string, int len);
int min(int x, int y);
int evaluate_curr_spaces(int spaces_end, int spaces_inside, int spaces_done, int ratio);

void enqueue(char queue[], int len, int* head, int* tail, char element);
char dequeue(char queue[], int len, int* head, int* tail);
void slide_characters(char* string, int len, int spaces_end, int spaces_inside, int ratio, int remainder);
void justify_string(char* string, int len);

int powi(int n, unsigned int exp);
int pos_atoi(char* str, int len, unsigned int* number);

#endif