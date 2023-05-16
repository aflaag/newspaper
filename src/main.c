#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "args.h"
#include "page.h"
#include "page_par.h"

#define PROGRAM_SUCCESS 0 // restituita se non si sono verificati problemi durante l'intera esecuzione del programma
#define INVALID_INPUT_FILE -1 // restituita se non è stato possibile aprire il file di input nel percorso specificato
#define INVALID_OUTPUT_FILE -2 // restituita se non è stato possibile aprire il file di output nel percorso specificato
#define NON_EMPTY_OUTPUT_FILE -3 // restituita se il file di output specificato non è vuoto
#define ARGS_FAILURE -4 // restituita se è stato riscrontrato un errore negli argomenti forniti al programma
#define ALLOCATION_FAILURE -5 // restituita se si sono verificati errori durante le allocazioni nel progamma
#define INPUT_FILE_CLOSING_FAILURE -6 // restituita se non è stato possibile chiudere il file di input
#define OUTPUT_FILE_CLOSING_FAILURE -7 // restituita se non è stato possibile chiudere il file di output
#define INVALID_INPUT_TEXT -8 // restituita se il file di input conteneva parole più larghe della larghezza di colonna specificata
#define UNKNOWN_ERROR -9 // restituita se si sono verificati errori di natura ignota
#define FORK_ERROR -10 // restituita se si sono verificati errori durante il fork di un processo
#define PIPE_ERROR -11 // restituita se si sono verificati errori durante la lettura o la scrittura con una pipe tra processi

/*
    La funzione gestisce i codici di errori che vengono restituiti
    dalle varie funzioni del programma.
*/
int handle_exit_code(int exit_code) {
    switch (exit_code)  {
        case READ_ERROR:
            fprintf(stderr, "An error occurred while trying to read from a pipe.\n");
            return PIPE_ERROR;
        case WRITE_ERROR:
            fprintf(stderr, "An error occurred while trying to write into a pipe.\n");
            return PIPE_ERROR;
        case ALLOC_ERROR:
            fprintf(stderr, "An error occurred while trying to allocate memory in the program.\n");
            return ALLOCATION_FAILURE;
        case INSUFFICIENT_WIDTH:
            fprintf(stderr, "The file given as input contains words that are larger than the width provided.\n\nSee '--help' for more information\n");
            return INVALID_INPUT_TEXT;
        case INVALID_INPUT:
        case FSEEK_ERROR:
            fprintf(stderr, "An error occurred while running the program.\n");
            return UNKNOWN_ERROR;
        case PAGE_SUCCESS:
        default:
            return PROGRAM_SUCCESS; // TODO: ricontrolla che siano gestiti tutti boh
    }
}

