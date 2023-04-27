#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char* content;

    struct LinePart* next_line_part;
} LinePart;

LinePart* new_line_part(char* content) {
    LinePart* new_line_part;
    new_line_part = calloc(1, sizeof(LinePart));

    if (new_line_part == NULL) {
        return NULL;
    }

    new_line_part->content = content;
    new_line_part->next_line_part = NULL;

    return new_line_part;
}

LinePart* append_line_part(LinePart* curr_line_part, char* content) {
    LinePart* next_line_part = new_line_part(content);

    if (next_line_part == NULL) {
        return NULL;
    }

    if (curr_line_part == NULL) {
        curr_line_part = next_line_part;
    } else {
        curr_line_part->next_line_part = (struct LinePart*) next_line_part;
    }

    return next_line_part;
}

void print_line_part(LinePart* line_part) {
    if (line_part == NULL) {
        return;
    }

    printf("%s", line_part->content);
}

typedef struct {
    struct LinePart* line_parts_head;
    struct LinePart* curr_line_part;

    struct Line* next_line;
} Line;

Line* new_line(LinePart* line_part) {
    Line* new_line;
    new_line = calloc(1, sizeof(Line));

    if (new_line == NULL) {
        return NULL;
    }

    new_line->line_parts_head = (struct LinePart*) line_part;
    new_line->curr_line_part = (struct LinePart*) line_part;

    new_line->next_line = NULL;

    return new_line;
}

Line* append_line(Line* curr_line, LinePart* line_part) {
    Line* next_line = new_line(line_part);

    if (next_line == NULL) {
        return NULL;
    }

    if (curr_line == NULL) {
        curr_line = next_line;
    } else {
        curr_line->next_line = (struct Line*) next_line;
    }

    return next_line;
}

void print_line(Line* line) {
    if (line == NULL) {
        return;
    }

    LinePart* curr_line_part = line->line_parts_head;

    while (curr_line_part != NULL) {
        print_line_part(curr_line_part);

        printf("  "); // TODO: da fare della dimensione appropriata

        curr_line_part = curr_line_part->next_line_part;
    }

    printf("\n");

    print_line(line->next_line);
}

typedef struct {
    int cols;
    int h_col;
    int w_col;
    int spacing;

    struct Line* lines_head;
    struct Line* curr_line;

    struct Page* next_page;
} Page;

Page* new_page(int cols, int h_col, int w_col, int spacing, Line* line) {
    Page* new_page;
    new_page = calloc(1, sizeof(Page));

    if (new_page == NULL) {
        return NULL;
    }

    new_page->cols = cols;
    new_page->h_col = h_col;
    new_page->w_col = w_col;
    new_page->spacing = spacing;

    new_page->lines_head = (struct Line*) line;
    new_page->curr_line = (struct Line*) line;

    new_page->next_page = NULL;

    return new_page;
}

