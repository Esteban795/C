#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <clock.h>

#define NB_THREADS 2
#define MAXIMUM 1000000000
struct petersen{
    bool want[2];
    bool turn;
};

struct thread_args{
    double* counter;
    petersen* mutex;
    bool which_one;
};

typedef struct petersen petersen;
typedef struct thread_args thread_args;


petersen* init(void){
    petersen* lock = malloc(sizeof(petersen));
    lock->turn = false;
    lock->want[0] = false;
    lock->want[1] = false;
    return lock;
}

void lock(petersen* mutex,bool thread){
    int other = thread == 0 ? 1 : 0;
    int current = thread = 0 ? 0 : 1;
    mutex->want[current] = true;
    mutex->turn = 1 - current;
    while (mutex->turn = other && mutex->want[other]) {}
}

void unlock(petersen* mutex,bool thread){
    mutex->want[thread == 1 ? 1 : 0] = false;
}

double *create_array(int len){
    double *arr = malloc(len * sizeof(double));
    for (int i = 0; i < len; i++) {
        arr[i] = sin(i);
    }
    return arr;
}

void* count(void* args){
    thread_args* infos = (thread_args*)args;
    if (infos->counter < MAXIMUM){
        lock(infos->mutex,infos->which_one);
        infos->counter++;
        unlock(infos->mutex,infos->which_one);
    }
    return NULL;
}


void main(int argc,char* argv[]){
    int counter = 0;
    if (argc < 2) return EXIT_FAILURE;
    int maxi = atoi(argv[1]);
    petersen* mutex = 0;
    pthread_t* threads = malloc(sizeof(pthread_t) * NB_THREADS);
    thread_args* infos = malloc(sizeof(thread_args) * NB_THREADS);
    for (int i = 0; i < NB_THREADS;i++){
        infos[i].mutex = mutex;
        infos[i].counter = &counter;
        infos[i].which_one = false;
    }
    infos[1].which_one = true;
    for (int i = 0; i < NB_THREADS; i++){
        pthread_create(&threads[i],NULL,count,&infos[i]);
    }
    for (int i = 0; i < NB_THREADS; i++){
        pthread_join(threads[i],NULL);
    }
}