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

uint64_t hash(uint32_t k,int p) {
    return k & ones(p);
}

set *set_new(void) {
    set *s = malloc(sizeof(set));
    s->p = 1;
    s->a = malloc(2 *sizeof(bucket));
    s->a[0].status = empty;
    s->a[1].status = empty;
    s->nb_empty = 2;
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
        else if (s->a[index].element == x) return true;
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

uint64_t set_next(set *s,uint64_t i) {
    i++;
    printf("I vaut de base : %d\n",i);
    uint64_t end = set_end(s);
    printf("End vaut : %d\n\n",end);
    /*
    while (i < end && s->a[i].status == empty) {
        printf("I vaut : %d\n",i);
        i++;
    }*/
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
        } else if (s->a[index].element == x){
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

void set_resize(set *s,int p) {// Prepare an empty table of size 2**p
    uint64_t m = 1ull << p;
    set *s_new = malloc(sizeof(set));
    s_new->p = p;
    s_new->a = malloc(m *sizeof(bucket));
    for(uint64_t i = 0; i < m; ++i) {
        s_new->a[i].status = empty;
    }
    s_new->nb_empty = m;
    // Add the elements of s to that table
    for (uint64_t i = set_begin(s); i != set_end(s); i = set_next(s, i)) {
        uint32_t x = set_get(s, i);
        bool found;
        uint64_t j = set_search(s_new, x, &found);
        s_new->a[j].status = occupied;
        s_new->a[j].element = x;
        s_new->nb_empty--;
    }
    free(s->a);
    // The next three lines could be replaced with *s = *s_new;
    s->a = s_new->a;
    s->p = p;
    s->nb_empty = s_new->nb_empty;
    free(s_new);
}

void set_add(set *s,uint32_t x) {
    bool found;
    uint64_t j = set_search(s, x, &found);
    if (found) return; 
    uint64_t m = 1ull << s->p;
    if (s->nb_empty <= 1 || 3 * s->nb_empty <= m) {
        printf("avant le resizemmmlmazlma");
        set_resize(s, s->p + 1);
        j = set_search(s, x, &found);
    }
    s->a[j].status = occupied;
    s->a[j].element = x;
    s->nb_empty--;
}

int main() {
    set* s = set_new();  
    set_add(s,10);
    if (set_is_member(s,10)) {
        printf("10 est bien dans le set\n");
    }
    for (int i = 11; i < 100;i++){
        set_add(s,i);
    }
    for (int i = 11; i < 100;i++){
        if (set_is_member(s,i)) {
            printf("%d est bien dans le set\n",i);
        }
    }
    set_delete(s);
    return 0;
}
