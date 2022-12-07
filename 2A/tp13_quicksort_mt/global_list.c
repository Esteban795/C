#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>

#include "counter.h"
#include "deque.h"
#include "quicksort.h"
#include "global_list.h"

#define MIN_SIZE 1000

struct global_data {
    deque_t *queue;
    counter_t *nb_tasks;
};

typedef struct global_data global_data_t;


static void process_task(global_data_t *data, slice_t slice){
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
            push_right(data->queue,s_hi);
            process_task(data,s_lo);
        } else {
            push_right(data->queue,s_lo);
            process_task(data,s_hi);
        }
    }
}


static void* worker(void *args){
    global_data_t* data = (global_data_t*)args;
    while (get_counter(data->nb_tasks) != 0){
        slice_t t;
        if (try_pop_left(data->queue,&t)) {
            process_task(data,t);
        }
    }
    return NULL;
}

void parallel_sort_global(int *arr, int len, int nb_threads){
    pthread_t threads[nb_threads];
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
}
