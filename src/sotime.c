/*
* Grupo 001
* Kethlen Salviano, 51835
* Maria Silva, 40140
* Tiago Badalo, 55311
*/

#include "sotime.h"
#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

#define NANO_TO_MILI(N) ((N)/ 1000000)

void saveStartTime(struct operation *op){
    if(clock_gettime(CLOCK_REALTIME, &(op->start_time)) == -1){
        perror("clock gettime");
        exit(-1);
    }
};

void saveClientTime(struct operation *op){
    if(clock_gettime(CLOCK_REALTIME, &(op->client_time)) == -1){
        perror("clock gettime");
        exit(-1);
    }
};

void saveProxyTime(struct operation *op){
    if(clock_gettime(CLOCK_REALTIME, &(op->proxy_time)) == -1){
        perror("clock gettime");
        exit(-1);
    }
};

void saveServerTime(struct operation *op){
    if(clock_gettime(CLOCK_REALTIME, &(op->server_time)) == -1){
        perror("clock gettime");
        exit(-1);
    }
};

void saveEndTime(struct operation *op){
    if(clock_gettime(CLOCK_REALTIME, &(op->end_time)) == -1){
        perror("clock gettime");
        exit(-1);
    }
}

char* getTime(struct timespec curTime){
    time_t milli;
    struct tm* info;
    char* _timeStr = malloc(sizeof(char)*50);

    milli = NANO_TO_MILI(curTime.tv_nsec);
    info = localtime(&curTime.tv_sec);
    sprintf(_timeStr, "%04d-%02d-%02d %02d:%02d:%02d.%03d", 1900 + info->tm_year, 1 + info->tm_mon, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec, (int)milli);
    return _timeStr;
}