#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

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

typedef struct {
    struct LineChunk* line_chunks_head;
    struct LineChunk* curr_line_chunk;

    struct Line* next_line;
} Line;

Line* new_line(LineChunk* line_chunk) {
    Line* new_line;
    new_line = calloc(1, sizeof(Line));

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

    if (curr_line == NULL) {
        curr_line = next_line;
    } else {
        curr_line->next_line = (struct Line*) next_line;
    }

    return next_line;
}

void set_line_chunks_head(Line* line) {
    if (line->line_chunks_head == NULL) {
        line->line_chunks_head = line->curr_line_chunk;
    }
}

void print_line(Line* line, int spacing) {
    if (line == NULL) {
        return;
    }

    LineChunk* curr_line_chunk = line->line_chunks_head;

    while (curr_line_chunk != NULL) {
        print_line_chunk(curr_line_chunk);

        for (int i = 0; i < spacing; i++) {
            printf(" ");
        }

        curr_line_chunk = curr_line_chunk->next_line_chunk;
    }

    printf("\n");
}

typedef struct {
    // TODO: RIMUOVI STI TRE CHE NON LI USO MAI
    int cols;
    int h_col;
    int w_col;

    struct Line* lines_head;
    struct Line* curr_line;

    struct Page* next_page;
} Page;

Page* new_page(int cols, int h_col, int w_col, Line* line) {
    Page* new_page;
    new_page = calloc(1, sizeof(Page));

    if (new_page == NULL) {
        return NULL;
    }

    new_page->cols = cols;
    new_page->h_col = h_col;
    new_page->w_col = w_col;

    new_page->lines_head = (struct Line*) line;
    new_page->curr_line = (struct Line*) line;

    new_page->next_page = NULL;

    return new_page;
}

Page* append_page(Page* curr_page, int cols, int h_col, int w_col, Line* line) {
    Page* next_page;
    next_page = new_page(cols, h_col, w_col, line);

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
    Line* curr_line = page->curr_line;

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

void print_pages(Page* page, int spacing, char* pages_separator) {
    if (page == NULL) {
        return;
    }

    Line* curr_line = page->lines_head;

    while (curr_line != NULL) {
        print_line(curr_line, spacing);

        curr_line = curr_line->next_line;
    }

    if (page->next_page != NULL) {
        printf("%s", pages_separator);

        print_pages(page->next_page, spacing, pages_separator);
    }
}

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
    return character != ' ' && character != '\n' && character != '\0';
}

bool read_chunk(char* input_content, char* line_chunk_content, int w_col, int* base_idx) {
    bool is_text_started = false;

    int i = *base_idx;
    int finish = i + w_col;

    for (int j = i; j < finish; j++) {
        char curr_char = input_content[j];
        char next_char = input_content[j + 1];
        
        if (curr_char != '\0') {
            if ((!is_text_started && !is_char(curr_char)) || (is_text_started && !is_char(curr_char) && !is_char(next_char))) { // TODO: controlla \n\n
                finish++;
                *base_idx += 1;
            } else {
                // printf("%c", input_content[j]);
                is_text_started = true;

                line_chunk_content[j - finish + w_col] = curr_char; // j - i - (finish - i - w_col)
            }
        } else {
            return true;
        }
    }

    return false;
}

bool check_truncated_end(char* line_chunk_content, int w_col, char next_char) {
    return is_char(line_chunk_content[w_col - 1]) && is_char(next_char);
}

