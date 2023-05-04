#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "line.h"
#include "string_tools.h"

#define PAGE_SUCCESS 4
#define TRUNCATED_HANDLING_SUCCESS 3
#define ENDED_PARAGRAPH 2
#define ENDED_TEXT 1
#define NOT_ENDED_TEXT 0
#define INSUFFICIENT_WIDTH -1
#define ALLOC_ERROR -2
#define INVALID_INPUT -3

#ifndef PAGE_H
#define PAGE_H

/*
    Struttura dati che rappresenta le pagine dell'output del programma
    attraverso una lista puntata.
*/
typedef struct {
    struct Line* lines_head; // il puntatore alla testa della lista puntata delle righe della pagina
    struct Line* curr_line; // il puntatore alla riga corrente

    struct Page* next_page; // il puntatore alla prossima pagina
} Page;

/*
    La funzione crea una nuova pagina, utilizzando la riga in input;
    restituisce NULL se non è stato possibile allocare la nuova pagina.
*/
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

/*
    La funzione crea una nuova pagina, e aggiorna l'attributo 'next_page' della pagina
    fornita in input con la pagina appena creata; restituisce NULL se non è stato possibile
    allocare la nuova pagina.
*/
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

/*
    La funzione prende in input una pagina, e ne avanza il puntatore alla riga corrente, se possibile.
*/
void advance_curr_line(Page* page) {
    if (page == NULL) {
        return;
    }

    Line* curr_line = (Line*) page->curr_line;

    if (curr_line != NULL) {
        page->curr_line = curr_line->next_line;
    }
}

/*
    La funzione prende in input una pagina, e ne setta il puntatore alla testa
    della lista puntata delle righe, con il puntatore della riga corrente, solamente
    se il puntatore alla testa non era NULL.
*/
void set_lines_head(Page* page) {
    if (page != NULL && page->lines_head == NULL && page->curr_line != NULL) {
        page->lines_head = page->curr_line;
    }
}

/*
    La funzione prende in input una pagina, e ne resetta il puntatore alla testa
    della lista puntata delle righe, con il puntatore della riga corrente.
*/
void reset_lines_head(Page* page) {
    if (page == NULL) {
        return;
    }

    page->curr_line = page->lines_head;
}

/*
    La funzione prende in input un Page**, ed un chunk di riga, ed effettua
    2 operazioni: viene creata una nuova riga alla pagina corrente, e viene avanzato
    il puntatore della riga corrente alla nuova riga appena aggiunta; restituisce
    NULL se non è stato possibile allocare la nuova riga, altrimenti restituisce la riga creata.
*/
Line* append_line_and_advance(Page** page_ptr, LineChunk* line_chunk) {
    if (page_ptr == NULL || *page_ptr == NULL) {
        return NULL;
    }

    Line* new_line = (Line*) append_line((Line*) (*page_ptr)->curr_line, line_chunk);

    if (new_line == NULL) {
        return NULL;
    }

    (*page_ptr)->curr_line = (struct Line*) new_line;

    return new_line;
}

/*
    La funzione stampa su 'output_file' le pagine contenute nella lista puntata fornita
    in input, ricorsivamente; non viene stampato nulla se il seperatore tra pagine è una stringa vuota,
    oppure il carattere di separazione tra le colonne è '\0'.
*/
void print_pages(FILE* output_file, Page* page, int spacing, char* pages_separator, char spacing_char) {
    if (output_file == NULL || page == NULL || !strcmp(pages_separator, "\0") || spacing_char == '\0') {
        return;
    }

    Line* curr_line = (Line*) page->lines_head;

    while (curr_line != NULL) {
        print_line(output_file, curr_line, spacing, spacing_char);

        curr_line = (Line*) curr_line->next_line;
    }

    // questo controllo assicura che non venga
    // stampato il separatore tra pagine anche
    // dopo l'ultima pagina della lista puntata
    if (page->next_page != NULL) {
        fprintf(output_file, "%s", pages_separator);

        print_pages(output_file, (Page*) page->next_page, spacing, pages_separator, spacing_char);
    }
}

