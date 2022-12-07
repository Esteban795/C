#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>

#include "counter.h"
#include "deque.h"
#include "quicksort.h"
#include "global_list.h"

#define MIN_SIZE 1000

struct worker_data {
    deque_t **queues;
    counter_t *nb_tasks;
    int index;
    int nb_workers;
};

typedef struct worker_data worker_data_t;

static bool try_steal(worker_data_t *data, slice_t *slice);


static void process_task(worker_data_t *data, slice_t slice);


static void *worker(void *args);


void parallel_sort_work_stealing(int *arr, int len, int nb_threads);
