#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


//Linked list implementation for queues.
typedef int vertex;

struct cell {
    vertex data;
    struct cell* next;
};

typedef struct cell cell;

struct queue {
    cell* left;
    cell* right;
    int len;
};

typedef struct queue queue;

cell* cell_new(vertex v){
    cell* c = malloc(sizeof(cell));
    c->data = v;
    c->next = NULL;
    return c;
}

queue* queue_create(void) {
    queue* q = malloc(sizeof(queue));
    q->len = 0;
    q->left = NULL;
    q->right = NULL;
    return q;
}

int queue_length(queue* q){
    return q->len;
}

void queue_push(queue* q,vertex v){
    cell* c = cell_new(v);
    if (q->len == 0) {
        q->right = c;
        q->left = c;
    } else {
        q->right->next = c;
        q->right = c;
    }
    q->len++;
}

vertex queue_pop(queue* q){
    assert(q->len > 0);
    vertex result = q->left->data;
    if (q->len == 1){
        free(q->left);
        q->left = NULL;
        q->right = NULL;
    } else {
        cell* temp = q->left->next;
        free(q->left);
        q->left = temp; 
    }
    q->len--;
    return result;
}

void cell_free(cell* c){
    if (c == NULL) return;
    else {
        cell* next = c->next;
        free(c);
        cell_free(next);
    }
}

void queue_free(queue* q){
    cell_free(q->left);
}