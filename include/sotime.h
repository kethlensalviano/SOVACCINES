/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#ifndef SOTIME_H_GUARD
#define SOTIME_H_GUARD

#include "memory.h"

void saveStartTime(struct operation* op);

void saveClientTime(struct operation* op);

void saveProxyTime(struct operation* op);

void saveServerTime(struct operation* op);

void saveEndTime(struct operation* op);

/*
* Funcao que devolve o tempo passado como argumento no formato: YYYY-MM-DD HH:MM:SS.MS
*/
char* getTime(struct timespec curTime);

#endif