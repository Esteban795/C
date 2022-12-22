#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "deque.h"


struct node {
    int num;
    struct node *next;
    struct node *prev;
};

typedef struct node node_t;

struct deque {
    node_t* sentinel;
};

typedef struct deque deque_t;

node_t *new_node(int data){
    node_t *n = malloc(sizeof(node_t));
    n->next = NULL;
    n->prev = NULL;
    n->num = data;
    return n;
}

deque_t *new_deque(void){
    deque_t *q = malloc(sizeof(deque_t));
    q->sentinel = new_node(-1);
    q->sentinel->next = q->sentinel;
    q->sentinel->prev = q->sentinel;
    return q;
}

void free_deque(deque_t *q){
    node_t *n = q->sentinel->next;
    while (n != q->sentinel) {
        node_t *tmp = n->next;
        free(n);
        n = tmp;
    }
    free(q->sentinel);
    free(q);
}

bool try_pop_left(deque_t *q, int *result){
    node_t* left = q->sentinel->next;
    if (left == q->sentinel){
        return false;
    }
    left->prev->next = left->next;
    left->next->prev = left->prev;
    *result = left->num;
    free(left);
    return true;
}

bool try_pop_right(deque_t *q, int *result){
    node_t* right = q->sentinel->prev;
    if (right == q->sentinel){
        return false;
    }
    right->next->prev= right->prev;
    right->prev->next = right->next;
    *result = right->num;
    free(right);
    return true;
}

void push_left(deque_t *q, int data){
    node_t* n = new_node(data);
    n->next = q->sentinel->next;
    q->sentinel->next = n;
    n->prev = q->sentinel;
    n->next->prev = n;
}

void push_right(deque_t *q, int data){
    node_t* n = new_node(data);
    n->prev = q->sentinel->prev;
    q->sentinel->prev = n;
    n->next = q->sentinel;
    n->prev->next = n;
}


