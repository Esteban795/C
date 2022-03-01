#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const uint8_t empty = 0;
const uint8_t occupied = 1;
const uint8_t tombstone = 2;

typedef uint32_t T;
struct bucket {
    uint8_t status;
    uint32_t element;
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
uint64_t hash(T x,int p){
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

bool set_is_member(set* s, T x){
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

T set_get(set* s, uint64_t i){
    return s->a[i].element;
}

uint64_t set_search(set* s,T x, bool *found){
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


bool new_set_is_member(set* s, T x){
    bool found;
    set_search(s,x,&found);
    return found;
}

void set_resize(set *s, int p){
    set* new_set = malloc(sizeof(set));
    new_set->p = p;
    new_set->a = malloc(sizeof(bucket) * set_end(s));
    for (uint64_t i = 0; i < set_end(s);i++){
        s->a[i].status = empty;
    }
    for (uint64_t i = set_begin(s); i < set_end(s);set_next(s,i)){
        T x = set_get(s,i);
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

void set_add(set* s,T x){
    uint64_t i = hash(x,s->p);
    bool found;
    uint64_t j = set_search(s,x,&found);
    if (found) return; //déjà dans le tableau
    uint64_t size = set_end(s); 
    if (s->nb_empty <= 1){ //plus de place
        set_resize(s, 1 + s->p); 
        j = set_search(s,x,&found); //modifie l'indice où on devrait donc placer x
    }
    s->a[j].status = occupied;
    s->a[j].element = x;
    s->nb_empty--;
}

uint64_t new_set_search(set* s,T x,bool* found){
    uint64_t index = hash(x,s->p);
    uint64_t i_tombstone = set_end(s);

    while (true) {
        if (s->a[index].status == empty){
            *found = false;
            if (i_tombstone == set_end(s)) return index;
            else return i_tombstone;

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
/*
set *set_example() {
    set *s = malloc(sizeof(set));
    s->p = 2;
    s->a = malloc(4 * sizeof(bucket));
    s->a[0].status = occupied;
    s->a[0].element = 1492;
    s->a[1].status = occupied;
    s->a[1].element = 1939;
    s->a[2].status = empty;
    s->a[3].status = occupied;
    s->a[3].element = 1515;
    s->nb_empty = 1;
    return s;
}
*/





/*
uint32_t *read_data(char *filename, int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL;
    }

    int nb_lines = 0;
    char line[16];
    while (!feof(file)) {
        fscanf(file, "%15s\n", line);
        nb_lines++;
    }
    rewind(file);

    uint32_t *t = malloc(nb_lines * sizeof(uint32_t));
    int a, b, c, d;
    for (int i = 0; i < nb_lines; ++i) {
        int nb_read = fscanf(file, "%d.%d.%d.%d", &a, &b, &c, &d);
        if (nb_read != 4){
            fclose(file);
            free(t);
            return NULL;
        }
        t[i] = (((a * 256u) + b) * 256u + c) * 256u + d;
    }

    fclose(file);

    *n = nb_lines;
    return t;
}
*/


int main() {
    set* s = set_new();    
    s->a[0].status = occupied;
    s->a[0].element = 1492;
    s->nb_empty = 1;
    set_resize(s,4);
    set_delete(s);
    return 0;
}
