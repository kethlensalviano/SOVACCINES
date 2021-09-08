/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "sosignal.h"
#include "main.h"
#include "sotime.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>

extern struct main_data* data;
extern struct communication_buffers* buffers;
extern struct semaphores* sems;
extern int counter;

void signal_handler(){

    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    write_log(getTime(time), "SIGINT");
    stop_execution(data, buffers,sems);

    exit(0);

};

void child_handler(){
    exit(counter);
};

void signal_alarm() {
    struct itimerval val;
    signal(SIGALRM, alarm_handler);

    val.it_interval.tv_sec = data->alarm_time;
    val.it_interval.tv_usec = 0;
    val.it_value.tv_sec = data->alarm_time;
    val.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &val, 0);
    
};

void alarm_handler() {
    signal(SIGALRM, alarm_handler);

    semaphore_mutex_lock(sems->results_mutex);
    for(int i = 0; i < data->max_ops; i++) {

        if ((data->results+i) != NULL && (data->results+i)->status == 'S') {
            printf("op: %d status: S start: %ld client: %d client_time: %ld proxy: %d proxy_time: %ld server: %d server_time: %ld end: %ld \n", 
            i, (data->results+i)->start_time.tv_sec, (data->results+i)->client, (data->results+i)->client_time.tv_sec, (data->results+i)->proxy, (data->results+i)->proxy_time.tv_sec, (data->results+i)->server, (data->results+i)->server_time.tv_sec, (data->results+i)->end_time.tv_sec);
        }  else {
            printf("op: %d status: 0 \n", i);
        }
    }
    semaphore_mutex_unlock(sems->results_mutex);

};