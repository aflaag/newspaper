#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define TRUNCATION_NOT_PERFORMED 2
#define TRUNCATION_SUCCESS 1
#define ATOI_SUCCESS 0
#define ATOI_INVALID_CHAR -1
#define TRUNCATION_ERROR -2

#ifndef STRING_TOOLS_H
#define STRING_TOOLS_H

void string_replace(char* string, char target, char replacement) {
    if (string == NULL) {
        return;
    }

    int len = strlen(string);

    for (int i = 0; i < len; i++) {
        if (string[i] == target) {
            string[i] = replacement;
        }
    }
}

bool is_char(char character) {
    return character != ' ' && character != '\n' && character != '\0' && character != EOF && character != '\t';
    // return character > 32;
}

void pad_string(char* string, int start, int len, char character) {
    for (int i = start; i < len; i++) {
        string[i] = character;
    }
}

bool is_utf8(unsigned char character) {
    return character >> 6 == 0b10;
}

/*
    La funzione restituisce true, se il chunk fornito in input contiene una parola
    troncata alla fine.
*/

bool check_truncated_end(char* line_chunk_content, int w_col, char next_char) {
    return is_char(line_chunk_content[w_col - 1]) && (is_char(next_char) || is_utf8(next_char));
}

/*
    La funzione prende in input una stringa, e la sua nuova dimensione, e se possibile,
    aggiorna il puntatore char** di input con l'indirizzo alla nuova stringa di dimensione fornita;
    si noti che la funzione tronca, e ritorna codici di errore nel caso in cui non è
    stato possibile effettuare il troncamento; la funzione, in caso di successo, garantisce
    che la stringa fornita in input venga deallocata, e ne viene riallocata una nuova.
*/
int truncate_string(char** string, int new_size) {
    int original_size = strlen(*string);

    // se la nuova dimensione è maggiore o uguale alla dimensione originale,
    // il troncamento non deve essere effettuato
    if (original_size <= new_size) {
        return TRUNCATION_NOT_PERFORMED;
    }

    char* truncated_string = calloc(new_size + 1, sizeof(char));

    if (truncated_string == NULL) {
        return TRUNCATION_ERROR;
    }

    strncpy(truncated_string, *string, new_size);

    free(*string);

    *string = truncated_string;
    
    return TRUNCATION_SUCCESS;
}

/*
    La funzione prende in input una stringa, la sua larghezza, e un carattere
    di rimpiazzo, e rimpiazza con quest'ultimo tutta l'ultima parola della stringa fornita.
*/
long replace_truncated_chars(char* line_chunk_content, int* w_col, char replacement) {
    long truncated_chars = 0;

    // il ciclo parte dalla fine della riga e procede fino a trovare un carattere
    // che sia parte di una parola
    for (int i = *w_col - 1; is_char(line_chunk_content[i]) && i >= 0; i--) {
        // poiché la dimensione della riga è stata modificata, aggiungendo 1 byte per ogni
        // carattere unicode che essa contiene, è necessario decrementare nuovamente
        // la sua dimensione, per ogni carattere unicode incontrato all'interno dell'ultima
        // parola della riga, affinché la dimensione di questa sia coerente
        if (is_utf8(line_chunk_content[i])) {
            *w_col -= 1;
        }

        line_chunk_content[i] = replacement;

        truncated_chars++;
    }

    return truncated_chars;
}

bool no_spaces(char* string, int len) {
    for (int i = 0; i < len; i++) {
        if (string[i] == ' ') {
            return false;
        }
    }

    return true;
}

int round_division(int x, int y) {
    return (x + (y / 2)) / y;
}

int count_words(char* string, int len) {
    bool in_word = false;

    int words = 0;

    for (int i = 0; i < len; i++) {
        char curr_char = string[i];
        bool is_space = is_char(curr_char);

        if (is_space && in_word) {
            words++;

            in_word = false;
        } else if (!is_space && !in_word) {
            in_word = true;
        }
    }

    if (in_word) {
        words++;
    }

    return words;
}

