#include "string_tools.h"

/*
    La funzione prende in input una stringa, un carattere da rimpiazzare, e un carattere di rimpiazzo, e
    sostiuisce ogni istanza del primo carattere con il secondo, se possibile.
*/
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

/*
    La funzione restituisce true se il carattere in input può essere considerato come parte di una parola;
    si noti che caratteri come l'apostrofo o i segni di punteggiatura, poiché vengono scritti senza spazi tra parole,
    restituiscono true poiché considerati parte di parola.
*/
bool is_char(char character) {
    return character != ' ' && character != '\n' && character != '\0' && character != EOF && character != '\t' && character != '\r';
}

/*
    La funzione prende in input una stringa, un punto di partenza ed uno di fine, ed il carattere con il quale
    si vuole effettuare il padding della stringa fornita, e sovrascrive ogni carattere di questa a partire
    dall'indice d'inizio fornito, fino a quello di fine; se gli indici non sono validi, la funzione non modifica
    la stringa in input.
*/
void pad_string(char* string, unsigned int start, unsigned int end, char character) {
    for (int i = start; i < end; i++) {
        string[i] = character;
    }
}

/*
    La funzione restituisce true se il carattere in input è un carattere facente parte della codifica UTF-8.
*/
bool is_utf8(unsigned char character) {
    return character >> 6 == 0b10;
}

/*
    La funzione restituisce true, se il chunk in input, fornito assieme alla sua lunghezza ed
    al primo carattere del prossimo chunk, contiene una parola troncata alla fine; la funzione non
    effettua controlli sulla lunghezza fornita, né sul puntatore della stringa.
*/
bool check_truncated_end(char* line_chunk_content, int w_col, char next_char) {
    return is_char(line_chunk_content[w_col - 1]) && (is_char(next_char) || is_utf8(next_char));
}

/*
    La funzione restituisce true, se la stringa fornita (che verrà controllata su 'len' caratteri),
    contiene esclusivamente '\0'; la funzione restituisce true anche se il puntatore alla stringa è NULL;
    la funzione non effettua controlli sulla lunghezza fornita.
*/
bool is_empty(char* string, int len) {
    if (string == NULL) {
        return true;
    }

    for (int i = 0; i < len; i++) {
        if (string[i] != '\0') {
            return false;
        }
    }

    return true;
}

