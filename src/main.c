#include <stdio.h>

#include "args.h"
#include "page.h"

#define PROGRAM_SUCCESS 0
#define INVALID_INPUT_FILE -1
#define INVALID_OUTPUT_FILE -2
#define NON_EMPTY_OUTPUT_FILE -3
#define ARGS_FAILURE -4
#define ALLOCATION_FAILURE -5
#define INPUT_FILE_CLOSING_FAILURE -6
#define OUTPUT_FILE_CLOSING_FAILURE -7
#define INVALID_INPUT_TEXT -8
#define UNKNOWN_ERROR -9

int main(int argc, char* argv[]) {
    int cols;
    int h_col;
    int w_col;
    int spacing;
    
    char* input_path;
    char* output_path;
    
    int args_err = parse_args(argc, argv, &input_path, &output_path, &cols, &h_col, &w_col, &spacing);
    // int args_err = 0;

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
            fprintf(stderr, "TODO: FAI L'HELP\n");
            break;
        default:
            break;
    }
    
    if (args_err != ARGS_SUCCESS) {
        return ARGS_FAILURE;
    }


    FILE* input_file = fopen(input_path, "r");

    // cols = 3; h_col = 40; w_col = 21; spacing = 10; input_file = fopen("../sample_inputs/utf8_prof_in.txt", "r");

    if (input_file == NULL) {
        fprintf(stderr, "An error occurred while trying to open the input file '%s'.\n", input_path);
        return INVALID_INPUT_FILE;
    }

    Page* pages = new_page(NULL);

    if (pages == NULL) {
        fprintf(stderr, "An error occurred while trying to allocate memory in the program.\n");
        return ALLOCATION_FAILURE;
    }

    int exit_code = build_pages(input_file, pages, cols, h_col, w_col);

    if (fclose(input_file)) {
        fprintf(stderr, "An error occurred while trying to closing the input file '%s'.\n", input_path);
        return INPUT_FILE_CLOSING_FAILURE;
    }

    FILE* output_file = fopen(output_path, "a");

    if (output_file == NULL) {
        fprintf(stderr, "An error occurred while trying to open the output file '%s'.\n", output_path);
        return INVALID_OUTPUT_FILE;
    }

    if (!is_file_empty(output_file)) {
        fprintf(stderr, "Output file '%s' is not empty, the program will not run.\n\nSee '--help' for more information.\n", output_path);
        return NON_EMPTY_OUTPUT_FILE;
    }

    switch (exit_code)  {
        case ALLOC_ERROR:
            fprintf(stderr, "REALLOC: TODO ERROR HANDLING\n");
            break;
        case INSUFFICIENT_WIDTH:
            fprintf(stderr, "WIDTH: TODO ERROR HANDLING\n");
            // break;
        case INVALID_INPUT:
            fprintf(stderr, "INVALID INPUT: TODO ERROR HANDLING\n");
            // break;
        case PAGE_SUCCESS:
            print_pages(output_file, pages, spacing, "\n%%%\n\n", ' ');
            break;
        default:
            break;
    }

    free_pages(pages); // TODO: dovrebbe funzionare bene, però debugga just to make sure, anche il free di line_chunk_content in page.h

    if (fclose(output_file)) {
        fprintf(stderr, "An error occurred while trying to closing the output file '%s'.\n", output_path);
        return OUTPUT_FILE_CLOSING_FAILURE;
    }

    switch (exit_code)  {
        case ALLOC_ERROR:
            return ALLOCATION_FAILURE;
        case INSUFFICIENT_WIDTH:
            return INVALID_INPUT_TEXT;
        case INVALID_INPUT:
            return UNKNOWN_ERROR;
        case PAGE_SUCCESS:
            break;
        default:
            break;
    }

    return PROGRAM_SUCCESS;
}