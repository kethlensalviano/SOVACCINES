/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "main.h"
#include "main-private.h"
#include "memory.h"
#include "synchronization.h"
#include "process.h"
#include "configuration.h"
#include "sotime.h"
#include "sosignal.h"
#include "log.h"
#include "stats.h"
#include <stdlib.h> 
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

struct main_data* data;
struct communication_buffers* buffers;
struct semaphores* sems;

int main(int argc, char **argv) {


    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("main:");
        exit(-1);
    }
    if (argc != 2){
        printf("Uso: sovaccines nome_ficheiro \n");
        printf("Exemplo: ./bin/sovaccines teste\n");
        return -1;
    }

    data = create_dynamic_memory(sizeof(struct main_data));
    buffers = create_dynamic_memory(sizeof(struct communication_buffers));

    buffers->main_cli = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->cli_prx = create_dynamic_memory(sizeof(struct circular_buffer));
    buffers->prx_srv = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->srv_cli = create_dynamic_memory(sizeof(struct circular_buffer));

    sems = create_dynamic_memory(sizeof(struct semaphores));
    sems->main_cli = create_dynamic_memory(sizeof(struct prodcons));
    sems->cli_prx = create_dynamic_memory(sizeof(struct prodcons));
    sems->prx_srv = create_dynamic_memory(sizeof(struct prodcons));
    sems->srv_cli = create_dynamic_memory(sizeof(struct prodcons));

    
    getContentFile(argv, data);
    
    create_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, buffers);
    create_semaphores(data, sems);

    signal_alarm();

    launch_processes(buffers, data, sems);
 
    user_interaction(buffers, data, sems);
   
    stop_execution(data, buffers, sems);

    return -1;
  
}
 
/* Função que reserva a memória dinâmica necessária para a execução
* do sovaccines, nomeadamente para os arrays *_pids e *_stats da estrutura
* main_data. Para tal, pode ser usada a função create_dynamic_memory do memory.h.
*/
void create_dynamic_memory_buffers(struct main_data* data){

   data->client_pids = create_dynamic_memory(data->n_clients * sizeof(int));
   data->proxy_pids = create_dynamic_memory(data->n_proxies * sizeof(int));
   data->server_pids = create_dynamic_memory(data->n_servers * sizeof(int));
   data->client_stats = create_dynamic_memory(data->n_clients * sizeof(int));
   data->proxy_stats = create_dynamic_memory(data->n_proxies * sizeof(int));
   data->server_stats = create_dynamic_memory(data->n_servers * sizeof(int)); 

}; 

/* Função que reserva a memória partilhada necessária para a execução do
* sovaccines. É necessário reservar memória partilhada para todos os buffers da
* estrutura communication_buffers, incluindo os buffers em si e respetivos
* pointers, assim como para o array data->results e variável data->terminate.
* Para tal, pode ser usada a função create_shared_memory do memory.h.
*/
void create_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers){

    data->results = create_shared_memory(STR_SHM_RESULTS, data->max_ops * sizeof(struct operation));
    data->terminate = create_shared_memory(STR_SHM_TERMINATE, sizeof(int));

    buffers->main_cli->bfr = create_shared_memory(STR_SHM_MAIN_CLI_BUFFER, sizeof(struct operation) * data->buffers_size);
    buffers->cli_prx->bfr = create_shared_memory(STR_SHM_CLI_PRX_BUFFER, sizeof(struct operation) * data->buffers_size);
    buffers->prx_srv->bfr = create_shared_memory(STR_SHM_PRX_SRV_BUFFER, sizeof(struct operation) * data->buffers_size);
    buffers->srv_cli->bfr = create_shared_memory(STR_SHM_SRV_CLI_BUFFER, sizeof(struct operation) * data->buffers_size);

    buffers->main_cli->ptrbool = create_shared_memory(STR_SHM_MAIN_CLI_PTR, sizeof(int) * data->buffers_size);
    buffers->cli_prx->ptr = create_shared_memory(STR_SHM_CLI_PRX_PTR, sizeof(struct ponteiros) * data->buffers_size);
    buffers->prx_srv->ptrbool = create_shared_memory(STR_SHM_PRX_SRV_PTR, sizeof(int) * data->buffers_size);
    buffers->srv_cli->ptr = create_shared_memory(STR_SHM_SRV_CLI_PTR, sizeof(struct ponteiros) * data->buffers_size);

    buffers->cli_prx->ptr->in = 0;
    buffers->cli_prx->ptr->out = 0;

    buffers->srv_cli->ptr->in = 0;
    buffers->srv_cli->ptr->out = 0;

};

