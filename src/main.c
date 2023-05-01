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
    // char* input_content = "Sistemi Operativi II Modulo. Progetto (Homework).\n\nL’obiettivo è implementare un programma C che trasformi un testo in italiano da una colonna a più colonne su più pagine (come ad es. per una pagina di quotidiano). Sono richieste due versioni del programma: una a singolo processo e una multi-processo con almeno tre processi concorrenti.\n\nDati in ingresso e requisiti: un file di testo in codifica Unicode (UTF-8) contenente un testo in italiano strutturato in paragrafi separati da una o più linee vuote (stringa contenente solo il carattere ‘\n’). Ogni paragrafo è  costituito da una o più stringhe terminate da ‘\n’ (il testo in ingresso è tutto su una sola colonna); il numero di colonne per pagina su cui distribuire il testo; il numero di linee per ogni colonna; larghezza di ogni colonna (numero di caratteri per colonna); la distanza in caratteri tra una colonna di testo e l’altra.\n\nDati in uscita e requisiti: un file di testo ASCII contenente il testo di input incolonnato e impaginato secondo i parametri espressi dall’utente. Le pagine sono separate dalla stringa “\n %%% \n”; ogni paragrafo è separato da una linea vuota; la prima colonna di testo è a sinistra; tutte le righe, tranne l’ultima, di ogni paragrafo dovranno essere allineate ad entrambi i margini della colonna; l’ultima riga di ogni paragrafo è solo allineata a sinistra; le parole in una linea dovranno essere separate da almeno un carattere di spazio; la sillabazione di eventuali parole troppo lunghe non è necessaria.\n\nRequisiti generali del progetto: ogni file .c/.h  dovrà essere ben commentato: per ogni funzione commentare brevemente i parametri di ingresso/uscita e il suo funzionamento generale; nel corpo di ogni funzione commentare le linee di codice più importanti; fornire un makefile per compilare il progetto con un semplice make; fornire un file di testo README con una breve spiegazione dei file inclusi e un breve manuale utente; la versione multi-processo del programma dovrà essere costituita da almeno tre processi concorrenti ed intercomunicanti: uno per leggere il file di ingresso, uno per creare le colonne, ed infine uno per la scrittura del file di output. (Ulteriori suddivisioni del carico di lavoro in più di tre processi sono ammesse.) NON è ammesso l’uso di librerie esterne con funzioni per la manipolazione di stringhe o testo che non siano quelle standard del C. Eccezione: è ammesso l’uso di librerie esterne per l’analisi delle opzioni della linea di comando per il vostro programma.\n\nSuggerimenti: usare nomi di variabili e funzioni corrispondenti al loro significato/utilizzo; usare stdin e stdout per rispettivamente il testo in ingresso e in uscita; sviluppare prima la versiosne mono-processo del programma, poi quella multi-processo; per l’analisi della linea di comando si consiglia getopt, di uso molto semplice e che fa chunke della libreria GNU standard del C; concentratevi prima sulla correttezza dei programmi, cioè assicuratevi con più testi di input che i programmi producano un output incolonnato secondo le richieste. Se avete tempo alla fine, ottimizzate il codice per uso di CPU e/o RAM, mantenendone la correttezza; potete usare wc per verificare che il numero delle parole in ingresso e in uscita dei vostri programmi sia lo stesso (usando stdin e stdout nel vostri programmi, questo semplice test si riduce ad una sola linea di comando con l’operatore pipe | della shell).\n\nIl punteggio massimo è 6/30, articolato come segue: correttezza del programma (3/30); architettura del programma e commenti (2/30); usabilità e istruzioni utente (1/30).";
    // char* input_content = "lorem ipsum dolor sit amet"; h_col = 4; w_col = 5;
    // char* input_content = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.xxxxxxxxxxxx\n\n   Mauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.\n\nLorem ipsum dolor sit amet, consectetur adipiscing elit. Quisque euismod mauris ac malesuada sagittis. Morbi imperdiet ullamcorper tortor, eget varius sem ultrices a. Nam molestie dignissim lectus, et dapibus nunc tristique in. Sed eget neque egestas, aliquam lacus non, rhoncus ex. Suspendisse metus orci, gravida eget gravida ullamcorper, accumsan dapibus justo. Donec elit libero, ullamcorper id interdum et, porttitor non metus. In ultrices non arcu tincidunt scelerisque. Morbi at lectus quis nunc faucibus mattis id et ex. Ut viverra nunc arcu, at mollis orci ultricies non. Etiam tempor euismod neque eget imperdiet. Fusce faucibus sem velit, at pellentesque odio sagittis vel. Duis hendrerit dui sed maximus suscipit. Praesent tristique quam erat, eu mollis ante malesuada a. Phasellus rutrum tempus lorem, vel vulputate ante lobortis in. Nam lacinia ut arcu vitae suscipit.\n\nMauris volutpat purus placerat lacus vestibulum, sit amet bibendum arcu tincidunt. Fusce volutpat orci in ex suscipit euismod. Donec mi nibh, blandit vitae ligula non, cursus fermentum dolor. Mauris sit amet venenatis nisi. Curabitur quis dolor at metus consectetur congue. Ut velit nunc, ultrices sed convallis eu, posuere a arcu. Vivamus tristique finibus sapien, vitae lacinia ante dapibus eget. Praesent libero nisl, efficitur vel ex in, semper tempor erat. Aliquam pharetra mi nec nisi varius, nec sollicitudin ligula pulvinar. \n\n    Curabitur bibendum arcu quis erat tincidunt, sed tempus ex placerat. Donec ornare, tellus pulvinar pellentesque elementum, arcu ante pharetra metus, ut malesuada metus nibh ut orci. Aenean pulvinar nisi arcu, a pharetra mi ornare nec.";
    // char* input_content = "ciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene      mi      sento veramente fortissimo ciao a tutti.       \n\n      questa e' una prova di paragrafo, speriamo non esploda tutto quanto.\n\nciao a tutti sono il capobastone come va spero tutto bene, io sto molto bene mi sento veramente fortissimo ciao a tutti quanti.      \n\n      questo e' una prova di paragrafo, speriamo non esploda tutto qua    nto.";
    // char* input_content = "questo e' un super test che mi serve per capire se quello che sto facendo sta effettivamente funzionando oppure non sta funzionando proprio assolutamente niente, ti prego funzona pls.\n\nah e questa e' una prova di paragrafo.";
    // char* input_content = "a                        b";
    // char* input_content = "a x x b x x c c iaone come stae"; h_col = 7; w_col = 18; // TODO: controlla quando arriva all'ultimo (devo pensare a com'è fatto il test)

    FILE* input_file = fopen("lorem_long.txt", "r"); // COMPILATION
    // FILE* input_file = fopen("../lorem_long.txt", "r"); // DEBUG

    if (input_file == NULL) {
        fprintf(stderr, "FILE: TODO ERROR HANDLING\n");
        
        return -1;
    }

    Page* pages = new_page(NULL);

    int exit_code = build_pages(pages, input_file, cols, h_col, w_col);

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