int main(int argc, char* argv[]) {
    int cols;
    int h_col;
    int w_col;
    int spacing;
    
    char* input_path;
    char* output_path;
    
    // TODO: cambiare questo, deve accattare in input anche la flag per il multiprocesso,
    // e vanno fatte 2 funzioni qui nel main, in cui una runna il programma mono processo,
    // e l'altra multi processo
    int args_err = parse_args(argc, argv, &input_path, &output_path, &cols, &h_col, &w_col, &spacing);

    switch (args_err) {
        case NOT_ENOUGH_ARGS:
            fprintf(stderr, "Not enough argument passed, 6 arguments expected.\n\nSee '--help' for more information.\n");
            break;
        case CHARS_IN_NUMERIC_ARG:
            fprintf(stderr, "The last 4 arguments are expected to be strictly positive integers, but non-digits characters where found.\n\nSee '--help' for more information.\n");
            break;
        case ZERO_INTEGER:
            fprintf(stderr, "0 as value is not allowed.\n\nSee '--help' for more information.\n");
            break;
        case PRINT_HELP:
            fprintf(stderr, "%s", HELP_MESSAGE); // TODO: cambia, fai tanti print in una funzione
            return PROGRAM_SUCCESS;
        default:
            break;
    }
    
    if (args_err != ARGS_SUCCESS) {
        return ARGS_FAILURE;
    }

    int pipefd_sw[2];

    if (pipe(pipefd_sw) == -1) {
        fprintf(stderr, "An error has occured while trying to create a pipe.\n");
        return PIPE_ERROR;
    }

    pid_t pid = fork();

    if (pid == -1) {
        fprintf(stderr, "An error has occurred while trying to fork.\n");
        return FORK_ERROR;
    }
    
    if (pid == 0) {
        int pipefd_rs[2];

        if (pipe(pipefd_rs) == -1) {
            fprintf(stderr, "An error has occured while trying to create a pipe.\n");
            return PIPE_ERROR;
        }

        pid = fork();

        if (pid == -1) {
            fprintf(stderr, "An error has occurred while trying to fork.\n");
            return FORK_ERROR;
        }
        
        if (pid == 0) {
            FILE* input_file = fopen(input_path, "r");

            if (input_file == NULL) {
                fprintf(stderr, "An error occurred while trying to open the input file '%s'.\n", input_path);
                return INVALID_INPUT_FILE;
            }

            if (close(pipefd_rs[0]) == -1) {
                fprintf(stderr, "An error occurred while trying to close a pipe.\n");
                return PIPE_ERROR;
            }

            int exit_code = read_input_file_par(pipefd_rs, input_file, cols, h_col, w_col);

            exit_code = handle_exit_code(exit_code);

            if (fclose(input_file)) {
                fprintf(stderr, "An error occurred while trying to close the input file '%s'.\n", input_path);
                return INPUT_FILE_CLOSING_FAILURE;
            }

            if (close(pipefd_rs[1]) == -1) {
                fprintf(stderr, "An error occurred while trying to close a pipe.\n");
                return PIPE_ERROR;
            }

            return exit_code; // TODO: capire se questa cosa va bene
        } else {
            if (close(pipefd_rs[1]) == -1 ||
                close(pipefd_sw[0]) == -1) {
                fprintf(stderr, "An error occurred while trying to close a pipe.\n");
                return PIPE_ERROR;
            }

            int exit_code = build_pages_par(pipefd_rs, pipefd_sw, cols, h_col, spacing, ' ');

            exit_code = handle_exit_code(exit_code);

            if (close(pipefd_rs[0]) == -1 ||
                close(pipefd_sw[1]) == -1) {
                fprintf(stderr, "An error occurred while trying to close a pipe.\n");
                return PIPE_ERROR;
            }

            return exit_code; // TODO: capire se questa cosa va bene
        }
    } else {
        FILE* output_file = fopen(output_path, "a");

        if (output_file == NULL) {
            fprintf(stderr, "An error occurred while trying to open the output file '%s'.\n", output_path);

            return INVALID_OUTPUT_FILE;
        }

        if (!is_file_empty(output_file)) {
            fprintf(stderr, "Output file '%s' is not empty, the program will not run.\n\nSee '--help' for more information.\n", output_path);

            if (fclose(output_file)) {
                fprintf(stderr, "An error occurred while trying to closing the output file '%s'.\n", output_path);
                return OUTPUT_FILE_CLOSING_FAILURE;
            }

            return NON_EMPTY_OUTPUT_FILE;
        }

        if (close(pipefd_sw[1]) == -1) {
            fprintf(stderr, "An error occurred while trying to close a pipe.\n");
            return PIPE_ERROR;
        }
        
        int exit_code = write_output_file_par(pipefd_sw, output_file, h_col, spacing, "\n%%%%%%\n\n");

        exit_code = handle_exit_code(exit_code);

        if (fclose(output_file)) {
            fprintf(stderr, "An error occurred while trying to closing the output file '%s'.\n", output_path);
            return OUTPUT_FILE_CLOSING_FAILURE;
        }

        if (close(pipefd_sw[0]) == -1) {
            fprintf(stderr, "An error occurred while trying to close a pipe.\n");
            return PIPE_ERROR;
        }

        return exit_code; // TODO: capire se questa cosa va bene
    }

    // TODO: qua in qualche modo dovrei prendere i risultati dei processi figli boh non lo so, o forse no

    return PROGRAM_SUCCESS;
}