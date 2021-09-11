/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "main-private.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* 
* Função que verifica se algum dos inputs eh 0 e se sim, devolve 0, se não
* devolve 1.
*/
int validateInputs(int argc, char ** argv){

    int check = 1;

    for(int i = 1; i<argc; ++i){
        if(atoi(argv[i]) == 0){
            check = 0;
        }

    }
    return check;

}

/*
* Imprime o menu do sovaccines
*/
void print_menu(){

    printf("\nAvailable actions: \n");
    printf("\t op – create vaccine request \n");
    printf("\t read – checks the status of a request (specified by the user) \n");
    printf("\t stop – terminates SOVACCINES execution \n");
    printf("\t help – shows menu options and descriptions \n");

}

/*
* Devolve o opcode corresponde à operação colocada pelo utilizador. Se não existir, devolve -1.
*/
int get_opcode(char* operacao){
    int opcode = 0;
    if(strcmp(operacao, "op") == 0){
        opcode = OP;
    }else if(strcmp(operacao, "read") == 0){
        opcode = READ;
    }else if(strcmp(operacao, "stop") == 0){
        opcode = STOP;
    }else if(strcmp(operacao, "help") == 0){
        opcode = HELP;
    } else {
         opcode = -1;
    }
    return opcode;

}