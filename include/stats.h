/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "sotime.h"

/*
* Funcao que calcula o tempo total decorrido entre o start e end.
*/
double total_time(struct timespec start, struct timespec end);

/*
* Funcao que escreve as estatisticas de processos e operacoes num ficheiro de estatisticas.
*/
void write_statistics_file();

#endif