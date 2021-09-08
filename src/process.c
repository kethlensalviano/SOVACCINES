/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "process.h"
#include "memory.h"
#include "main.h"
#include "client.h"
#include "server.h"
#include "proxy.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

/* Função que inicia um processo através da função fork do SO. O novo
* processo pode ser um cliente, proxy, ou servidor, consoante a variável
* process_code tem valor 0, 1, ou 2 respectivamente, e irá executar a função
* execute_* respetiva, fazendo exit do retorno. O processo pai devolve o pid
* do processo criado.
*/
int launch_process(int process_id, int process_code, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){

   int pid = fork();

   if(pid == -1){
       perror("Houve um erro na criação de um processo");
       exit(1);
   }

   if(pid == 0){
        if(process_code == 0){
            exit(execute_client(process_id, buffers, data, sems));    
        } else if(process_code == 1){
            exit(execute_proxy(process_id, buffers, data, sems));
        } else{
            exit(execute_server(process_id, buffers, data, sems));
        }
   }

   return pid;
};


/* Função que espera que um processo termine através da função waitpid. 
* Devolve o retorno do processo, se este tiver terminado normalmente.
*/
int wait_process(int process_id){

    int wstatus;
    waitpid(process_id, &wstatus, 0);

    if(WIFEXITED(wstatus)){
        return WEXITSTATUS(wstatus);
    } else{
        return -1;
    }
};