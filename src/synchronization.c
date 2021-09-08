/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "synchronization.h"
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>        

	
/* Função que cria um novo semáforo com nome name e valor inicial igual a
* value. Pode concatenar o resultado da função getuid() a name, para tornar
* o nome único para o processo.
*/
sem_t * semaphore_create(char* name, int value){

    sem_t *new_sem;

    char name_uid[strlen(name) + 10];
	int uid = getuid();
	sprintf(name_uid,"/%s_%d", name, uid);

    new_sem = sem_open(name_uid, O_CREAT, 0xFFFFFFFF, value);

    if (new_sem == SEM_FAILED) {
        perror(name_uid);
        exit(6);
    }
    return new_sem;

};

/* Função que destroi o semáforo passado em argumento.
*/
void semaphore_destroy(char* name, sem_t* semaphore){

    char name_uid[strlen(name) + 10];
	int uid = getuid();
	sprintf(name_uid,"/%s_%d", name, uid);

    if (sem_close(semaphore)==-1){
        perror(name_uid);
        exit(3);
    }

    if (sem_unlink(name_uid)==-1){
        perror(name_uid);
        exit(3);
    }
};

/* Função que inicia o processo de produzir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_begin(struct prodcons* pc){
    if(sem_wait(pc->empty) == -1){
        perror("wait");
        exit(3);
    }
    
    if(sem_wait(pc->mutex) == -1){
        perror("wait");
        exit(3);
    }
};

/* Função que termina o processo de produzir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void produce_end(struct prodcons* pc){
    if(sem_post(pc->mutex) == -1) {
        perror("post");
        exit(3);
    }
    if(sem_post(pc->full) == -1) {
        perror("post");
        exit(3);       
    }
};

/* Função que inicia o processo de consumir, fazendo sem_wait nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_begin(struct prodcons* pc){
    if(sem_wait(pc->full) == -1){
        perror("wait");
        exit(3);
    }
    
    if(sem_wait(pc->mutex) == -1){
        perror("wait");
        exit(3);
    }
};

/* Função que termina o processo de consumir, fazendo sem_post nos semáforos
* corretos da estrutura passada em argumento.
*/
void consume_end(struct prodcons* pc){
    if(sem_post(pc->mutex) == -1) {
        perror("post");
        exit(3);
    }
    if(sem_post(pc->empty) == -1) {
        perror("post");
        exit(3);       
    }
};

/* Função que faz wait a um semáforo.
*/
void semaphore_mutex_lock(sem_t* sem){
    if(sem_wait(sem) == -1){
        perror("wait");
        exit(3);
    }
};

/* Função que faz post a um semáforo.
*/
void semaphore_mutex_unlock(sem_t* sem){
    if(sem_post(sem) == -1) {
        perror("post");
        exit(3);       
    }
};