/*
    La funzione prende in input una stringa, e la sua nuova dimensione, e se possibile,
    aggiorna il puntatore char** di input con l'indirizzo alla nuova stringa di dimensione fornita;
    si noti che la funzione tronca, e ritorna codici di errore nel caso in cui non è
    stato possibile effettuare il troncamento; la funzione, in caso di successo, garantisce
    che la stringa fornita in input venga deallocata, e ne viene riallocata una nuova.
*/
int truncate_string(char** string, int new_size) {
    if (string == NULL || *string == NULL) {
        return TRUNCATION_NOT_PERFORMED;
    }

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
    La funzione prende in input una stringa, la sua larghezza, e un carattere,
    e rimpiazza con quest'ultimo tutta l'ultima parola della stringa fornita;
    la funzione non effetua controlli sul puntatore alla stringa, né sulla lunghezza fornita.
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

/*
    La funzione restituisce true se la stringa fornita, assieme alla sua lunghezza, non contiene spazi;
    la funzione non effettua controlli sulla lunghezza fornita, e restituisce true anche se il puntatore
    alla stringa in input è NULL.
*/
bool no_spaces(char* string, int len) {
    if (string == NULL) {
        return true;
    }

    for (int i = 0; i < len; i++) {
        if (string[i] == ' ') {
            return false;
        }
    }

    return true;
}

/*
    La funzione restituisce il numero di caratteri della stringa fornita input; la funzione non effettua
    controlli sul puntatore della stringa in input, né sulla lunghezza fornita.
*/
int count_words(char* string, int len) {
    // flag che viene settata a true quando ci si trova
    // all'interno di una parola
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

    // se, dopo aver terminato il ciclo, la flag 'in_word' è attiva, allora
    // la stringa non termina con uno spazio, e dunque senza questo controllo
    // l'ultima parola non verrebbe contata dall'algoritmo
    if (in_word) {
        words++;
    }

    return words;
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

void slide_characters(char* string, int len, int spaces_end, int spaces_inside) {
    // il numero di spazi per buco tra parole, è pari al numero di spazi alla fine della riga,
    // diviso per il numero di buchi; naturalmente tale rapporto può non avere resto 0, e dunque
    // è necessario arrotondare il rapporto per difetto, e gestire il resto della divisione
    // come se fossero ulteriori spazi da ridistribuire
    int ratio = spaces_end / spaces_inside;
    int remainder = spaces_end % spaces_inside;

    int head = 0;
    int tail = 0;

    // viene creata una coda lunga pari alla lunghezza fornita in input,
    // e viene riempita di '\0' completamente; tale carattere sarà usato dall'algoritmo
    // per sapere se un certo slot della coda è disponibile o meno
    char queue[len]; // TODO: dire che non faccio controlli sull'input
    pad_string(queue, 0, len, '\0');

    for (int i = 1; i < len; i++) {
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
                
                int curr_spaces = ratio;

                if (remainder != 0) {
                    curr_spaces++;
                    remainder--;
                }

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
        } else if (is_char(string[i]) && !is_char(string[i - 1])) {
            int curr_spaces = ratio;

            if (remainder != 0) {
                curr_spaces++;
                remainder--;
            }

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

/*
    La funzione prende in input una stringa e la sua lunghezza, e ne giustifica il testo;
    l'output restituito è garantito essere corretto solamente se la stringa non contiene
    spazi multipli tra le parole; la funzione non effettua controlli sul puntatore alla stringa,
    né sulla lunghezza fornita.
*/
void justify_string(char* string, int len) {
    int spaces_end = 0;

    // viene calcolato il numero di spazi alla fine della riga
    for (int i = len - 1; string[i] == ' ' && i >= 0; i--) {
        spaces_end++;
    }

    // se il numero di spazi alla fine della riga è pari a 0,
    // allora non ci sono spazi alla fine da ridistribuire, e poiché
    // la lettura dei chunk è stata effettuata in modo tale da prendere
    // sempre 1 spazio tra le parole, allora non è necessario effettuare
    // la giustificazione
    if (spaces_end == 0) {
        return;
    }

    // il numero di spazi tra le parole (o intervalli) sarà esattamente il numero
    // di parole - 1, poiché la stringa in input contiene parole separate da 1 solo spazio
    int spaces_inside = count_words(string, len) - 1;

    // se non ci sono spazi tra le parole, ma c'erano spazi alla fine
    // della riga, allora la stringa è costituita da una sola parola,
    // e dunque non è necessario effettuarne la giustificazione
    if (spaces_inside == 0) {
        return;
    }

    slide_characters(string, len, spaces_end, spaces_inside);
}

/*
    La funzione prende in input un puntatore ad una stringa, e la nuova dimensione che le si vuole fornire,
    e ritorna, se possibile, un puntatore ad una stringa con la dimensione richiesta, con lo stesso
    contenuto della stringa in input; la funzione non effettua controlli sulla dimensione della stringa.
*/
char* string_realloc(char* string, int new_size) {
    if (string == NULL) {
        return NULL;
    }

    char* larger_chunk = realloc(string, new_size + 1); // null byte

    if (larger_chunk == NULL) {
        return NULL;
    }

    larger_chunk[new_size] = '\0';

    return larger_chunk;
}

/*
    La funzione restituisce n^exp.
*/
int powi(int n, unsigned int exp) {
    int tot = 1;

    for (int i = 0; i < exp; i++) {
        tot *= n;
    }

    return tot;
}

/*
    La funzione prende in input una stringa, la sua lunghezza, e un puntatore ad un intero positivo,
    al quale verrà assegnato il numero che la stringa rappresenta, nel caso in cui il parsing
    non abbia prodotto errori.
*/
int pos_atoi(char* str, int len, unsigned int* number) {
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