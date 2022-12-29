#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//T-type stack implementation, using linked lists.
// O(1) push and pop. O(n) space complexity

typedef int T;

struct node {
    T data;
    node* next;
};

typedef struct node node;

struct stack {
    int len;
    node* top;
};

typedef struct stack stack;

node* new_node(T d){
    node* n = malloc(sizeof(node));
    n->data = d;
    n->next = NULL;
    return n;
}

stack* new_stack(void){
    stack* s = malloc(sizeof(stack));
    s->len = 0;
    s->top = NULL;
    return s;
}

void free_list(node* n){
    if (n == NULL) return;
    free_list(n->next);
    free(n);
}

void free_stack(stack* s){
    free_list(s->top);
    free(s);
}

void stack_push(stack* s,T data){
    s->len++;
    node* n = new_node(data);
    if (s->len > 0){
        node* previous_top = s->top;
        n->next = previous_top;
    }
    s->top = n;
}

T stack_pop(stack* s){
    assert(s->len > 0);
    node* top = s->top;
    T result = top->data;
    s->top = top->next;
    s->len = s->len - 1;
    free(top);
    return result;
}