/* Função que inicializa os semáforos da estrutura semaphores. Semáforos
* *_full devem ser inicializados com valor 0, semáforos *_empty com valor
* igual ao tamanho dos buffers de memória partilhada, e os *_mutex com valor
* igual a 1. Para tal pode ser usada a função semaphore_create.
*/
void create_semaphores(struct main_data* data, struct semaphores* sems){

    sems->main_cli->full = semaphore_create(STR_SEM_MAIN_CLI_FULL, 0);
    sems->cli_prx->full = semaphore_create(STR_SEM_CLI_PRX_FULL, 0);
    sems->prx_srv->full = semaphore_create(STR_SEM_PRX_SRV_FULL, 0);
    sems->srv_cli->full = semaphore_create(STR_SEM_SRV_CLI_FULL, 0);

    sems->main_cli->mutex = semaphore_create(STR_SEM_MAIN_CLI_MUTEX, 1);
    sems->cli_prx->mutex = semaphore_create(STR_SEM_CLI_PRX_MUTEX, 1);
    sems->prx_srv->mutex = semaphore_create(STR_SEM_PRX_SRV_MUTEX, 1);
    sems->srv_cli->mutex = semaphore_create(STR_SEM_SRV_CLI_MUTEX, 1);

    sems->results_mutex = semaphore_create(STR_SEM_RESULTS_MUTEX, 1);
    
    sems->main_cli->empty = semaphore_create(STR_SEM_MAIN_CLI_EMPTY, data->buffers_size);
    sems->cli_prx->empty = semaphore_create(STR_SEM_CLI_PRX_EMPTY, data->buffers_size);
    sems->prx_srv->empty = semaphore_create(STR_SEM_PRX_SRV_EMPTY, data->buffers_size);
    sems->srv_cli->empty = semaphore_create(STR_SEM_SRV_CLI_EMPTY, data->buffers_size);

};

/* Função que inicia os processos dos clientes, proxies e
* servidores. Para tal, pode usar a função launch_process,
* guardando os pids resultantes nos arrays respetivos
* da estrutura data.
*/
void launch_processes(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){

    for(int i = 0; i < data->n_clients; i++){
        *(data->client_pids + i) = launch_process(i, 0, buffers, data, sems);
    }

    for(int i = 0; i < data->n_proxies; i++){
        *(data->proxy_pids + i) = launch_process(i, 1, buffers, data, sems);
    }

    for(int i = 0; i < data->n_servers; i++){
        *(data->server_pids + i) = launch_process(i, 2, buffers, data, sems);
    }

}; 

/* Função que faz interação do utilizador com o sistema, podendo receber 4 comandos:
* op - cria uma nova operação, através da função create_request
* read - verifica o estado de uma operação através da função read_answer
* stop - termina o execução do sovaccines através da função stop_execution
* help - imprime informação sobre os comandos disponiveis
*/
void user_interaction(struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){

    int connected = 1;
    char input[1024];
    int opcode;
    int op_counter = 0;

    print_menu();

    while(connected == 1){
        usleep(10000);
        printf("Introduzir ação: \n");

        scanf("%s", input);

        opcode = get_opcode(input);

        if(opcode != READ){
            struct timespec time;
            clock_gettime(CLOCK_REALTIME, &time);
            write_log(getTime(time), input);

        }

        switch(opcode){

            case OP:
                create_request(&op_counter, buffers, data, sems);
                break;
            case READ:
                read_answer(data, sems);
                break;
            case STOP:
                connected = -1;
                break;
            case HELP:
                print_menu();
                break;
            default:
                printf("Ação não reconhecida, insira 'help' para assistência. \n");
            break;

        }

    }

};

/* Se o limite de operações ainda não tiver sido atingido, cria uma nova
* operação identificada pelo valor atual de op_counter, escrevendo a mesma
* no buffer de memória partilhada entre main e clientes e efetuando a
* necessária sincronização antes e depois de escrever. Imprime o id da
* operação e incrementa o contador de operações op_counter.
*/
void create_request(int* op_counter, struct communication_buffers* buffers, struct main_data* data, struct semaphores* sems){
    struct operation* op;
    if(data->max_ops > *op_counter){
        op = (struct operation*) malloc(sizeof (struct operation));
        if(op != NULL){
            op->id = *op_counter;
            saveStartTime(op);
            produce_begin(sems->main_cli);
            write_rnd_access_buffer(buffers->main_cli, data->buffers_size, op);
            produce_end(sems->main_cli);
            printf("O pedido #%d foi criado! \n", op->id);
            (*op_counter)++;
            free(op);
        }
    } else {
        printf("O número máximo de pedidos foi alcançado! \n");
    }

};

