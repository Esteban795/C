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

int main(int argc,char* argv[]){
    if (argc != 2) return 1;
    int n = atoi(argv[1]);
    int temp;
    stack* s = new_stack();
    for (int i = 0; i < n;i++){
        scanf("%d",&temp);
        stack_push(s,temp);
    }
    for (int i = 0; i < n;i++){
        printf("%d\n",stack_pop(s));
    }
    free_stack(s);
    return 0;
}