int min(int x, int y) {
    return (x < y) ? x : y;
}

int evaluate_curr_spaces(int spaces_end, int spaces_inside, int spaces_done, int ratio) {
    return ratio + 1 == spaces_end && spaces_done + 1 == spaces_inside ? ratio + 1 : min(spaces_end, ratio);
}

void enqueue(char queue[], int len, int* head, int* tail, char element) {
    if (*tail >= 0 && *tail < len - 1) {
        queue[*tail] = element;

        *tail += 1;
    }
}

char dequeue(char queue[], int len, int* head, int* tail) {
    if (*head < *tail) { // TODO: avrebbe comunque senso mettere i controlli anche qui though
        char element = queue[*head];

        queue[*head] = '\0';

        *head += 1;

        return element;
    } else {
        return '\0';
    }
}

void slide_characters(char* string, int len, int spaces_end, int spaces_inside, int ratio) {
    int head = 0;
    int tail = 0;

    char queue[len];

    pad_string(queue, 0, len, '\0');

    int spaces_done = 0;

    for (int i = 1; i < len; i++) {
        char DEBUG = string[i];
        if (tail != 0) {
            if ((is_char(string[i]) && !is_char(queue[tail - 1])) || !is_char(queue[head])) {
                while (is_char(queue[head])) {
                    enqueue(queue, len, &head, &tail, string[i]);

                    string[i] = dequeue(queue, len, &head, &tail);

                    i++;
                }

                enqueue(queue, len, &head, &tail, string[i]);
                string[i] = dequeue(queue, len, &head, &tail);
                i++;
                
                int curr_spaces = evaluate_curr_spaces(spaces_end, spaces_inside, spaces_done, ratio);

                spaces_end -= curr_spaces;

                while (curr_spaces > 0) {
                    enqueue(queue, len, &head, &tail, string[i]);

                    string[i] = ' ';

                    curr_spaces--;

                    i++;
                }

                spaces_done++;

                i--;
            } else {
                enqueue(queue, len, &head, &tail, string[i]);

                string[i] = dequeue(queue, len, &head, &tail);
            }
        } else {
            if (is_char(string[i]) && !is_char(string[i - 1])) {
                int curr_spaces = evaluate_curr_spaces(spaces_end, spaces_inside, spaces_done, ratio);

                spaces_end -= curr_spaces;

                while (curr_spaces > 0) {
                    enqueue(queue, len, &head, &tail, string[i]);

                    string[i] = ' ';

                    curr_spaces--;

                    i++;
                }

                spaces_done++;

                i--;
            }
        }
    }
}

void justify_string(char* string, int len) {
    int spaces_end = 0;

    for (int i = len - 1; string[i] == ' ' && i >= 0; i--) {
        spaces_end++;
    }

    if (spaces_end == 0) {
        return;
    }

    int spaces_inside = count_words(string, len) - 1;

    // printf("%d ", spaces_inside);

    // TODO: ricordati di scrivere che è garantito
    // che gli space chunks abbiano esattamente 1 spazio
    if (spaces_inside == 0) { // una sola parola
        return;
    }

    int ratio = round_division(spaces_end, spaces_inside);

    if (ratio == 0) { // TODO: scrivere che questo è un fix che risolve quando il ratio fa 0
        ratio = 1;
    }

    slide_characters(string, len, spaces_end, spaces_inside, ratio);
}

/* Returns the power of a number. */
int powi(int n, int exp) {
    int tot = 1;

    for (int i = 0; i < exp; i++) {
        tot *= n;
    }

    return tot;
}

/* Returns the integer represented by the input string. */
int pos_atoi(char* str, int len, int* number) {
    int tot = 0;

    int exp = len - 1;

    for (int i = 0; i < len; i++) {
        int curr_char = str[i] - '0';
        
        if (curr_char < 0 || curr_char > 9) {
            return ATOI_INVALID_CHAR;
        }

        tot += powi(10, exp) * curr_char;

        exp--;
    }

    *number = tot;

    return ATOI_SUCCESS;
}

#endif