int replace_truncated_chars(char* line_chunk_content, int w_col) {
    int truncated_chars = 0;

    for (int i = w_col - 1; is_char(line_chunk_content[i]) && i >= 0; i--) {
        line_chunk_content[i] = ' ';

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

// https://en.wikipedia.org/wiki/Floor_and_ceiling_functions#Quotients
int ceil_division(int x, int y) {
    return (x + y - 1) / y;
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

void initialize(char queue[], int len) {
    for (int i = 0; i < len; i++) {
        queue[i] = '\0';
    }
}

void enqueue(char queue[], int len, int* head, int* tail, char element) {
    if (*tail >= 0 && *tail < len - 1) {
        queue[*tail] = element;

        *tail += 1;
    }
}

char dequeue(char queue[], int len, int* head, int* tail) {
    if (*head < *tail) {
        char element = queue[*head];

        queue[*head] = '\0';

        *head += 1;

        return element;
    }
}

void slide_characters(char* string, int len, int spaces_end, int ratio) {
    int head = 0;
    int tail = 0;

    char queue[len];

    initialize(queue, len);

    for (int i = 1; i < len; i++) {
        char DEBUG = string[i];
        if (tail != 0) {
            if (is_char(string[i]) && !is_char(queue[tail - 1])) {
                while (is_char(queue[head])) {
                    enqueue(queue, len, &head, &tail, string[i]);

                    string[i] = dequeue(queue, len, &head, &tail);

                    i++;
                }

                enqueue(queue, len, &head, &tail, string[i]);
                string[i] = dequeue(queue, len, &head, &tail);
                i++;
                
                int curr_spaces = min(spaces_end, ratio);

                spaces_end -= curr_spaces;

                while (curr_spaces > 0) {
                    enqueue(queue, len, &head, &tail, string[i]);

                    string[i] = ' ';

                    curr_spaces--;

                    i++;
                }

                i--;
            } else {
                enqueue(queue, len, &head, &tail, string[i]);

                string[i] = dequeue(queue, len, &head, &tail);
            }
        } else {
            if (is_char(string[i]) && !is_char(string[i - 1])) {
                int curr_spaces = min(spaces_end, ratio);

                spaces_end -= curr_spaces;

                while (curr_spaces > 0) {
                    enqueue(queue, len, &head, &tail, string[i]);

                    string[i] = ' ';

                    curr_spaces--;

                    i++;
                }

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
        // printf("X X\n");
        return;
    }

    int spaces_inside = count_words(string, len) - 1;

    // printf("%d ", spaces_inside);

    // TODO: ricordati di scrivere che è garantito
    // che gli space chunks abbiano esattamente 1 spazio
    if (spaces_inside == 0) { // una sola parola
        // printf("X\n");
        return;
    }

    int ratio = ceil_division(spaces_end, spaces_inside);

    // printf("%d\n", ratio);

    slide_characters(string, len, spaces_end, ratio);
}

Page* build_pages(char* input_content, int cols, int h_col, int w_col) {
    Page* curr_page = new_page(cols, h_col, w_col, NULL);
    Page* first_page = curr_page; // backup

    int line_counter = 0;
    bool h_col_reached = false;

    int col_counter = 0;

    for (int i = 0; input_content[i] != '\0'; i += w_col) {
        char* line_chunk_content = calloc(w_col + 1, sizeof(char));

        bool end = read_chunk(input_content, line_chunk_content, w_col, &i);

        if (check_truncated_end(line_chunk_content, w_col, input_content[i + w_col])) {
            if (no_spaces(line_chunk_content, w_col)) {
                return curr_page; // TODO: free vari, inoltre questo dovrebbe essere NULL probabilmente
            }

            i -= replace_truncated_chars(line_chunk_content, w_col);
        }

        string_replace(line_chunk_content, '\n', ' ');

        // printf("%s %d %d\n", line_chunk_content, strlen(line_chunk_content), w_col);
        printf("%s\n", line_chunk_content);

        justify_string(line_chunk_content, w_col);

        printf("%s\n", line_chunk_content);

        if (!h_col_reached) {
            curr_page->curr_line = append_line(curr_page->curr_line, NULL);

            set_lines_head(curr_page);
        }

        Line* curr_line = curr_page->curr_line;

        curr_line->curr_line_chunk = append_line_chunk(curr_line->curr_line_chunk, line_chunk_content);

        set_line_chunks_head(curr_line);

        if (end) {
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

int main(int argc, char* argv[]) {
    // TODO: file reading
    // char* input_content = "Sistemi Operativi II Modulo. Progetto (Homework).\n\nL’obiettivo è implementare un programma C che trasformi un testo in italiano da una colonna a più colonne su più pagine (come ad es. per una pagina di quotidiano). Sono richieste due versioni del programma: una a singolo processo e una multi-processo con almeno tre processi concorrenti.\n\nDati in ingresso e requisiti: un file di testo in codifica Unicode (UTF-8) contenente un testo in italiano strutturato in paragrafi separati da una o più linee vuote (stringa contenente solo il carattere ‘\n’). Ogni paragrafo è  costituito da una o più stringhe terminate da ‘\n’ (il testo in ingresso è tutto su una sola colonna); il numero di colonne per pagina su cui distribuire il testo; il numero di linee per ogni colonna; larghezza di ogni colonna (numero di caratteri per colonna); la distanza in caratteri tra una colonna di testo e l’altra.\n\nDati in uscita e requisiti: un file di testo ASCII contenente il testo di input incolonnato e impaginato secondo i parametri espressi dall’utente. Le pagine sono separate dalla stringa “\n %%% \n”; ogni paragrafo è separato da una linea vuota; la prima colonna di testo è a sinistra; tutte le righe, tranne l’ultima, di ogni paragrafo dovranno essere allineate ad entrambi i margini della colonna; l’ultima riga di ogni paragrafo è solo allineata a sinistra; le parole in una linea dovranno essere separate da almeno un carattere di spazio; la sillabazione di eventuali parole troppo lunghe non è necessaria.\n\nRequisiti generali del progetto: ogni file .c/.h  dovrà essere ben commentato: per ogni funzione commentare brevemente i parametri di ingresso/uscita e il suo funzionamento generale; nel corpo di ogni funzione commentare le linee di codice più importanti; fornire un makefile per compilare il progetto con un semplice make; fornire un file di testo README con una breve spiegazione dei file inclusi e un breve manuale utente; la versione multi-processo del programma dovrà essere costituita da almeno tre processi concorrenti ed intercomunicanti: uno per leggere il file di ingresso, uno per creare le colonne, ed infine uno per la scrittura del file di output. (Ulteriori suddivisioni del carico di lavoro in più di tre processi sono ammesse.) NON è ammesso l’uso di librerie esterne con funzioni per la manipolazione di stringhe o testo che non siano quelle standard del C. Eccezione: è ammesso l’uso di librerie esterne per l’analisi delle opzioni della linea di comando per il vostro programma.\n\nSuggerimenti: usare nomi di variabili e funzioni corrispondenti al loro significato/utilizzo; usare stdin e stdout per rispettivamente il testo in ingresso e in uscita; sviluppare prima la versiosne mono-processo del programma, poi quella multi-processo; per l’analisi della linea di comando si consiglia getopt, di uso molto semplice e che fa chunke della libreria GNU standard del C; concentratevi prima sulla correttezza dei programmi, cioè assicuratevi con più testi di input che i programmi producano un output incolonnato secondo le richieste. Se avete tempo alla fine, ottimizzate il codice per uso di CPU e/o RAM, mantenendone la correttezza; potete usare wc per verificare che il numero delle parole in ingresso e in uscita dei vostri programmi sia lo stesso (usando stdin e stdout nel vostri programmi, questo semplice test si riduce ad una sola linea di comando con l’operatore pipe | della shell).\n\nIl punteggio massimo è 6/30, articolato come segue: correttezza del programma (3/30); architettura del programma e commenti (2/30); usabilità e istruzioni utente (1/30).";
    // char* input_content = "lorem ipsum dolor sit amet"; // TODO: FIXA PROBLEMA DELLE PAROLE PRECISE
    // char* input_content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.\n\nMauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.";
    char* input_content = "ciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene      mi      sento veramente fortissimo ciao a tutti quanti.\n\nquesta e' una prova di paragrafo, speriamo non esploda tutto quanto.\n\nciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene mi sento veramente fortissimo ciao a tutti quanti.\n\nquesto e' una prova di paragrafo, speriamo non esploda tutto qua    nto.";
    // char* input_content = "questo e' un super test che mi serve per capire se quello che sto facendo sta effettivamente funzionando oppure non sta funzionando proprio assolutamente niente, ti prego funzona pls.\n\nah e questa e' una prova di paragrafo.";
    // char* input_content = "a                        b";

    // TODO: grab this from argv
    // check if everything is != 0
    int cols = 3;
    int h_col = 40;
    int w_col = 20;
    int spacing = 10;

    // h_col = 4; w_col = 5; // debug
    h_col = 7; w_col = 15; // debug

    Page* pages = build_pages(input_content, cols, h_col, w_col);

    printf("\nbegin---\n\n");

    print_pages(pages, spacing, "\n%%%\n\n");

    // free(pages); // TODO: controlli + free di tutto quanto
    // free(curr_line);

    return 0;
}