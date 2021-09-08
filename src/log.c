/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "log.h"
#include "main.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

extern struct main_data* data;
void write_log(char* _timeStr, char* _input){

    FILE *ficheiro;
    int r;
    ficheiro = fopen(data->log_filename, "a+");
    if (ficheiro == NULL)
    {
        perror("cria_file()");
        exit(1);
    }
        if(strcmp(_input, "stop") == 0 || strcmp(_input, "SIGINT") == 0){
            r = fprintf(ficheiro, "%s %s\n---------------------------------------------- \n", _timeStr, _input);

        } else {
            r = fprintf(ficheiro, "%s %s\n", _timeStr, _input);

        }
        if (r < 0)
        {
            perror("Erro na escrita do ficheiro de logs");
        }

    fclose(ficheiro);

    free(_timeStr);

}