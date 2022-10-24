#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "dfa.h"

typedef int class;

struct partitioned_dfa {
    dfa *dfa;
    state *ordered_states;
    class *classes;
    int nb_classes;
};

typedef struct partitioned_dfa partitioned_dfa;

void print_int_array(int arr[], int n){
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


partitioned_dfa *initialize_partition(dfa *a){
    partitioned_dfa* p_dfa = malloc(sizeof(partitioned_dfa));
    int n = a->nb_states;
    p_dfa->nb_classes = 2;
    p_dfa->dfa = a;
    p_dfa->nb_classes = malloc(sizeof(state) * n);
    p_dfa->ordered_states = malloc(sizeof(state) * n);
    int nb_already_accepting = 0;
    int nb_non_accepting = 0;
    for (int q = 0; q < a->nb_states;q++){
        if (a->accepting[q]) {
            p_dfa->ordered_states[n - 1 - nb_already_accepting] = q;
            p_dfa->classes[q] = 1;
            nb_already_accepting++;
        } else {
            p_dfa->ordered_states[nb_non_accepting] = q;
            p_dfa->classes[q] = 0;
            nb_non_accepting++;
        }
    }

    return p_dfa;
}


void zero_out(int t[], int n){
    for (int i = 0; i < n; i++) {
        t[i] = 0;
    }
}

class destination_class(partitioned_dfa *a, state q, letter x){
    state destination_state = a->dfa->delta[q][x];
    return a->classes[destination_state];
}

bool discriminate(partitioned_dfa *a, state q, state r){
    if (a->classes[q] != a->classes[r]) return true;
    for (int lettre = 0; lettre < a->dfa->nb_letters; lettre++){
        if (destination_class(a,q,lettre) != destination_class(a,r,lettre)) return true;
    }
    return false;
}

class *refine(partitioned_dfa *a){
    int nb_states = a->dfa->nb_states;
    class* new_classes = malloc(sizeof(class) * nb_states);
    int i = 0;
    for (int i = 0; i < nb_states; i++){
        new_classes[i] = -1;
    }
    for (state s1 = 0; s1 < nb_states; s1++){
        if (new_classes[s1] >= 0) continue; //l'état est déjà dans une classe d'équivalence, on skip la boucle
        for (state s2 = 0; s2 < nb_states; s2++){
            if (!discriminate(a,s1,s2)) {
                new_classes[s2] = i;
            }
        }
        i++;
    }
}

int *histogram(partitioned_dfa *a, letter x){
    int nb_classes = a->nb_classes;
    int* tab = malloc(sizeof(int) * nb_classes);
    for (int i = 0; i < nb_classes; i++){
        tab[i] = 0;
    }
    for (int i = 0; i < a->dfa->nb_states; i++){
        tab[destination_class(a,a->ordered_states[i],x)]++;
    }
    return tab;
}

void inplace_prefix_sum(int h[], int nb_values){
    int s = 0;
    for (int i = 0; i < nb_values; i++){
        s += h[i];
        h[i] = s;
    }
    return h;
}

void sort_by_transition(partitioned_dfa *a, letter x){
    int n = a->dfa->nb_states;
    int *new_ordered_states = malloc(n * sizeof(state));
    int *hist = histogram(a, x);
    inplace_prefix_sum(hist, a->nb_classes);
    for (int i = 0; i < n; i++) {
        state q = a->ordered_states[i];
        class c = destination_class(a, q, x);
        new_ordered_states[hist[c]] = q;
        hist[c]++;
    }
    free(hist);
    free(a->ordered_states);
    a->ordered_states = new_ordered_states;
}

bool step(partitioned_dfa *a){
    for (letter l = 0; l < a->dfa->nb_letters;l++){
        sort_by_transition(a,l);   
    }
    int n = a->dfa->nb_states;
    class current_class = 0;
    int* new_classes = malloc(sizeof(class) * n);
    new_classes[a->ordered_states[0]] = 0;
    for (int i = 1; i < n;i++){
        state etat = a->ordered_states[i];
        if (discriminate(a,etat,a->ordered_states[i - 1])) current_class++;
        new_classes[etat] = current_class;
    }
    free(a->classes);
    a->classes = new_classes;
    bool diff = (a->nb_classes == current_class + 1);
    a->nb_classes = current_class + 1;
    return diff;
    
}

dfa *to_dfa(partitioned_dfa *a){
    dfa *b = malloc(sizeof(dfa));
    b->nb_states = a->nb_classes;
    b->nb_letters = a->dfa->nb_letters;
    // mark the equivalence classes of the accepting states
    // as accepting
    b->accepting = malloc(b->nb_states * sizeof(bool));
    for (state q = 0; q < a->dfa->nb_states; q++) {
        int c = a->classes[q];
        b->accepting[c] = a->dfa->accepting[q];
    }
    // allocate b->delta
    b->delta = malloc(b->nb_states * sizeof(state*));
    for (class c = 0; c < b->nb_states; c++) {
        b->delta[c] = malloc(b->nb_letters * sizeof(state));
    }
    // fill b->delta using the relation [q].x = [q.x]
    for (state q = 0; q < a->dfa->nb_states; q++) {
        class c = a->classes[q];
        for (letter x = 0; x < b->nb_letters; x++) {
            class c_prime = a->classes[a->dfa->delta[q][x]];
            b->delta[c][x] = c_prime;
        }
    }
    // the initial state of the quotient automataton is [q_I]
    b->initial = a->classes[a->dfa->initial];
    return b;
}


dfa *minimize(dfa *a){
    partitioned_dfa *b = initialize_partition(a);
    while (step(b)) {}
    dfa *a_minimal = to_dfa(b);
    free(b->classes);
    free(b->ordered_states);
    free(b);
    return a_minimal;
}

int main(int argc, char *argv[]){
    return 0;
}
