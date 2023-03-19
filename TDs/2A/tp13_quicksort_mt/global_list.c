#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#include "counter.h"
#include "deque.h"
#include "quicksort.h"
#include "global_list.h"
#include "clock.h"
#include "utils.h"

#define MIN_SIZE 1000

struct global_data {
    deque_t *queue;
    counter_t *nb_tasks;
};

typedef struct global_data global_data_t;


static void process_task(global_data_t *data, slice_t slice){
    if (slice.len < MIN_SIZE) {
        full_sort(slice.start, slice.len);
        decrement(data->nb_tasks);
        return;
    }
    int pivot = partition(slice.start, slice.len);
    slice_t s_lo = {.start = slice.start,
                    .len = pivot};
    slice_t s_hi = {.start = slice.start + pivot + 1,
                    .len = slice.len - pivot - 1};
    increment(data->nb_tasks);
    if (s_hi.len > s_lo.len) {
        push_right(data->queue, s_hi);
        process_task(data, s_lo);
    } else {
        push_right(data->queue, s_lo);
        process_task(data, s_hi);
    }
}


static void *worker(void *args){
    global_data_t *data = (global_data_t*)args;
    deque_t *q = data->queue;
    counter_t *nb_tasks = data->nb_tasks;
    while (get_counter(nb_tasks) != 0) {
        slice_t t;
        if (try_pop_left(q, &t)) {process_task(data, t);}
    }
    return NULL;
}


void parallel_sort_global(int *arr, int len, int nb_threads){
    pthread_t *threads = malloc(nb_threads * sizeof(pthread_t));
    deque_t *queue = new_deque();
    counter_t *nb_tasks = initialize_counter(1);
    global_data_t args = {.queue = queue, .nb_tasks = nb_tasks};
    slice_t initial = {.start = arr, .len = len};
    push_right(queue, initial);
    for (int i = 0; i < nb_threads; i++) {
        pthread_create(&threads[i], NULL, worker, &args);
    }
    for (int i = 0; i < nb_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    free_deque(queue);
    free(threads);
    destroy_counter(nb_tasks);
}


int main(int argc,char* argv[]){
    printf("départ\n\n");
    if (argc < 2) return 1;
    int len = atoi(argv[1]);
    int nb_threads = atoi(argv[2]);
    int* random = rand_array(len);
    timestamp start = gettime();
    parallel_sort_global(random,len,nb_threads);
    timestamp end = gettime();
    print_array(random,len);
    printf("Tableau de %d éléments trié en %f\n\n",len,delta_t(start,end));
    printf("Dernier élément du tableau : %d\n",random[len - 1]);
    return 1;
}