/*
    La funzione prende in input un puntatore ad un file, un char** di un buffer di caratteri
    da riempire con il chunk che verrà letto, la larghezza della colonna (che potrebbe variare
    in base al chunk letto), il puntatore alla posizione corrente nel file
    (che potrebbe variare in base al chunk letto), e il puntatore al numero di caratteri unicode
    incontrati all'interno del chunk (che potrebbe variare in base al chunk letto). La funzione
    non effettua controlli sulla correttezza di 'w_col', 'base_idx' e 'unicode_offset'.
*/
int read_chunk(FILE* input_file, char** line_chunk_content, int* w_col, long* base_idx, int* unicode_offset) {
    if (input_file == NULL || line_chunk_content == NULL || *line_chunk_content == NULL) {
        return INVALID_INPUT;
    }

    // questa flag viene settata a true quando viene trovato
    // un carattere che faccia parte di una parola, per evitare
    // di considerare i caratteri che non sono parte di parola
    // all'inizio della riga, e rischiare di rimuovere possibile
    // spazio per altre parole all'interno della riga
    bool is_text_started = false;

    // questa variabile rappresenta il numero di caratteri incontrati
    // che sono stati ignorati: ad esempio, se si trovano 2 o più spazi di fila
    // all'interno del testo di partenza, è necessario prenderne in considerazione
    // solamente uno, poiché altrimenti si leverebbe spazio utile per possibili
    // parole all'interno della riga
    int invalid_offset = 0;

    int exit_code = NOT_ENDED_TEXT;

    // questo ciclo legge una riga di una colonna (che nel programma verrà chiamato 'chunk'),
    // ma il numero di byte da leggere cambia dinamicamente, in quanto ogni volta che
    // viene incontrato un carattere unicode, o che viene incontrato un carattere che deve essere ignorato 
    // (come ad esempio spazi multipli, tabulazione etc., si veda quanto discusso precedentemente)
    // il numero di byte da leggere viene incrementato di 1; nel caso di caratteri
    // unicode, questi vengono inseriti all'interno del buffer, e la dimensione di
    // quest'ultimo viene modificata dinamicamente, mentre nel caso di caratteri speciali
    // vengono semplicemente ignorati e non vengono inseriti nel buffer
    for (int j = 0; j < *w_col + invalid_offset + *unicode_offset; j++) {
        char curr_char = fgetc(input_file);

        long next_pos = ftell(input_file);

        char next_char = fgetc(input_file);

        // se il carattere corrente e il carattere successivo
        // all'interno del file sono entrambi '\n', allora la riga era
        // l'ultima di un paragrafo, e dunque è necessario smettere di leggere
        // dal file di input, e riempire il resto del buffer con spazi
        if (curr_char == '\n' && next_char == '\n') {
            pad_string(*line_chunk_content, j - invalid_offset, *w_col + *unicode_offset, ' ');

            // base_idx è il puntatore alla posizione con cui si è iniziato a leggere
            // il chunk corrente, e deve essere aggiornato oppurtunamente per non
            // ignorare i caratteri che non sono stati inseriti nel buffer, poiché al loro
            // posto vi sono stati inseriti degli spazi; si noti che la formula vede un + 2
            // al suo interno, poichè vanno saltati i due '\n' letti come indicatore
            // di fine paragrafo, altrimenti si andrebbe in loop infinito
            *base_idx += j - (*w_col + invalid_offset + *unicode_offset) + 2;

            // si noti che con questo algoritmo si presenta un caso particolare nel momento
            // in cui la riga dell'ultimo paragrafo termina proprio sull'ultimo carattere disponibile
            // sulla riga; infatti, in tal caso, la vera ultima riga del paragrafo è stata
            // letta al chunk precdente, e all'interno di questa iterazione è stato letto
            // un chunk contenente due '\n' al suo inizio; allora, poiché è stato rimpiazzato
            // ogni suo carattere con ' ', è sufficiente controllare che l'attuale primo carattere
            // del chunk non sia uno spazio, per gestire questo caso particolare
            exit_code = (*line_chunk_content)[0] != ' ' ? ENDED_PARAGRAPH : NOT_ENDED_TEXT;
            
            break;
        }
        
        if (curr_char != EOF) {
            // questo controllo serve a rendere true la flag 'is_text_started' accuratamente
            // controllando le possibili combinazioni di caratteri da ignorare
            if ((!is_text_started && !is_char(curr_char)) || (is_text_started && !is_char(curr_char) && !is_char(next_char))) {
                invalid_offset++;

                // per ogni carattere ignorato, è necessario spostare in avanti la posizione
                // corrente all'interno del file
                *base_idx += 1;
            } else {
                is_text_started = true;

                (*line_chunk_content)[j - invalid_offset] = curr_char;

                if (is_utf8((unsigned char) curr_char)) {
                    *unicode_offset += 1;

                    // il buffer deve essere aumentato di 1 byte, per ogni carattere
                    // unicode incontrato all'interno del chunk
                    char* larger_chunk = realloc(*line_chunk_content, *w_col + *unicode_offset + 1);

                    if (larger_chunk == NULL) {
                        return ALLOC_ERROR;
                    }

                    larger_chunk[*w_col + *unicode_offset] = '\0';

                    *line_chunk_content = larger_chunk; // realloc non garantisce che il puntatore sia lo stesso
                }
            }
        } else {
            exit_code = ENDED_TEXT;
        }

        fseek(input_file, next_pos, SEEK_SET);
    }



    long curr_pos = ftell(input_file);
    char last_char = fgetc(input_file);

    if (is_utf8((unsigned char) last_char)) {

                // printf("random op");
    (*line_chunk_content)[*w_col + *unicode_offset] = last_char;
                    *unicode_offset += 1;

                    // il buffer deve essere aumentato di 1 byte, per ogni carattere
                    // unicode incontrato all'interno del chunk
                    char* larger_chunk = realloc(*line_chunk_content, *w_col + *unicode_offset + 1);

                    if (larger_chunk == NULL) {
                        return ALLOC_ERROR;
                    }

                    larger_chunk[*w_col + *unicode_offset] = '\0';

                    *line_chunk_content = larger_chunk; // realloc non garantisce che il puntatore sia lo stesso
    } else {
        fseek(input_file, curr_pos, SEEK_SET);
    }

    // alla larghezza della colonna corrente viene aggiunto il numero di
    // caratteri unicode che sono stati incontrati, per poter effettuare
    // le prossime operazioni correttamente
    *w_col += *unicode_offset;

    return exit_code;
}

