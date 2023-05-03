#include <stdio.h>

#include "args.h"
#include "page.h"

#define PROGRAM_SUCCESS 0
#define INVALID_INPUT_FILE -1
#define INVALID_OUTPUT_FILE -2
#define NON_EMPTY_OUTPUT_FILE -3
#define ARGS_FAILURE -4
#define ALLOCATION_FAILURE -5

int main(int argc, char* argv[]) {
    // int cols = 3;
    // int h_col = 40;
    // int w_col = 21;
    // int spacing = 10;

    // h_col = 4; w_col = 5; // debug

    // h_col = 7; w_col = 15; // debug, capobastone
    // h_col = 7; w_col = 18;

    int cols;
    int h_col;
    int w_col;
    int spacing;
    
    char* input_path;
    char* output_path;
    
    int args_err = parse_args(argc, argv, &input_path, &output_path, &cols, &h_col, &w_col, &spacing);

    switch (args_err) {
        case NOT_ENOUGH_ARGS:
            fprintf(stderr, "Not enough argument passed, 7 arguments expected.\n\nSee '--help' for more information.\n");
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


    // TODO: fai diventare questi tipo dei test, CHE SERVE ANCHE PER QUANDO LO DEVI RIFARE MULTIPROCESSORE
    // char* input_content = "lorem ipsum dolor sit amet"; h_col = 4; w_col = 5;
    // char* input_content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.xxxxxxxxxxxx\n\n   Mauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.\n\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.\n\nMauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. \n\n    Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.";
    // char* input_content = "ciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene      mi      sento veramente fortissimo ciao a tutti.       \n\n      questa e' una prova di paragrafo, speriamo non esploda tutto quanto.\n\nciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene mi sento veramente fortissimo ciao a tutti quanti.      \n\n      questo e' una prova di paragrafo, speriamo non esploda tutto qua    nto.";
    // char* input_content = "questo e' un super test che mi serve per capire se quello che sto facendo sta effettivamente funzionando oppure non sta funzionando proprio assolutamente niente, ti prego funzona pls.\n\nah e questa e' una prova di paragrafo.";
    // char* input_content = "a                        b";
    // char* input_content = "a x x b x x c c iaone come stae"; h_col = 7; w_col = 18; // TODO: controlla quando arriva all'ultimo (devo pensare a com'è fatto il test)

    FILE* input_file = fopen(input_path, "r");
    FILE* output_file = fopen(output_path, "a");

    if (input_file == NULL) {
        fprintf(stderr, "An error occurred while trying to open the input file '%s'.\n", input_path);
        
        return INVALID_INPUT_FILE;
    }

    if (output_file == NULL) {
        fprintf(stderr, "An error occurred while trying to open the output file '%s'.\n", output_path);

        return INVALID_OUTPUT_FILE;
    }

    if (!is_file_empty(output_file)) {
        fprintf(stderr, "Output file '%s' is not empty, the program will not run.\n\nSee '--help' for more information.\n", output_path);

        return NON_EMPTY_OUTPUT_FILE;
    }

    Page* pages = new_page(NULL);

    if (pages == NULL) {
        fprintf(stderr, "An error occurred while trying to allocate memory in the program.\n");

        return ALLOCATION_FAILURE;
    }

    int exit_code = build_pages(input_file, pages, cols, h_col, w_col);

    switch (exit_code)  {
        case ALLOC_ERROR:
            fprintf(stderr, "REALLOC: TODO ERROR HANDLING\n");
            break;
        case INSUFFICIENT_WIDTH:
            fprintf(stderr, "WIDTH: TODO ERROR HANDLING\n");
            break;
        case PAGE_SUCCESS:
            printf("tutto ok zi\n");

            print_pages(output_file, pages, spacing, "\n%%%\n\n", ' '); // TODO: se questi sono NULL che succede? ehehe

            break;
        default:
            break;
    }

    free_pages(pages); // TODO: dovrebbe funzionare bene, però debugga just to make sure, anche il free di line_chunk_content in page.h

    fclose(input_file); // TODO: gestire l'errore (che me invento?!)

    return PROGRAM_SUCCESS;
}