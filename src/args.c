#include "args.h"

/*
    La funzione prende in input un file, e restituisce true se il puntatore al file fornito è vuoto;
    si noti che la funzione restituisce true anche se il puntatore è NULL, e restituisce false anche se
    la 'fseek' ha restituito errore.
*/
bool is_file_empty(FILE* file) {
    if (file != NULL) {
        if (fseek(file, 0, SEEK_END)) {
            return false;
        }

        if (ftell(file) != 0) {
            return false;
        }
    }

    return true;
}

/*
    La funzione prende input argc ed argv del programma, una flag da cercare e la sua forma alternativa,
    e restituisce l'indice dell'argomento che contiene la flag; se la flag non è presente, o vi erano
    input non validi, la funzione restituisce -1.
*/
int find_flag(int argc, char* argv[], char* flag, char* flag_short) {
    if (argv == NULL || flag == NULL || flag_short == NULL) {
        return -1;
    }

    for (int i = 0; i < argc; i++) {
        if (!strcmp(argv[i], flag) || !strcmp(argv[i], flag_short)) {
            return i;
        }
    }
    
    return -1;
}

/*
    La funzione prende in input l'indice base, e l'indice dell'eventuale flag '--par', e restituisce
    il nuovo valore dell'indice, nel caso in cui la flag era presente; in particolare, ogni indice che
    si trova dopo l'indice della flag (se presente), deve essere spostato di 1 in avanti.
*/
int set_index(int index, int flag_index) {
    return index >= flag_index && flag_index >= 1 ? index + 1 : index;
}

/*
    La funzione prende in input i puntatori agli argomenti, ed effettua il parsing degli argomenti
    forniti in input al programma; consultare la documentazione per maggiori informazioni.
*/
int parse_args(int argc, char* argv[], char** input_path, char** output_path, int* cols, int* h_col, int* w_col, int* spacing) {
    if (argc == 2) {
        if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")) {
            return PRINT_HELP;
        }
    }

    // cerca la posizione dell'eventuale flag '--par',
    // o nella versione breve '-p'; il valore di 'flag_index' sarà
    // pari a -1 nel caso in cui la flag non era presente negli argomenti passati
    int flag_index = find_flag(argc, argv, "--par", "-p");

    // se il numero di argomenti è minore di 7, oppure è pari a 7,
    // e tra gli argomenti era presente la flag, allora
    // non sono presenti argomenti a sufficienza
    if (argc < 7 || argc == 7 && flag_index >= 1) {
        return NOT_ENOUGH_ARGS;
    }

    // viceversa, se il numero di argomenti è superiore ad 8
    // (ovvero, 7 + l'opzionale flag)
    if (argc > 8 || argc == 8 && flag_index < 1) {
        return TOO_MANY_ARGS;
    }

    *input_path = argv[set_index(1, flag_index)];
    *output_path = argv[set_index(2, flag_index)];

    char* raw_cols = argv[set_index(3, flag_index)];
    char* raw_h_col = argv[set_index(4, flag_index)];
    char* raw_w_col = argv[set_index(5, flag_index)];
    char* raw_spacing = argv[set_index(6, flag_index)];

    if (ATOI_INVALID_CHAR == pos_atoi(raw_cols, strlen(raw_cols), cols)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (ATOI_INVALID_CHAR == pos_atoi(raw_h_col, strlen(raw_h_col), h_col)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (ATOI_INVALID_CHAR == pos_atoi(raw_w_col, strlen(raw_w_col), w_col)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (ATOI_INVALID_CHAR == pos_atoi(raw_spacing, strlen(raw_spacing), spacing)) {
        return CHARS_IN_NUMERIC_ARG;
    }

    if (*cols == 0 || *h_col == 0 || *w_col == 0 || *spacing == 0) {
        return ZERO_INTEGER;
    }

    return flag_index >= 1 ? ARGS_SUCCESS_PAR : ARGS_SUCCESS;
}