/*
    La funzione gestisce il caso in cui il chunk corrente contiene una parola troncata
    al termine della riga, e in tal caso viene modificata la posizione all'interno del file;
    la funzione prende in ingresso anche il puntatore alla larghezza della colonna, che potrebbe
    però subire variazioni in base alla dimensione del buffer ed al tipo di caratteri che contiene.
    La funzione non effettua controlli sulla correttezza di 'w_col' e 'base_idx'.
*/
int handle_truncated_string(FILE* input_file, char** line_chunk_content, int* w_col, long* base_idx) {
    if (input_file == NULL || line_chunk_content == NULL || *line_chunk_content == NULL) {
        return INVALID_INPUT;
    }

    fseek(input_file, *base_idx + *w_col, SEEK_SET);

    char fchar_next_line = fgetc(input_file); // primo carattere del chunk successivo

    fseek(input_file, *base_idx, SEEK_SET);

    if (check_truncated_end(*line_chunk_content, *w_col, fchar_next_line)) {
        // se la parola alla fine del chunk corrente è troncata, ma la riga stessa non contiene spazi,
        // allora l'unica possibilità è che la riga contenga 1 sola parola, e poiché è troncata, segue
        // che la larghezza della colonna non è sufficiente a contenere la dimensione della parola
        // corrente; allora, è necessario restituire un errore poiché non possibile procedere
        if (no_spaces(*line_chunk_content, *w_col)) { 
            return INSUFFICIENT_WIDTH;
        }

        // se il chunk contiene alla sua fine una parola troncata, poiché la parola
        // deve essere spostata alla riga successiva, la posizione sul file originale viene
        // spostata all'indietro di 1 per ogni carattere che è stato rimpiazzato al termine della riga
        *base_idx -= replace_truncated_chars(*line_chunk_content, w_col, ' ');

        // se la riga, alla sua fine, conteneva una parola contenente caratteri unicode, poichè
        // questa è ora stata "spostata" alla riga successiva (decrementando la posizione sul file),
        // è necessario troncare la dimensione della stringa poiché questa era stata modificata
        // in base al numero di caratteri unicode che la riga conteneva
        if (truncate_string(line_chunk_content, *w_col) == TRUNCATION_ERROR) {
            return ALLOC_ERROR;
        }
    }

    return TRUNCATED_HANDLING_SUCCESS;
}

