/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#ifndef MAIN_PRIVATE_H_GUARD
#define MAIN_PRIVATE_H_GUARD

#define OP 10
#define READ 20
#define STOP 30
#define HELP 40

int validateInputs(int argc, char ** argv);

void print_menu();

int get_opcode(char* operacao);

#endif