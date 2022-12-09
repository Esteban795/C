#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>

#include "deque.h"


struct node {
    slice_t slice;
    struct node *next;
    struct node *prev;
};

typedef struct node node_t;

const slice_t DEFAULT = {.start = NULL, .len = 0};

struct deque {
    node_t *sentinel;
    pthread_mutex_t lock;
};

typedef struct deque deque_t;

node_t *new_node(slice_t data){
    node_t *n = malloc(sizeof(node_t));
    n->next = NULL;
    n->prev = NULL;
    n->slice = data;
    return n;
}

deque_t *new_deque(void){
    deque_t *q = malloc(sizeof(deque_t));
    q->sentinel = new_node(DEFAULT);
    q->sentinel->next = q->sentinel;
    q->sentinel->prev = q->sentinel;
    pthread_mutex_init(&q->lock, NULL);
    return q;
}

void free_deque(deque_t *q){
    pthread_mutex_lock(&q->lock);
    node_t *n = q->sentinel->next;
    while (n != q->sentinel) {
        node_t *tmp = n->next;
        free(n);
        n = tmp;
    }
    free(q->sentinel);
    pthread_mutex_unlock(&q->lock);
    pthread_mutex_destroy(&q->lock);
    free(q);
}

bool try_pop_left(deque_t *q, slice_t *result){
    pthread_mutex_lock(&q->lock);
    node_t* left = q->sentinel->next;
    if (left == q->sentinel){
        pthread_mutex_unlock(&q->lock);
        return false;
    }
    left->prev->next = left->next;
    left->next->prev = left->prev;
    *result = left->slice;
    free(left);
    pthread_mutex_unlock(&q->lock);
    return true;
}

bool try_pop_right(deque_t *q, slice_t *result){
    pthread_mutex_lock(&q->lock);
    node_t* right = q->sentinel->prev;
    if (right == q->sentinel){
        pthread_mutex_unlock(&q->lock);
        return false;
    }
    right->next->prev= right->prev;
    right->prev->next = right->next;
    *result = right->slice;
    free(right);
    pthread_mutex_unlock(&q->lock);
    return true;
}

void push_left(deque_t *q, slice_t data){
    pthread_mutex_lock(&q->lock);
    node_t* n = new_node(data);
    n->next = q->sentinel->next;
    q->sentinel->next = n;
    n->prev = q->sentinel;
    n->next->prev = n;
    pthread_mutex_unlock(&q->lock);
}

void push_right(deque_t *q, slice_t data){
    pthread_mutex_lock(&q->lock);
    node_t* n = new_node(data);
    n->prev = q->sentinel->prev;
    q->sentinel->prev = n;
    n->next = q->sentinel;
    n->prev->next = n;
    pthread_mutex_unlock(&q->lock);
}


