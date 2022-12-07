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
    if ()
}


static void *worker(void *args);


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
