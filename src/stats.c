/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "stats.h"
#include "main.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

extern struct main_data* data;
extern struct communication_buffers* buffers;
extern struct semaphores* sems;

double total_time(struct timespec start, struct timespec end) {
    double dif = (end.tv_sec - start.tv_sec) 
            + (end.tv_nsec - start.tv_nsec) 
            / 1000000000.0;
    return dif;
};

void write_statistics_file() {
    
    FILE *ficheiro;

    ficheiro = fopen(data->statistics_filename, "a+");
    if (ficheiro == NULL)
    {
        perror("cria_file()");
        exit(1);
    }

    if(fputs("Process Statistics: \n", ficheiro) < 0) {
        printf("Erro na escrita das estatisticas\n");
    }

    for(int i = 0; i < data->n_clients; i++){
        if(fprintf(ficheiro, "\tClient %d received %d requests! \n", i, *(data->client_stats + i)) < 0) {
            printf("Erro na escrita das estatisticas\n");
        }
    }

    for(int i = 0; i < data->n_proxies; i++){
        if(fprintf(ficheiro, "\tProxy %d forwarded %d requests! \n", i, *(data->proxy_stats + i)) < 0) {
            printf("Erro na escrita das estatisticas\n");
        }
    }
    
    for(int i = 0; i < data->n_servers; i++){
        if(fprintf(ficheiro, "\tServer %d responded to %d requests! \n", i, *(data->server_stats + i)) < 0) {
            printf("Erro na escrita das estatisticas\n");
        }
    }

    if(fputs("\nOperation Statistics: \n", ficheiro) < 0) {
        printf("Erro na escrita das estatisticas\n");
    }

    for (int i = 0; i < data->max_ops; i++) {
        if ((data->results+i) != NULL && (data->results+i)->status == 'S') {
            
            char* start = getTime((data->results+i)->start_time);
            char* client_time = getTime((data->results+i)->client_time);
            char* proxy_time = getTime((data->results+i)->proxy_time);
            char* server_time = getTime((data->results+i)->server_time);
            char* end = getTime((data->results+i)->end_time);

            fprintf(ficheiro, "\nOP: %d \nStatus: %c \nClient_id: %d \nProxy_id: %d \nServer_id: %d \nCreated: %s \nClient_time: %s \nProxy_time: %s \nServer_time: %s \nEnded: %s \nTotal Time: %f \n", 
                i, (data->results+i)->status, (data->results+i)->client, (data->results+i)->proxy, (data->results+i)->server, 
                start, client_time, proxy_time, server_time, end, total_time((data->results+i)->start_time, (data->results+i)->end_time));

            free(start);
            free(client_time);
            free(proxy_time);
            free(server_time);
            free(end);

        }
    }
    if(fputs("-----------------------------------------------------------------------\n", ficheiro) < 0) {
        printf("Erro na escrita das estatisticas\n");
    }

    fclose(ficheiro);

};

