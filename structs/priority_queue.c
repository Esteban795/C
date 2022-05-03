#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*
Priority queue, implemented with a min-heap structure
*/

struct PrioQ {
    int last;  // basically, index of the last element
    int* priorities; 
    int* keys;
    int* mapping; // mapping[i] is the index of the element i in priorities/keys tables
    int capacity;
};

typedef struct PrioQ prioq;

int up(int i){ //index of the parent of the node
    return (i - 1)/2;
}

int left(int i){ //index of the left child of the node
    return 2 * i + 1;
}

int right(int i){ //index of the right child of the node
    return 2 * i + 2;
}

int length(prioq* q){
    return q->last + 1;
}

prioq* queue_empty(int size){
    prioq* q = malloc(sizeof(prioq));
    q->last = -1;
    q->priorities = malloc(sizeof(int) * size);
    q->keys = malloc(sizeof(int) * size);
    q->mapping = malloc(sizeof(int) * size);
    q->capacity = size;
    for (int i = 0; i < size; i++){
        q->keys[i] = 0;
        q->mapping[i] = -1;
    }
    return q;
}

int delete_prioq(prioq* q){
    free(q->keys);
    free(q->mapping);
    free(q->priorities);
    free(q);
    return 0;
}

void swap(int* tab,int i,int j){
    int temp = tab[i];
    tab[i] = tab[j];
    tab[j] = temp;
}

void full_swap(prioq* q,int i,int j){
    swap(q->keys,i,j);
    swap(q->priorities,i,j);
    swap(q->mapping,q->keys[i],q->keys[j]);
}

void sift_up(prioq* q,int i){
    int sup = up(i);
    if (i > 0 && q->priorities[i] < q->priorities[sup]){
        full_swap(q,i,sup);
        sift_up(q,sup);
    }
}

void insert(prioq* q,int key,int prio){
    if (length(q) == q->capacity) return;
    else {
        int last_element = q->last + 1;
        q->keys[last_element] = key;
        q->priorities[last_element] = prio;
        q->mapping[key] = last_element;
        q->last = last_element;
        sift_up(q,q->last);
    }
}

void sift_down(prioq* q,int i){
    int mini = i;
    int left_i = left(i);
    int right_i = right(i);
    if (left_i <= q->last && q->priorities[left_i] < q->priorities[i]){
        mini = left_i;
    }
    if (right_i <= q->last && q->priorities[right_i] < q->priorities[i]){
        mini = right_i;
    }
    if (mini != i){
        full_swap(q,i,mini);
        sift_down(q,mini);
    }
}

int* extract_min(prioq* q){
    int* tab = malloc(sizeof(int) * 2); /* key and priority */
    if (q->last < 0) return;
    else {
        int key = q->keys[0];
        int priority = q->priorities[0];
        full_swap(q,0,q->last);
        q->last--;
        sift_down(q,0);
        tab[0] = key;
        tab[1] = priority;
        return tab;
    }
}

void decrease_priority(prioq* q,int elt,int prio){
    int i = q->mapping[elt];
    assert(q->mapping[elt] > 0 && prio < q->priorities[i]);
    q->priorities[i] <- prio;
    sift_up(q,i);
}