/* Função que lê um id de operação do utilizador e verifica se a mesma
* é valida e se já foi respondida por um servidor. Em caso afirmativo,
* imprime informação da mesma, nomeadamente o seu estado, e os ids do
* cliente, proxy e servidor que a processaram. O acesso à estrutura
* data->results deve ser sincronizado com as funções e semáforos
* respetivos.
*/
void read_answer(struct main_data* data, struct semaphores* sems){

    int n_op;
    char log[1024];
    char input[512];
    int check = scanf("%d", &n_op);
    
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    
    if(check != 1){
        printf("id de operação fornecido é inválido! \n");
        scanf("%s", input);
        sprintf(log, "%s %s", "read", input);
        write_log(getTime(time), log);

    }else{
        sprintf(input, "%d", n_op);
        sprintf(log, "%s %s", "read", input);
        write_log(getTime(time), log);
        semaphore_mutex_lock(sems->results_mutex);
        if(n_op == (data->results+n_op)->id && (data->results+n_op)->status == 'S'){
            printf("Op %d com estado S foi recebida pelo cliente %d, encaminhada pelo proxy %d, e tratada pelo servidor %d! \n", 
            n_op, (data->results+n_op)->client, (data->results+n_op)->proxy, (data->results+n_op)->server);

        } else {
            printf("Op %d ainda não é válido! \n", n_op);        
        }

        semaphore_mutex_unlock(sems->results_mutex);

    }


};

/* Função que termina a execução do programa sovaccines. Deve começar por
* afetar a flag data->terminate com o valor 1. De seguida, e por esta
* ordem, deve acordar processos adormecidos, esperar que terminem a sua
* execução, escrever as estatisticas finais do programa, e por fim libertar
* os semáforos e zonas de memória partilhada e dinâmica previamente
*reservadas. Para tal, pode usar as outras funções auxiliares do main.h.
*/
void stop_execution(struct main_data* data, struct communication_buffers* buffers, struct semaphores* sems){

    *(data->terminate) = 1;

    wakeup_processes(data,sems);

    wait_processes(data);

    write_statistics_file();

    write_statistics(data);

    destroy_semaphores(sems);

    destroy_dynamic_memory_buffers(data);

    destroy_shared_memory_buffers(data,buffers);

    free(data->log_filename);
    free(data->statistics_filename);
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_cli); 
    destroy_dynamic_memory(buffers->cli_prx); 
    destroy_dynamic_memory(buffers->prx_srv); 
    destroy_dynamic_memory(buffers->srv_cli);
    destroy_dynamic_memory(buffers);
    destroy_dynamic_memory(sems->main_cli);
    destroy_dynamic_memory(sems->cli_prx);
    destroy_dynamic_memory(sems->prx_srv);
    destroy_dynamic_memory(sems->srv_cli);
    destroy_dynamic_memory(sems);

}; 

/* Função que acorda todos os processos adormecidos em semáforos, para que
* estes percebam que foi dada ordem de terminação do programa. Para tal,
* pode ser usada a função produce_end sobre todos os conjuntos de semáforos
* onde possam estar processos adormecidos e um número de vezes igual ao
* máximo de processos que possam lá estar.
*/
void wakeup_processes(struct main_data* data, struct semaphores* sems){

    for(int i = 0; i < data->n_clients; i++){     
        produce_end(sems->srv_cli);
        produce_end(sems->main_cli);     
    }

    for(int i = 0; i < data->n_proxies; i++){
        produce_end(sems->cli_prx);
    }

    for(int i = 0; i < data->n_servers; i++){
        produce_end(sems->prx_srv);
    } 

};

/* Função que espera que todos os processos previamente iniciados terminem,
* incluindo clientes, proxies e servidores. Para tal, pode usar a função
* wait_process do process.h.
*/
void wait_processes(struct main_data* data){

    for(int i = 0; i < data->n_clients; i++){
        data->client_stats[i] = wait_process(*(data->client_pids)+i);
    }

    for(int i = 0; i < data->n_proxies; i++){
        data->proxy_stats[i] = wait_process(*(data->proxy_pids)+i);
    }

    for(int i = 0; i < data->n_servers; i++){
        data->server_stats[i] = wait_process(*(data->server_pids)+i);
    }

}; 

