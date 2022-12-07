#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include "clock.h"


double SUM = 0.;
pthread_mutex_t LOCK;
pthread_mutex_t* lock = &LOCK;

struct thread_args {
    double *arr;
    int len;
};

typedef struct thread_args thread_args;

void* partial_sum1(void *fun_args){
    thread_args infos = *(thread_args*)fun_args;
    for (int i = 0; i < infos.len;i++){
        SUM += infos.arr[i];
    }
    return NULL;
}

void* partial_sum2(void *fun_args){
    thread_args infos = *(thread_args*)fun_args;
    for (int i = 0; i < infos.len;i++){
        pthread_mutex_lock(lock);
        SUM += infos.arr[i];
        pthread_mutex_unlock(lock);
    }
    return NULL;
}

void* partial_sum3(void* fun_args){
    thread_args infos = *(thread_args*)fun_args;
    double s = 0;
    for (int i = 0; i < infos.len;i++){
        s += infos.arr[i];
    }
    pthread_mutex_lock(lock);
    SUM = s;
    pthread_mutex_unlock(lock);
    return NULL;
}


double *create_array(int len){
    double *arr = malloc(len * sizeof(double));
    for (int i = 0; i < len; i++) {
        arr[i] = sin(i);
    }
    return arr;
}


int main(int argc, char *argv[]){
    pthread_mutex_init(lock,NULL);
    if (argc < 2) return 1;
    int nb_threads = atoi(argv[1]);
    int len_tab = atoi(argv[2]);
    double* arr = create_array(len_tab);
    pthread_t* threads = malloc(sizeof(pthread_t) * nb_threads);
    thread_args* infos = malloc(sizeof(thread_args) * nb_threads);
    int* index = malloc(sizeof(int) * nb_threads);
    for (int i = 0; i < nb_threads;i++){
        index[i] = i;
    }
    for (int i = 0; i < nb_threads;i++){ //initialisation infos
        infos[i].len = len_tab/nb_threads;
        infos[i].arr = &arr[len_tab/nb_threads];
        if (i == nb_threads - 1){
            infos[i].len += len_tab % nb_threads; 
        }
    }
    //manière standard monothread
    double mono_t_sum = 0;
    timestamp st_time = gettime();
    for (int i = 0;i < len_tab;i++){
        mono_t_sum += arr[i];
    }
    timestamp st_time2 = gettime();
    double st_delta = delta_t(st_time,st_time2);
    //

    //partial_sum_1
    SUM = 0;
    timestamp ps1_start = gettime();
    for (int i = 0; i < nb_threads;i++){
        pthread_create(&threads[i],NULL,partial_sum1,&infos[i]);
    }
    for (int i = 0; i < nb_threads; i++){
        pthread_join(threads[i],NULL);
    }
    timestamp ps1_end = gettime();
    double ps1_delta = delta_t(ps1_start,ps1_end);
    double ps1 = SUM;
    //end partial_sum1
    //partial_sum_2
    SUM = 0;
    timestamp ps2_start = gettime();
    for (int i = 0; i < nb_threads;i++){
        pthread_create(&threads[i],NULL,partial_sum2,&infos[i]);
    }
    for (int i = 0; i < nb_threads; i++){
        pthread_join(threads[i],NULL);
    }
    timestamp ps2_end = gettime();
    double ps2_delta = delta_t(ps2_start,ps2_end);
    double ps2 = SUM;

    //end partial_sum_2
    //partial_sum_3
    SUM = 0;
    timestamp ps3_start = gettime();
    for (int i = 0; i < nb_threads;i++){
        pthread_create(&threads[i],NULL,partial_sum3,&infos[i]);
    }
    for (int i = 0; i < nb_threads; i++){
        pthread_join(threads[i],NULL);
    }
    timestamp ps3_end = gettime();
    double ps3_delta = delta_t(ps3_start,ps3_end);
    double ps3 = SUM;
    //end partial_sum_3
    printf("Version normale single-thread: %f en %f\n\n",mono_t_sum,st_delta);
    printf("Version ps1 multithread: %f en %f\n\n",ps1,ps1_delta);
    printf("Version ps2 multithread : %f en %f\n\n",ps2,ps2_delta);
    printf("Version ps3 multithread : %f en %f\n\n",ps3,ps3_delta);
    free(threads);
    free(infos);
    free(index);
    return 0;
}


//gcc sum1.c clock.c -o test -Wall -Wvla -pthread -fsanitize=thread -lm