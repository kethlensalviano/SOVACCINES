/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "memory.h"
#include "memory-private.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


/* Função que reserva uma zona de memória partilhada com tamanho indicado
* por size e nome name, preenche essa zona de memória com o valor 0, e 
* retorna um apontador para a mesma. Pode concatenar o resultado da função
* getuid() a name, para tornar o nome único para o processo.
*/
void* create_shared_memory(char* name, int size){

    int *ptr;
    int trun;
    char uid [50];
    snprintf(uid, 50, "%d", getuid());
    int fd = shm_open(strcat(uid, name), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    if(fd == -1){
        perror(name);
        exit(1);
    }

    trun = ftruncate(fd, size);

    if(trun == -1){
        perror(name);
        exit(1);
    }

    ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd , 0);

    if(ptr == MAP_FAILED){
        perror(strcat(name, "-mmap"));
        exit(1);
    }

    *ptr = 0;

    return ptr;
};


/* Função que reserva uma zona de memória dinâmica com tamanho indicado
* por size, preenche essa zona de memória com o valor 0, e retorna um 
* apontador para a mesma.
*/
void* create_dynamic_memory(int size){
    
    void *ptr, *ftr;

    ptr = malloc(size);

    ftr = memset(ptr, 0, size);

    return ftr;

};


/* Função que liberta uma zona de memória dinâmica previamente reservada.
*/
void destroy_shared_memory(char* name, void* ptr, int size){

    int ret;

    ret = munmap(ptr, size);

    if(ret == -1){
        perror(name);
        exit(1);
    }

    char uid [50];
    snprintf(uid, 50, "%d", getuid());
    ret = shm_unlink(strcat(uid, name)); 

    if(ret == -1){
        perror(name);
        exit(1);
    }
};


/* Função que liberta uma zona de memória partilhada previamente reservada.
*/
void destroy_dynamic_memory(void* ptr){

    free(ptr); // liberta 
};


/* Função que escreve uma operação num buffer de acesso aleatório. A
* operação deve ser escrita numa posição livre do buffer, segundo as
* regras de escrita em buffers de acesso aleatório. Se não houver nenhuma
* posição livre, não escreve nada.
*/
void write_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){

    int n;

    for(n = 0; n < buffer_size; n++){

        if(*(buffer->ptrbool + n) == 0){

            *(buffer->bfr + n) = *op;                              

            *(buffer->ptrbool + n) = 1;

            break;
        }
    }

};


/* Função que escreve uma operação num buffer circular. A operação deve 
* ser escrita numa posição livre do buffer, segundo as regras de escrita
* em buffers circulares. Se não houver nenhuma posição livre, não escreve
* nada.
*/
void write_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){

    if(buffer->ptr->out == buffer_size){

        op->id = -1;

    } else {
         

        while(((buffer->ptr->in + 1) % buffer_size) == (buffer->ptr->out));

        *(buffer->bfr + (buffer->ptr->in)) = *op;

        (buffer->ptr->in) = ((buffer->ptr->in) + 1) % buffer_size;
    }

};


/* Função que lê uma operação de um buffer de acesso aleatório, se houver
* alguma disponível para ler. A leitura deve ser feita segundo as regras
* de leitura em buffers acesso aleatório. Se não houver nenhuma operação
* disponível, afeta op com o valor -1.
*/
void read_rnd_access_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){

    int n;

    for(n = 0; n < buffer_size; n++){

        if(*(buffer->ptrbool) == 1){

            *op = *(buffer->bfr+n);

            *(buffer->ptrbool+n) = 0;

            break;
        }
    }

    if(n == buffer_size){
        op->id = -1;
    }

};


/* Função que lê uma operação de um buffer circular, se houver alguma 
* disponível para ler. A leitura deve ser feita segundo as regras de
* leitura em buffers circular. Se não houver nenhuma operação disponível,
* afeta op->id com o valor -1.
*/
void read_circular_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){

    if(buffer->ptr->out == buffer_size){

        op->id = -1;

    } else {

        while(buffer->ptr->in == buffer->ptr->out);
                    
        *op = *(buffer->bfr + buffer->ptr->out);

        buffer->ptr->out = (buffer->ptr->out + 1) % buffer_size;

    }
};