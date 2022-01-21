#include <stdio.h>
#include <stdlib.h>

struct Node {
    int value;
    struct Node *prev;
    struct Node *next;
};

typedef struct Node node;

struct DLL_1 {
    node *start;
    node *end;
};

typedef struct DLL_1 dll_1;

node* new_node(int x) {
    node* n = malloc(sizeof(node));
    n->value = x;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

dll_1* new_dll_1(void){
    dll_1* d = malloc(sizeof(dll_1));
    d->start = NULL;
    d->end = NULL;
    return d;
}

void delete_node(node* n,dll_1* d){
    if (n->prev != NULL){
        n->prev->next = n->next;
    } else {
        d->start = n->next;
    }
    if (n->next != NULL) {
        n->next->prev = n->next;
    } else {
        d->end = n->prev;
    }
    free(n);
} 

void insert_before(dll_1* d,node* n,int x){
    node* new = new_node(x);
    new->prev = n->prev;
    new->next = n;
    n->prev = new;
    if (new->prev != NULL) {
        new->prev->next = new;
    } else {
        d->start = new;
    }
}

void insert_after(dll_1* d, node* n, int x){
    node* new = new_node(x);
    new->next = n->next;
    new->prev = n;
    n->next = new;
    if (new->next != NULL){
        new->next->prev = new;
    } else {
        d->end = new;
    }
}

struct DLL{
    struct Node *sentinel;
};

typedef struct DLL dll;

dll* new_dll(void){
    dll* new = malloc(sizeof(dll));
    node* sentinelle = new_node(-1);
    sentinelle->prev = sentinelle;
    sentinelle->next = sentinelle;
    new->sentinel = sentinelle;
    return new;
}

void delete_node_2(node *n){
    n->prev->next = n->next;
    n->next->prev = n->prev;
    free(n);
}

node* insert_before_2(node* n,int x){
    node* new = new_node(x);
    new->next = n;
    new->prev = n->prev;
    n->prev = new;
    new->prev->next = new;
    return new;
}

node* insert_after_2(node* n,int x){
    node* new = new_node(x);
    new->prev = n;
    new->next = n->next;
    n->next = new;
    new->next->prev = new;
    return new;
}

void free_dll_2(dll* d){
    node* n = d->sentinel->prev;
    while (n != d->sentinel) {
        node* temp = n->next;
        free(n);
        n = temp;
    }
    free(d->sentinel);
    free(d);
}

void push_left(dll* d, int x) {
    insert_after_2(d->sentinel,x);
}

void push_right(dll* d, int x){
    insert_before_2(d->sentinel,x);
}

int pop_left(dll* d){
    node* first_elt = d->sentinel->next;
    assert(first_elt != d->sentinel);
    int val = first_elt->value;
    delete_node_2(first_elt);
    return val;
}

int pop_right(dll *d){
    node* last_elt = d->sentinel->prev;
    assert(last_elt != d->sentinel);
    int val = last_elt->value;
    delete_node_2(last_elt);
    return val;
}

dll* from_array(int t[],int len){
    dll* new = new_dll();
    for (int i = 0; i < len; i++){
        push_right(new,t[i]);
    }
    return new;
}