#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define JUSTIFICATION_SUCCESS 3 // restituita se non si sono verificati errori durante la giustificazione del chunk
#define TRUNCATION_NOT_PERFORMED 2 // restituita se non è stato necessario troncare la stringa fornita in input
#define TRUNCATION_SUCCESS 1 // restituita se la stringa fornita è stata troncata con successo
#define ATOI_SUCCESS 0 // restituita se non si sono verificati errori durante il parsing da stringa a intero
#define ATOI_INVALID_CHAR -1 // restituita se si sono verificati errori durante il parsing da stringa a intero
#define TRUNCATION_ERROR -2 // restituita se si sono verificati errori durante il troncamento della stringa fornita in input
#define INVALID_INPUT -3 // restituita se si sono verificati degli errori nel fornire inpu ad alcune funzioni

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

bool is_char(char character);
bool is_utf8(unsigned char character);
bool check_truncated_end(char* line_chunk_content, int w_col, char next_char);
bool no_spaces(char* string, int len);
bool is_empty(char* string, int len);

void string_replace(char* string, char target, char replacement);
void pad_string(char* string, unsigned int start, unsigned int end, char character);

int truncate_string(char** string, int new_size);
long replace_truncated_chars(char* line_chunk_content, int* w_col, char replacement);

int count_words(char* string, int len);

bool valid_queue_inputs(int head, int tails, int len);
void enqueue(char queue[], int len, int* head, int* tail, char element);
char dequeue(char queue[], int len, int* head, int* tail);
void put_spaces_and_enqueue(int ratio, int* remainder, char* string, char* queue, int len, int* head, int* tail, int* i);
void slide_characters(char* string, int len, int spaces_end, int spaces_inside);
void justify_string(char* string, int len);

char* string_realloc(char* string, int new_size);

int powi(int n, unsigned int exp);
int pos_atoi(char* str, int len, unsigned int* number);

#endif