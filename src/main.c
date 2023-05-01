#include <stdio.h>

#include "page.h"

int main(int argc, char* argv[]) {
    // TODO: grab this from argv
    // check if everything is != 0
    int cols = 3;
    int h_col = 40;
    int w_col = 21;
    int spacing = 10;

    // h_col = 4; w_col = 5; // debug

    // h_col = 7; w_col = 15; // debug, capobastone
    // h_col = 7; w_col = 18;

    // TODO: file reading
    // char* input_content = "lorem ipsum dolor sit amet"; h_col = 4; w_col = 5;
    // char* input_content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.xxxxxxxxxxxx\n\n   Mauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.\n\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.\n\nMauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. \n\n    Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.";
    // char* input_content = "ciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene      mi      sento veramente fortissimo ciao a tutti.       \n\n      questa e' una prova di paragrafo, speriamo non esploda tutto quanto.\n\nciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene mi sento veramente fortissimo ciao a tutti quanti.      \n\n      questo e' una prova di paragrafo, speriamo non esploda tutto qua    nto.";
    // char* input_content = "questo e' un super test che mi serve per capire se quello che sto facendo sta effettivamente funzionando oppure non sta funzionando proprio assolutamente niente, ti prego funzona pls.\n\nah e questa e' una prova di paragrafo.";
    // char* input_content = "a                        b";
    // char* input_content = "a x x b x x c c iaone come stae"; h_col = 7; w_col = 18; // TODO: controlla quando arriva all'ultimo (devo pensare a com'è fatto il test)

    // FILE* input_file = fopen("lorem_long.txt", "r"); // COMPILATION
    // FILE* input_file = fopen("../lorem_long.txt", "r"); // DEBUG

    FILE* input_file = fopen("utf8_text.txt", "r"); // COMPILATION
    // FILE* input_file = fopen("../utf8_text.txt", "r"); // DEBUG

    if (input_file == NULL) {
        fprintf(stderr, "FILE: TODO ERROR HANDLING\n");
        
        return -1;
    }

    Page* pages = new_page(NULL);

    int exit_code = build_pages(input_file, pages, cols, h_col, w_col);

    switch (exit_code)  {
        case INSUFFICIENT_WIDTH:
            fprintf(stderr, "WIDTH: TODO ERROR HANDLING\n");

            // break;
        case PAGE_SUCCESS:
            printf("\nbegin---\n\n");

            print_pages(pages, spacing, "\n%%%\n\n", ' '); // TODO: se questi sono NULL che succede? ehehe

            break;
        default:
            // unreachable code
            break;
    }

    free_pages(pages); // TODO: dovrebbe funzionare bene, però debugga just to make sure, anche il free di line_chunk_content in page.h

    fclose(input_file); // TODO: gestire l'errore (che me invento?!)

    return 0;
}