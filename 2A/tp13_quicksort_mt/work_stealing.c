#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>


#include "counter.h"
#include "deque.h"
#include "quicksort.h"
#include "clock.h"
#include "utils.h"

#define MIN_SIZE 1000

struct worker_data {
    deque_t **queues;
    counter_t *nb_tasks;
    int index;
    int nb_workers;
};

typedef struct worker_data worker_data_t;

static bool try_steal(worker_data_t *data, slice_t *slice){
    int index = data->index;
    int total_workers = data->nb_workers;
    int i = (index + 1) % total_workers;
    while (i != index){
        if (try_pop_left(data->queues[i], slice)) return true;
        i = (i + 1) % total_workers;
    }
    return false;
}

static void process_task(worker_data_t *data, slice_t slice){
    if (slice.len < MIN_SIZE){
        full_sort(slice.start,slice.len);
        decrement(data->nb_tasks);
        return;
    } else {
        int pivot = partition(slice.start,slice.len);
        slice_t s_lo = {.start = slice.start,.len = pivot};
        slice_t s_hi = {.start = slice.start + pivot + 1,.len = slice.len - pivot - 1};
        increment(data->nb_tasks);
        if (s_lo.len < s_hi.len){
            push_right(data->queues[data->index],s_hi);
            process_task(data,s_lo);
        } else {
            push_right(data->queues[data->index],s_lo);
            process_task(data,s_hi);
        }
    }
}


static void *worker(void *args){
    worker_data_t w_data = *(worker_data_t*)args;
    while (get_counter(w_data.nb_tasks) != 0){
        slice_t t;
        if (try_pop_right(w_data.queues[w_data.index],&t)) { //on a dit que les plus grosses tâches étaient à gauche
            process_task(&w_data,t);
        } else if (try_steal(&w_data,&t)) {
            process_task(&w_data,t);
        }
    }
    return NULL;
}


void parallel_sort_work_stealing(int *arr, int len, int nb_threads){
    pthread_t* threads = malloc(sizeof(pthread_t) * nb_threads);
    deque_t** queues = malloc(nb_threads * sizeof(deque_t*));
    worker_data_t* args = malloc(sizeof(worker_data_t) * nb_threads);
    counter_t* nb_tasks = initialize_counter(1);
    slice_t initial = {.start = arr, .len = len};
    for (int i = 0; i < nb_threads; i++){
        queues[i] = new_deque();
        args[i].index = i;
        args[i].nb_tasks = nb_tasks;
        args[i].nb_workers = nb_threads;
        args[i].queues = queues;
    }
    push_right(queues[0],initial);
    for (int i = 0; i < nb_threads;i++){
        pthread_create(&threads[i],NULL,worker,&args[i]);
    }
    for (int i = 0; i < nb_threads;i++){
        pthread_join(threads[i],NULL);
    }
    for (int i = 0; i < nb_threads;i++){
        free_deque(queues[i]);
    }
    free(threads);
    free(queues);
    free(args);
    destroy_counter(nb_tasks);
}

int main(int argc,char* argv[]){
    printf("départ\n\n");
    if (argc < 2) return 1;
    int len = atoi(argv[1]);
    int nb_threads = atoi(argv[2]);
    int* random = rand_array(len);
    timestamp start = gettime();
    parallel_sort_work_stealing(random,len,nb_threads);
    timestamp end = gettime();
    print_array(random,len);
    printf("Tableau de %d éléments trié en %f avec %d threads\n\n",len,delta_t(start,end),nb_threads);
    printf("Dernier élément du tableau : %d\n",random[len - 1]);
    return 1;
}

//gcc work_stealing.c counter.c deque.c quicksort.c clock.c utils.c -o test_work_stealing -Wall -Wextra -Wvla -pthread