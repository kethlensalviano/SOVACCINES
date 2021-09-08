/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#ifndef MEMORY_PRIVATE_H_GUARD

#define MEMORY_PRIVATE_H_GUARD

//estrutura que representa um buffer circular
struct circular_buffer {     
    struct operation* bfr;
    struct ponteiros* ptr;
    
};

//estrutura que representa os ponteiros de um buffer circular
struct ponteiros{
    int in;
    int out;
};

//estrutura que representa um buffer de acesso aleatório
struct rnd_access_buffer {         
    struct operation* bfr;
    int* ptrbool;

};

#endif
