#include "page_par.h"

/*
    La funzione prende in input una pipe, il puntatore al file di input, il numero di colonne di una pagina,
    il numero di righe di una pagina, e la larghezza di ogni colonna di una pagina, e legge dal file di input
    i chunk di colonne che verranno inviati al processo di creazione della struttura dati.

    I commenti in questa funzione sono ridotti al minimo, poiché sarebbero una sola ripetizione dei commenti
    che sono presenti nella funzione della versione monoprocesso.
*/
int read_input_file_par(int* pipefd_rs, FILE* input_file, int cols, int h_col, int w_col) {
    if (pipefd_rs == NULL || input_file == NULL) {
        return INVALID_INPUT;
    }

    int chunks_counter = 0;
    int col_counter = 0;

    bool is_new_par = false;

    while (!feof(input_file)) {
        long curr_pos = ftell(input_file);

        char* line_chunk_content = calloc(w_col + 1, sizeof(char));

        if (line_chunk_content == NULL) {
            return ALLOC_ERROR;
        }

        int unicode_offset = 0;
        int end_value = NOT_ENDED_TEXT;

        if (!is_new_par) {
            end_value = read_chunk(input_file, &line_chunk_content, &w_col, &curr_pos, &unicode_offset, col_counter == 0 && chunks_counter == 0);

            if (end_value == ALLOC_ERROR || end_value == FSEEK_ERROR) {
                free(line_chunk_content);
                return end_value;
            } else if (end_value == INVALID_INPUT) {
                return INVALID_INPUT;
            }

            int w_col_backup = w_col;

            int trunc_err = TRUNCATED_HANDLING_SUCCESS;
            
            if (end_value != ENDED_TEXT) {
                trunc_err = handle_truncated_string(input_file, &line_chunk_content, &w_col, &curr_pos);
            }

            if (trunc_err != TRUNCATED_HANDLING_SUCCESS) {
                if (trunc_err != INVALID_INPUT) {
                    free(line_chunk_content);
                }
                
                return trunc_err;
            }

            string_replace(line_chunk_content, '\n', ' ');
            string_replace(line_chunk_content, '\t', ' ');
            string_replace(line_chunk_content, '\r', ' ');

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

        if (is_empty(line_chunk_content, w_col)) {
            free(line_chunk_content);
            break;
        }

        int len = strlen(line_chunk_content) + 1;

        // nella pipe viene prima inserita la lunghezza del prossimo chunk, poi il chuhk stesso,
        // ed infine il valore di 'end_value', per far sapere se la lettura del file è terminata
        if (write(pipefd_rs[1], &len, sizeof(int)) == -1 ||
            write(pipefd_rs[1], line_chunk_content, len) == -1 ||
            write(pipefd_rs[1], &end_value, sizeof(int)) == -1) {
            free(line_chunk_content);
            return WRITE_ERROR;
        }

        free(line_chunk_content);

        if (end_value == ENDED_TEXT) {
            break;
        }

        chunks_counter++;

        if (chunks_counter % h_col == 0) {
            col_counter++;
        }

        if (col_counter == cols) {
            col_counter = 0;
        }

        if (fseek(input_file, curr_pos + w_col, SEEK_SET)) {
            return FSEEK_ERROR;
        }

        w_col -= unicode_offset;
    }

    return PAGE_SUCCESS;
}

/*
    La funzione prende in input il puntatore alla pagina corrente, tutte le informazioni che definiscono
    la struttura di una pagina, ed il carattere di separazione tra le colonne di pagina,
    e manda la pagina fornita in input, riga per riga, al processo di scrittura; le righe mandate
    contengono anche gli spazi tra le colonne costruite, ma non contengono il \n alla fine.
*/
int send_page(Page* curr_page, int* pipefd_sw, int cols, int w_col, int spacing, char spacing_char) {
    if (pipefd_sw == NULL || curr_page == NULL || spacing_char == '\0' || cols == 0 || w_col == 0 || spacing == 0) {
        return INVALID_INPUT;
    }

    Line* line = (Line*) curr_page->lines_head;

    // per ognuna delle righe della pagina
    while (line != NULL) {
        LineChunk* line_chunk = (LineChunk*) line->line_chunks_head;

        // la dimensione massima che una riga può avere è la seguente, poiché un carattere multibyte UTF-8
        // non può essere composto da più di 4 byte, e dunque 4 * w_col * cols è il numero di caratteri non spazi,
        // mentre gli spazi saranno spacing * (cols - 1), e sviluppando i calcoli si ottiene la formula seguente;
        // si noti che è necessario aggiungere + 1 per '\0' alla fine (che non è necessario inserire usando calloc)
        int len = (4 * w_col + spacing) * cols - spacing + 1;

        char* joined_line = calloc(len, sizeof(char));

        if (joined_line == NULL) {
            return ALLOC_ERROR;
        }

        int i = 0;

        while (true) {
            char* line_chunk_content = line_chunk->content;

            if (line_chunk_content == NULL) {
                free(joined_line);
                return ALLOC_ERROR;
            }
            
            // il contenuto del chunk corrente viene riposto nel buffer creato
            strcat(joined_line, line_chunk_content);

            i += strlen(line_chunk_content);

            if (line_chunk->next_line_chunk == NULL) {
                break;
            }

            // vengono aggiunti i caratteri di spaziatura tra le colonne
            for (int j = i; j < i + spacing; j++) {
                joined_line[j] = spacing_char;
            }

            i += spacing;

            line_chunk = (LineChunk*) line_chunk->next_line_chunk;
        }

        // l'attuale valore della lunghezza non considerava \0 alla fine della stringa,
        // ma è necessario contare anche quest'ultimo all'inteno della pipe
        len++;

        // attraverso la pipe, viene prima mandata la lunghezza della riga che sta per
        // essere inviata, e successivamente viene mandata la riga stessa
        if (write(pipefd_sw[1], &len, sizeof(int)) == -1 ||
            write(pipefd_sw[1], joined_line, len) == -1) {
            free(joined_line);
            return WRITE_ERROR;
        }

        free(joined_line);

        line = (Line*) line->next_line;
    }

    return PAGE_SUCCESS;
}

/*
    La funzione prende in input due pipe, tutte le informazioni che definiscono la struttura di una pagina,
    ed il carattere di separazione tra le colonne di una pagina, ed invia al processo di scrittura
    le righe delle pagine, non appena quest'ultime sono state completate.

    I commenti in questa funzione sono ridotti al minimo, poiché sarebbero una sola ripetizione dei commenti
    che sono presenti nella funzione della versione monoprocesso.
*/
int build_pages_par(int* pipefd_rs, int* pipefd_sw, int cols, int h_col, int w_col, int spacing, char spacing_char) {
    Page* curr_page = new_page(NULL);
    Page* prev_page = NULL;

    if (curr_page == NULL) {
        return ALLOC_ERROR;
    }

    int chunks_counter = 0;
    int col_counter = 0;

    bool h_col_reached = false;
    bool is_new_par = false;

    // rappresenta la lunghezza del prossimo chunk da leggere nella pipe
    int len;

    while (read(pipefd_rs[0], &len, sizeof(int))) {
        char* line_chunk_content = calloc(len, sizeof(char));

        int end_value;

        // vengono letti dalla pipe il prossimo chunk, ed 'end_value',
        // per sapere se la lettura del file è terminata
        read(pipefd_rs[0], line_chunk_content, len);
        read(pipefd_rs[0], &end_value, sizeof(int));

        if (is_empty(line_chunk_content, len)) {
            free(line_chunk_content);

            if (col_counter == 0 && chunks_counter % h_col == 0 && prev_page != NULL) {
                free(curr_page);

                prev_page->next_page = NULL;

                curr_page = prev_page;
            }

            break;
        }

        if (!h_col_reached) {
            if (append_line_and_advance(&curr_page, NULL) == NULL) {
                free(line_chunk_content);
                free(curr_page);
                return ALLOC_ERROR;
            }

            set_lines_head(curr_page);
        }

        if (append_line_chunk_and_advance((Line**) &curr_page->curr_line, line_chunk_content) == NULL) {
            free(line_chunk_content);
            free(curr_page);
            return ALLOC_ERROR;
        }

        set_line_chunks_head((Line*) curr_page->curr_line);

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
            // se la pagina è stata completata, allora viene mandata al processo di scrittura
            int exit_code = send_page(curr_page, pipefd_sw, cols, w_col, spacing, spacing_char);
            
            // TODO: CURR_PAGE VA LIBERATA

            if (exit_code != PAGE_SUCCESS) {
                free(prev_page);
                free(curr_page);
                return exit_code;
            }

            Page* new_page = append_page(curr_page, NULL);

            if (new_page == NULL) {
                free(prev_page);
                free(curr_page);
                return ALLOC_ERROR;
            }

            free(prev_page);

            prev_page = curr_page;
            curr_page = new_page;

            h_col_reached = false;

            col_counter = 0;
        }
    }

    // questo ulteriore invio è necessario, poiché altrimenti l'ultima pagina
    // non sarebbe stata mandata al processo di scrittura
    int exit_code = send_page(curr_page, pipefd_sw, cols, w_col, spacing, spacing_char);

    print_pages(stdout, curr_page, 10, "test", ' ');

    free(curr_page);

    if (exit_code != PAGE_SUCCESS) {
        return exit_code;
    }

    return PAGE_SUCCESS;
}

