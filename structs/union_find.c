#include <stdlib.h>
#include <stdint.h>

struct partition {
    int nb_sets;
    int nb_elements;
    int *arr;
};

typedef struct partition partition_t;
typedef int element;

int nb_sets(partition_t *p){
    return p->nb_sets;
}

int nb_elements(partition_t *p){
    return p->nb_elements;
}

partition_t* partition_new(int nb_elements){
    partition_t* p = malloc(sizeof(partition_t));
    p->nb_elements = nb_elements;
    p->arr = malloc(sizeof(int) * nb_elements);
    p->nb_sets = nb_elements;
    for (int i = 0; i < nb_elements;i++){
        p->arr[i] = -1;
    }
    return p;
}

void partition_free(partition_t* p){
    free(p->arr);
    free(p);
}

element find(partition_t* p,element x){
    if (p->arr[x] < 0) return x;
    int root = find(p,p->arr[x]);
    p->arr[x] = root;
    return root;
}

void merge(partition_t* p, element x, element y){
    element root_x = find(p,x);
    element root_y = find(p,y);
    if (p->arr[root_x] >= p->arr[root_y]) {
        p->arr[root_y] += p->arr[root_x];
        p->arr[root_x] = root_y;
    } else if (p->arr[root_x] < p->arr[root_y]) {
        p->arr[root_y] = root_x;
        p->arr[root_x] += p->arr[root_y];
    }
    p->nb_sets--;
}