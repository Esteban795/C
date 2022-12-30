#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//T-type stack implementation, using linked lists.
// O(1) push and pop. O(n) space complexity

typedef int T;

struct node {
    T data;
    struct node* next;
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

stack* stack_new(void){
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
    node* n = new_node(data);
    if (s->len > 0){
        node* previous_top = s->top;
        n->next = previous_top;
    }
    s->len++;
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

T stack_peek(stack* s){
    assert(s->len >0);
    return s->top->data;
}

T stack_peek_second(stack* s){
    assert(s->len > 1);
    return s->top->next->data;
}

void add_rec(stack* s,node* n){
    if (n == NULL) return;
    add_rec(s,n->next);
    stack_push(s,n->data);
}

stack* stack_copy(stack* s){
    stack* new_stack = stack_new();
    add_rec(new_stack,s->top);
    return new_stack;
}

void print_rec(node* n){
    if (n != NULL){
        printf("Data : %d\n",n->data);
        print_rec(n->next);
    }
}

void print_stack(stack* s){
    print_rec(s->top);
}


int main(int argc,char* argv[]){
    if (argc != 2) return 1;
    int n = atoi(argv[1]);
    int temp;
    stack* s = stack_new();
    for (int i = 0; i < n;i++){
        scanf("%d",&temp);
        stack_push(s,temp);
    }
    stack* c = stack_copy(s);
    print_stack(c);
    free_stack(c);
    free_stack(s);
    return 0;
}