/* Função que imprime as estatisticas finais do sovaccines, nomeadamente quantas
* operações foram processadas por cada cliente, proxy e servidor.
*/
void write_statistics(struct main_data* data){

    printf("\nTerminado o sovaccines! Imprimindo estatísticas: \n");

    for(int i = 0; i < data->n_clients; i++){

        printf("Cliente %d recebeu %d pedidos! \n", i, *(data->client_stats + i));

    }

    for(int i = 0; i < data->n_proxies; i++){

        printf("Proxy %d encaminhou %d pedidos! \n", i, *(data->proxy_stats + i));

    }
    
    for(int i = 0; i < data->n_servers; i++){

        printf("Server %d respondeu %d pedidos! \n", i, *(data->server_stats + i)) ;

    }   

};

/* Função que liberta todos os buffers de memória dinâmica previamente
* reservados na estrutura data.
*/
void destroy_dynamic_memory_buffers(struct main_data* data){

   destroy_dynamic_memory(data->client_pids);
   destroy_dynamic_memory(data->proxy_pids);
   destroy_dynamic_memory(data->server_pids);
   destroy_dynamic_memory(data->client_stats);
   destroy_dynamic_memory(data->proxy_stats);
   destroy_dynamic_memory(data->server_stats); 

};

/* Função que liberta todos os buffers de memória partilhada previamente
* reservados nas estruturas data e buffers.
*/
void destroy_shared_memory_buffers(struct main_data* data, struct communication_buffers* buffers){

    destroy_shared_memory(STR_SHM_RESULTS, data->results, data->max_ops * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));
    
    destroy_shared_memory(STR_SHM_MAIN_CLI_PTR, buffers->main_cli->ptrbool, data->buffers_size);
    destroy_shared_memory(STR_SHM_CLI_PRX_PTR, buffers->cli_prx->ptr, data->buffers_size);
    destroy_shared_memory(STR_SHM_PRX_SRV_PTR, buffers->prx_srv->ptrbool, data->buffers_size);
    destroy_shared_memory(STR_SHM_SRV_CLI_PTR, buffers->srv_cli->ptr, data->buffers_size);

    destroy_shared_memory(STR_SHM_MAIN_CLI_BUFFER, buffers->main_cli->bfr, sizeof(struct rnd_access_buffer));
    destroy_shared_memory(STR_SHM_CLI_PRX_BUFFER, buffers->cli_prx->bfr, sizeof(struct circular_buffer));
    destroy_shared_memory(STR_SHM_PRX_SRV_BUFFER, buffers->prx_srv->bfr, sizeof(struct rnd_access_buffer));
    destroy_shared_memory(STR_SHM_SRV_CLI_BUFFER, buffers->srv_cli->bfr, sizeof(struct circular_buffer));

};

/* Função que liberta todos os semáforos da estrutura semaphores.
*/
void destroy_semaphores(struct semaphores* sems){

    semaphore_destroy(STR_SEM_MAIN_CLI_FULL, sems->main_cli->full);
    semaphore_destroy(STR_SEM_CLI_PRX_FULL, sems->cli_prx->full);
    semaphore_destroy(STR_SEM_PRX_SRV_FULL, sems->prx_srv->full);
    semaphore_destroy(STR_SEM_SRV_CLI_FULL, sems->srv_cli->full);

    semaphore_destroy(STR_SEM_MAIN_CLI_MUTEX, sems->main_cli->mutex);
    semaphore_destroy(STR_SEM_CLI_PRX_MUTEX, sems->cli_prx->mutex);
    semaphore_destroy(STR_SEM_PRX_SRV_MUTEX, sems->prx_srv->mutex);
    semaphore_destroy(STR_SEM_SRV_CLI_MUTEX, sems->srv_cli->mutex);

    semaphore_destroy(STR_SEM_RESULTS_MUTEX, sems->results_mutex);
    
    semaphore_destroy(STR_SEM_MAIN_CLI_EMPTY, sems->main_cli->empty);
    semaphore_destroy(STR_SEM_CLI_PRX_EMPTY, sems->cli_prx->empty);
    semaphore_destroy(STR_SEM_PRX_SRV_EMPTY, sems->prx_srv->empty);
    semaphore_destroy(STR_SEM_SRV_CLI_EMPTY, sems->srv_cli->empty);

};
