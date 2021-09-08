/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#ifndef LOG_H_GUARD
#define LOG_H_GUARD

/*
* Funcao que escreve no ficheiro de log a data e a operacao realizada pelo utilizador.
* Apanha também a operação ctrl+c como SIGINT.
*/
void write_log(char* _timeStr, char* _input);

#endif