/*
    La funzione prende in input una pipe, il puntatore al file di output, il numero di righe di una pagina,
    la dimensione dello spazio tra le colonne di ogni pagina, ed il separatore tra pagine, e stampa le righe
    ricevute attraverso la pipe sul file di output.

    I commenti in questa funzione sono ridotti al minimo, poiché sarebbero una sola ripetizione dei commenti
    che sono presenti nella funzione della versione monoprocesso.
*/
int write_output_file_par(int* pipefd_sw, FILE* output_file, int h_col, int spacing, char* pages_separator) {
    if (pipefd_sw == NULL || output_file == NULL || !strcmp(pages_separator, "\0")) {
        return INVALID_INPUT;
    }
    
    // la lunghezza della prossima riga che verrà letta nella pipe
    int len;
    int read_code = read(pipefd_sw[0], &len, sizeof(int));

    if (read_code < 0) {
        return READ_ERROR;
    } else if (read_code == 0) {
        return PAGE_SUCCESS; // il file in input conteneva solamente spaziatura
    }

    int lines_counter = 0;

    while (true) {
        char* line = calloc(len, sizeof(char));

        if (line == NULL) {
            return ALLOC_ERROR;
        }

        // la prossima riga da scrivere nel file
        if (read(pipefd_sw[0], line, len) <= 0) {
            free(line);
            return READ_ERROR;
        }

        // viene stampata la riga sul file (si noti che
        // la riga ricevuta attraverso la pipe non conteneva '\n' alla fine)
        fprintf(output_file, "%s\n", line);

        lines_counter++;

        // la lunghezza della prossima riga che verrà letta nella pipe;
        // l'avanzamento viene fatto qui, e non come condizione di uscita del while,
        // poiché è necessario controllare che non venga inserito il separatore di pagina
        // dopo l'ultima pagina, e se questa lettura restituisce 0, allora gli elementi in
        // pipe sono terminati, e dunque il l'altro processo non ha più righe da mandare
        read_code = read(pipefd_sw[0], &len, sizeof(int));

        if (read_code != 0) {
            if (read_code < 0) {
                free(line);
                return READ_ERROR;
            }

            // se questa era l'ultima riga della pagina, e quella corrente non è l'ultima pagina
            // (si veda il controllo precedente), allora è possibile inserire il separatore tra pagine
            if (lines_counter == h_col) {
                fprintf(output_file, pages_separator);

                lines_counter = 0;
            }
        } else {
            break;
        }

        free(line);
    }

    return PAGE_SUCCESS;
}