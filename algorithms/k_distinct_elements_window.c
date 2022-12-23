#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

struct bornes { //return arguments for main function
    int i;
    int j;
};

typedef struct bornes bornes;


//Implementation of a set, with number of occurences of an item seen.

const uint8_t empty = 0;
const uint8_t occupied = 1;

struct bucket {
    uint8_t status;
    uint32_t element;
    int occurences;
};
typedef struct bucket bucket;

struct set {
    int p;
    bucket *a;
    uint64_t nb_empty;
};
typedef struct set set;

uint64_t ones(int p){
    return (1ull << p) - 1ull;
}
uint64_t hash(int x,int p){
    return x & ones(p);
}

set* set_new(void){
    set* s = malloc(sizeof(set));
    s->p = 1;
    s->a = malloc(sizeof(bucket) * 2);
    s->nb_empty = 2;
    s->a[0].status = empty;
    s->a[1].status = empty;
    return s;
}

void set_delete(set* s){
    free(s->a);
    free(s);
}

bool set_is_member(set* s, int x){
    uint64_t index = hash(x,s->p);
    while (true) {
        if (s->a[index].status == empty) return false;
        if (s->a[index].element == x) return true;
        index += 1;
        index = index & ones(s->p);
    }
}

uint64_t set_end(set* s){
    return 1ull << s->p;
}

uint64_t set_begin(set* s){
    uint64_t i = 0;
    while (i < set_end(s) && s->a[i].status == empty){
        i++;
    }
    return i;
}

uint64_t set_next(set* s,uint64_t i){
    i++;
    while (s->a[i].status == empty && i < (1ull << s->p)){
        i++;
    }
    return i;
}

int set_get(set* s, uint64_t i){
    return s->a[i].element;
}

uint64_t set_search(set* s,int x, bool *found){
    uint64_t index = hash(x,s->p);
    while (true) {
        if (s->a[index].status == empty){
            *found = false;
            return index;
        } 
        if (s->a[index].element == x){
            *found = true;
            return index;
        }
        index += 1;
        index = index & ones(s->p);
    }
}

void set_resize(set *s, int p){
    set* new_set = malloc(sizeof(set));
    new_set->p = p;
    new_set->a = malloc(sizeof(bucket) * set_end(s));
    for (uint64_t i = 0; i < set_end(s);i++){
        s->a[i].status = empty;
    }
    for (uint64_t i = set_begin(s); i < set_end(s);set_next(s,i)){
        int x = set_get(s,i);
        bool found;
        uint64_t k = set_search(new_set,x,&found);
        new_set->a[k].status = occupied;
        new_set->a[k].element = x;
        new_set->nb_empty--;
    }
    free(s->a);
    *s = *new_set;
    free(new_set);
}

void set_add(set* s,int x){
    uint64_t i = hash(x,s->p);
    bool found;
    uint64_t j = set_search(s,x,&found);
    if (found) {
        s->a[i].occurences++; //déjà dans le tableau donc on augmente son occurence
    } 
    uint64_t size = set_end(s); 
    if (s->nb_empty <= 1){ //plus de place
        set_resize(s, 1 + s->p); 
        j = set_search(s,x,&found); //modifie l'indice où on devrait donc placer x
    }
    s->a[j].status = occupied;
    s->a[j].element = x;
    s->nb_empty--;
}

void set_change_occurence(set* s,int x,int n){
    uint64_t h = hash(x,s->p);
    bool exists;
    if (set_is_member(s,x)){
        s->a[h].occurences += n;
    }
}

// set implementation

bornes* new_bornes(int i,int j){
    bornes* b = malloc(sizeof(bornes));
    b->i = i;
    b->j = j;
    return b;
}

bornes* k_distinct_elements_window(int* arr,int n,int k){
    int i = 0;
    int j = 0;
    int dist = 0;
    set* occurences = set_new();
    bool found;
    while (j < n){ 
        while (dist == k){
            set_change_occurence(occurences,arr[i],-1);
            if (set_is_member(occurences,arr[i])) {dist--;}
            i++;
        }
        while (j < n && (dist < k || set_is_member(occurences,arr[j]))){
            if (!set_is_member(occurences,arr[j])) {dist++;}
            set_change_occurence(occurences,arr[j],1);
            j++;
        }
        if (dist == k){
            return new_bornes(i,j);
        }
    }

}

int main(int argc,char* argv){
    if (argc != 3) return EXIT_FAILURE;
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    

    return 0;
}