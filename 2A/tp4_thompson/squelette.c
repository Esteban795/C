#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


#define EPS 256
#define ALL 257
#define MATCH 258

#define MAX_LINE_LENGTH 1024
struct state {
    int c;
    struct state *out1;
    struct state *out2;
    int last_set;
};

typedef struct state state_t;

struct nfa {
    state_t *start;
    state_t *final;
    int n;
};

typedef struct nfa nfa_t;

struct stack {
    int length;
    int capacity;
    nfa_t *data;
};

typedef struct stack stack_t;

struct set {
    int length;
    int id;
    state_t **states;
};

typedef struct set set_t;

state_t *new_state(int c, state_t *out1, state_t *out2){
    state_t* new_st = malloc(sizeof(state_t));
    new_st->c = c;
    new_st->out1 = out1;
    new_st->out2 = out2;
    new_st->last_set = -1;
    return new_st;
}

nfa_t character(char c){
    state_t* final = new_state(MATCH,NULL,NULL);
    state_t* initial = new_state(c,final,NULL);
    nfa_t automate;
    automate.final = final;
    automate.start = initial;
    automate.n = 2;
    return automate;
}

nfa_t all(void){
    nfa_t automate;
    state_t* final = new_state(ALL,NULL,NULL);
    state_t* initial = new_state(MATCH,final,NULL);
    automate.final = final;
    automate.start = initial;
    automate.n = 2;
    return automate;
}

nfa_t concat(nfa_t a, nfa_t b){
    a.final->c = EPS;
    a.final->out1 = b.start;
    a.final = b.final;
    a.n += b.n;
    return a;
}

nfa_t alternative(nfa_t a, nfa_t b){
    nfa_t automate;
    state_t* start = new_state(EPS,a.start,b.start);
    state_t* final = new_state(MATCH,NULL,NULL);
    a.final->c = EPS;
    b.final->c = EPS;
    a.final->out1 = final;
    b.final->out1 = final;
    automate.final = final;
    automate.start = start;
    automate.n = a.n + b.n + 2;
    return automate;
}

nfa_t star(nfa_t a){
    nfa_t automate;
    state_t* start = new_state(EPS,a.start,NULL);
    state_t* final =  new_state(MATCH,NULL,NULL);
    start->out2 = final;
    a.final->c = EPS;
    start->out1 = a.start;
    a.final->out1 = final;
    a.final->out2 = a.start;
    automate.start = start;
    automate.final = final;
    automate.n = a.n + 2;
    return automate;
}

nfa_t maybe(nfa_t a){
    state_t* start = new_state(EPS,a.start,a.final);
    nfa_t automate;
    automate.start = start;
    automate.final = a.final;
    automate.n = a.n + 1;
    return automate;
}

stack_t *stack_new(int capacity){
    stack_t *s = malloc(sizeof(stack_t));
    s->data = malloc(capacity * sizeof(nfa_t));
    s->capacity = capacity;
    s->length = 0;
    return s;
}

void stack_free(stack_t *s){
    free(s->data);
    free(s);
}

nfa_t pop(stack_t *s){
    assert(s->length > 0);
    nfa_t result = s->data[s->length - 1];
    s->length--;
    return result;
}

void push(stack_t *s, nfa_t a){
    assert(s->capacity > s->length);
    s->data[s->length] = a;
    s->length++;
}

nfa_t build(char *regex){
    int size = strlen(regex);
    stack_t* s = stack_new(size);
    for (int i = 0; i < size; i++){
        char c = regex[i];
        nfa_t b;
        nfa_t a;
        switch (c)
        {
        case '@':
            b = pop(s);
            a = pop(s);
            push(s,concat(a,b));
            break;
        case '|':
            b = pop(s);
            a = pop(s);
            push(s,alternative(a,b));
            break;
        case '*':
            a = pop(s);
            push(s,star(a));
            break;
        case '?':
            a = pop(s);
            push(s,maybe(a));
            break;
        case '.':
            push(s,all());
            break;
        default:
            push(s,character(s));
            break;
        }
    }
    assert(s->length = 1);
    nfa_t result = pop(s);
    stack_free(s);
    return result;

}

bool backtrack(state_t *state, char *s){
    if (state == NULL) return false;
    if (state->c == EPS) {
        return backtrack(state->out1,&s[1]) || backtrack(state->out2,&s[1]);
    }
    if (s[0] == '\0' || s[0] == "\n") return state->c == MATCH;
    if (s[0] == state->c || state->c == ALL){
        return backtrack(state->out1,&s[1]);
    }
    return false;
}

bool accept_backtrack(nfa_t a, char *s){
    return backtrack(a.start, s);
}

void match_stream_backtrack(nfa_t a, FILE *in){
    char *line = malloc((MAX_LINE_LENGTH + 1) * sizeof(char));
    while (true) {
        if (fgets(line, MAX_LINE_LENGTH, in) == NULL) break;
        if (accept_backtrack(a, line)) {
            printf("%s", line);
        }
    }
    free(line);
}


set_t *empty_set(int capacity, int id){
    state_t **arr = malloc(capacity * sizeof(state_t*));
    set_t *s = malloc(sizeof(set_t));
    s->length = 0;
    s->id = id;
    s->states = arr;
    return s;
}

void set_free(set_t *s){
    free(s->states);
    free(s);
}


void add_state(set_t *set, state_t *s){
    if (s == NULL || s->last_set == set->id) return;
    s->last_set = set->id;
    set->states[set->length] = s;
    set->length++;
    if (s->c == EPS) {
        add_state(set,s->out1);
        add_state(set,s->out2);
    }
}

void step(set_t *old_set, char c, set_t *new_set){
    new_set->id = old_set->id + 1;
    for (int i = 0; i < old_set->length; i++){
        state_t* s = old_set->states[i];
        if (s->c == c || s->c == ALL){
            add_state(new_set,s->out1);
        }
    }
}

bool accept(nfa_t a, char *s, set_t *s1, set_t *s2){
    s1->length = 0;
    add_state(s1, a.start);
    int i = 0;
    while (s[i] != '\0'&& s[i] != '\n') {
        step(s1, s[i], s2);
        set_t *tmp = s1;
        s1 = s2;
        s2 = tmp;
        i++;
    }
    return a.final->last_set == s1->id;
}

void match_stream(nfa_t a, FILE *in);

int main(int argc, char* argv[]){
    assert(argc >= 2);
    FILE* in_f = stdin;
    if (argc = 3) {
        in_f = fopen(argv[2],"r");
    }
    nfa_t a = build(argv[1]);
    match_stream_backtrack(a,in_f);
    if (argc >= 3 ) fclose(in_f);
    return 0;
}
