# newspaper

Il programma prende in input un file di testo, e produce un file di output contenente il testo del file di input, incolonnato come una pagina di giornale, secondo le misure definite dall'utente.

Esempio:

```sh
newspaper sample_inputs/example_in.txt 3 5 15 5
```

produce il file contenuto in `expected_outputs/example_out.txt`, ovvero:

```text
Come   si   può     perfettamente       specificati    
vedere              incolonnato   e     dall'utente:   
all'interno  di     giustificato         infatti     per
questo esempio,     secondo       i     produrre questo
il  testo viene     parametri           output  è stato

%%%

scelto       di     caratteri   per     l'altra        
utilizzare    3     riga         di     all'interno  di
colonne     per     colonna,   e  5     una pagina.    
pagina, 5 righe     caratteri   tra                    
per colonna, 15     una  colonna  e     Si noti inoltre

%%%

come         il     UTF-8,        e     originale,     
programma  è in     mantiene     la     inserendo   tra
grado        di     separazione  in     questi una riga
gestire             paragrafi   del      vuota        di
caratteri           testo               separazione. 😃
```

## Compilazione

Per compilare il programma, dopo aver scaricato la repository con

```sh
git clone https://github.com/ph04/newspaper
```

è sufficiente eseguire il comando

```sh
make
```

## Uso

Per usare il programma, seguire le indicazioni fornite da

```sh
newspaper --help
```

## Test

Per eseguire i test, usare il seguente comando:

```sh
sh run_tests.sh
```

## Architettura

TODO: spiega la struttura del programma

## TODO list

- ricontrolla TUTTE le cose da commentare, che adesso è un po un macello
  - manca da commentare la parte che sta in page.c che era "nuova" (non ricordo come funziona)
- gestire gli errori dei processi
- fare questo readme
  - architettura
  - aggiornare con la versione multiprocesso
- mettere la release su github
