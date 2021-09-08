/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "configuration.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

void getContentFile(char* argv[], struct main_data* data){

    char str[100];
    char *ptr;
    FILE *fptr = fopen(argv[1], "r");
    int i = 0;
    if(fptr == NULL){
        perror("O ficheiro não existe");
        exit(EXIT_FAILURE);
    }
    else{
        fgets(str, 99, fptr);
        ptr = strtok(str, " ");       

        while(i < 8)  {
            if(i == 0){
                data->max_ops = atoi(ptr);               
            }
            else if(i == 1){
                data->buffers_size = atoi(ptr);               
            }
            else if(i == 2){
                data->n_clients = atoi(ptr);
            }
            else if(i == 3){
                data->n_proxies = atoi(ptr);          
            }
            else if(i == 4){
                data->n_servers = atoi(ptr);                
            }
            else if(i == 5){
                data->log_filename = strdup(ptr);                    
            }
            else if(i == 6){
                
               data->statistics_filename = strdup(ptr);            
            }
            else{
                data->alarm_time = (time_t) atoi(ptr);
            }
        
            ptr = strtok(NULL, " ");
            i++;
        }
        fclose(fptr);      
    }
    
};