// TODO: descrivi la struttura dati
int build_pages(FILE* input_file, Page* curr_page, int cols, int h_col, int w_col) {
    if (curr_page == NULL) {
        return ALLOC_ERROR;
    }

    Page* first_page = curr_page; // backup per non perdere la testa della lista puntata

    int chunks_counter = 0; // il numero di chunk salvati
    int col_counter = 0; // il numero di colonne salvate

    // poiché la pagina corrente contiene una lista puntata per le sue righe,
    // ma l'algoritmo deve posizionare i chunk all'interno di queste "orizzontalmente",
    // è necessaria una flag che indica se bisogna ancora allocare nuove righe alla pagina,
    // poiché il numero di righe richieste non è ancora stato raggiunto, oppure in caso contrario,
    // inizare ad inserire i chunk nelle righe che già sono presenti all'interno della pagina
    bool h_col_reached = false;

    // questa flag viene attivata se è arrivata la fine di un paragrafo, ed è dunque necessario
    // inserire una riga completamente vuota nella pagina
    bool is_new_par = false;

    while (!feof(input_file)) {
        long curr_pos = ftell(input_file);

        char* line_chunk_content = calloc(w_col + 1, sizeof(char));

        // il numero di caratteri unicode che verranno incontrati all'interno
        // del chunk che verrà letto
        int unicode_offset = 0;

        int end_value = NOT_ENDED_TEXT;

        if (!is_new_par) {
            end_value = read_chunk(input_file, &line_chunk_content, &w_col, &curr_pos, &unicode_offset);

            if (end_value == ALLOC_ERROR) {
                free(line_chunk_content);

                // il puntatore della pagina viene comunque aggiornato, poichè sino al momento dell'errore,
                // le pagine sono comunque state accomulate correttamente, ed è comunque possibile utilizzarle
                curr_page = first_page;

                return ALLOC_ERROR;
            } else if (end_value == INVALID_INPUT) {
                // in questo branch, qualcosa è andato storto nel programma, e potrebbe essersi verificato che 
                // l'area del chunk corrente non sia valida, e dunque liberarne la memoria potrebbe
                // portare ad un double free
                return INVALID_INPUT;
            }

            int w_col_backup = w_col;

            int trunc_err = handle_truncated_string(input_file, &line_chunk_content, &w_col, &curr_pos);

            if (trunc_err != TRUNCATED_HANDLING_SUCCESS) {
                if (trunc_err != INVALID_INPUT) {
                    free(line_chunk_content); // TODO: debuggare anche questo E CONTROLLARE CHE NON SERVA ALTROVE (PROBABILMENTE SERVE SOTTO MA NON LO SO)
                }
                
                curr_page = first_page;

                return trunc_err;
            }

            // vengono rimpiazzati caratteri eventuali '\n' e '\t', poiché altrimenti potrebbero
            // disallineare le colonne in output
            string_replace(line_chunk_content, '\n', ' ');
            string_replace(line_chunk_content, '\t', ' ');

            if (end_value != ENDED_PARAGRAPH) {
                // se la riga corrente non è l'ultima di un paragrafo, allora deve essere giustificata
                justify_string(line_chunk_content, w_col);
            } else {
                // la flag viene settata per la prossima iterazione del ciclo, poiché la riga corrente è l'ultima
                // del paragrafo, e quindi la prossima dovrà essere una riga completamente vuota
                is_new_par = true;
            }

            w_col = w_col_backup; // TODO: PERCHE SERVIVA FA STA COSA?!?!?!
        } else {
            is_new_par = false;

            pad_string(line_chunk_content, 0, w_col, ' ');

            curr_pos -= w_col;
        }

        if (!h_col_reached) {
            if (append_line_and_advance(&curr_page, NULL) == NULL) {
                return ALLOC_ERROR;
            }

            set_lines_head(curr_page);
        }

        Line* curr_line = (Line*) curr_page->curr_line;

        append_line_chunk_and_advance(&curr_line, line_chunk_content);

        set_line_chunks_head(curr_line);

        if (end_value == ENDED_TEXT) {
            break;
        }

        if (h_col_reached) {
            advance_curr_line(curr_page);
        }

        chunks_counter++;

        if (chunks_counter % h_col == 0) {
            h_col_reached = true;

            col_counter++;

            reset_lines_head(curr_page); 
        }

        if (col_counter == cols) {
            Page* new_page = append_page(curr_page, NULL);

            if (new_page == NULL) {
                return ALLOC_ERROR;
            }

            curr_page = new_page;

            h_col_reached = false;

            col_counter = 0;
        }

        fseek(input_file, curr_pos + w_col, SEEK_SET);

        w_col -= unicode_offset;
    }

    curr_page = first_page;

    return PAGE_SUCCESS;
}

// TODO: TESTALA PRIMA
void free_pages(Page* page) {
    if (page == NULL) {
        return;
    }

    free_pages((Page*) page->next_page);

    free_lines((Line*) page->lines_head);

    free(page);
}

#endif