Page* append_page(Page* curr_page, int cols, int h_col, int w_col, int spacing, Line* line) {
    Page* next_page;
    next_page = new_page(cols, h_col, w_col, spacing, line);

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

void print_page(Page* page) {
    if (page == NULL) {
        return;
    }

    Line* curr_line = page->lines_head;

    while (curr_line != NULL) {
        print_line(curr_line);

        curr_line = curr_line->next_line;
    }

    printf("\n \%\%\% \n");

    print_page(page->next_page);
}

int main(int argc, char* argv[]) {
    // TODO: file reading
    // char* input_content = "Sistemi Operativi II Modulo. Progetto (Homework).\n\nL’obiettivo è implementare un programma C che trasformi un testo in italiano da una colonna a più colonne su più pagine (come ad es. per una pagina di quotidiano). Sono richieste due versioni del programma: una a singolo processo e una multi-processo con almeno tre processi concorrenti.\n\nDati in ingresso e requisiti: un file di testo in codifica Unicode (UTF-8) contenente un testo in italiano strutturato in paragrafi separati da una o più linee vuote (stringa contenente solo il carattere ‘\n’). Ogni paragrafo è  costituito da una o più stringhe terminate da ‘\n’ (il testo in ingresso è tutto su una sola colonna); il numero di colonne per pagina su cui distribuire il testo; il numero di linee per ogni colonna; larghezza di ogni colonna (numero di caratteri per colonna); la distanza in caratteri tra una colonna di testo e l’altra.\n\nDati in uscita e requisiti: un file di testo ASCII contenente il testo di input incolonnato e impaginato secondo i parametri espressi dall’utente. Le pagine sono separate dalla stringa “\n %%% \n”; ogni paragrafo è separato da una linea vuota; la prima colonna di testo è a sinistra; tutte le righe, tranne l’ultima, di ogni paragrafo dovranno essere allineate ad entrambi i margini della colonna; l’ultima riga di ogni paragrafo è solo allineata a sinistra; le parole in una linea dovranno essere separate da almeno un carattere di spazio; la sillabazione di eventuali parole troppo lunghe non è necessaria.\n\nRequisiti generali del progetto: ogni file .c/.h  dovrà essere ben commentato: per ogni funzione commentare brevemente i parametri di ingresso/uscita e il suo funzionamento generale; nel corpo di ogni funzione commentare le linee di codice più importanti; fornire un makefile per compilare il progetto con un semplice make; fornire un file di testo README con una breve spiegazione dei file inclusi e un breve manuale utente; la versione multi-processo del programma dovrà essere costituita da almeno tre processi concorrenti ed intercomunicanti: uno per leggere il file di ingresso, uno per creare le colonne, ed infine uno per la scrittura del file di output. (Ulteriori suddivisioni del carico di lavoro in più di tre processi sono ammesse.) NON è ammesso l’uso di librerie esterne con funzioni per la manipolazione di stringhe o testo che non siano quelle standard del C. Eccezione: è ammesso l’uso di librerie esterne per l’analisi delle opzioni della linea di comando per il vostro programma.\n\nSuggerimenti: usare nomi di variabili e funzioni corrispondenti al loro significato/utilizzo; usare stdin e stdout per rispettivamente il testo in ingresso e in uscita; sviluppare prima la versiosne mono-processo del programma, poi quella multi-processo; per l’analisi della linea di comando si consiglia getopt, di uso molto semplice e che fa parte della libreria GNU standard del C; concentratevi prima sulla correttezza dei programmi, cioè assicuratevi con più testi di input che i programmi producano un output incolonnato secondo le richieste. Se avete tempo alla fine, ottimizzate il codice per uso di CPU e/o RAM, mantenendone la correttezza; potete usare wc per verificare che il numero delle parole in ingresso e in uscita dei vostri programmi sia lo stesso (usando stdin e stdout nel vostri programmi, questo semplice test si riduce ad una sola linea di comando con l’operatore pipe | della shell).\n\nIl punteggio massimo è 6/30, articolato come segue: correttezza del programma (3/30); architettura del programma e commenti (2/30); usabilità e istruzioni utente (1/30).";
    char* input_content = "lorem ipsum dolor sit amet";

    // TODO: grab this from argv
    // check if everything is != 0
    int cols = 3;
    int h_col = 40;
    int w_col = 20;
    int spacing = 4;

    h_col = 3; w_col = 4; // debug

    Line* curr_line = new_line(NULL);
    Page* curr_page = new_page(cols, h_col, w_col, spacing, curr_line);

    Page* first_page = curr_page; // backup

    int line_counter = 0;
    bool lines_stop = false;

    for (int i = 0; input_content[i] != '\0'; i += w_col) {
        char* line_part_content = calloc(w_col + 1, sizeof(char));

        bool end = false;
        
        for (int j = i; j < i + w_col; j++) {
            char curr_char = input_content[j];
            
            if (curr_char != '\0') {
                printf("%c", input_content[j]);
                // strcat(line_part_content, curr_char);
                line_part_content[j - i] = curr_char;
            } else {
                end = true;

                break;
            }
        }

        printf("\n");

        curr_line->curr_line_part = append_line_part(curr_line->curr_line_part, line_part_content);

        if (curr_line->line_parts_head == NULL) {
            curr_line->line_parts_head = curr_line->curr_line_part;
        }

        if (!lines_stop) {
            // TODO: STA QUA IL PROBLEMA
            curr_page->curr_line = append_line(curr_page->curr_line, NULL); // è la PROSSIMA riga, andrebbe levata quando ricomincio
        } else {
            Line* a = curr_page->curr_line;
            curr_page->curr_line = a->next_line;
        }

        curr_line = curr_page->curr_line;

        if (end) {
            break;
        }

        line_counter++;

        if (line_counter == h_col) {
            lines_stop = true;

            curr_line = curr_page->lines_head;
            curr_page->curr_line = curr_line;
        }
    }

    printf("begin---\n");
    print_page(first_page);
    // free(pages);
    // free(curr_line);

    return 0;
}