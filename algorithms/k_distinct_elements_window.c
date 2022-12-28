#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "../utilities/printers.c"

/*
This algorithm allows you to look for the first k-sized window of distincts elements in an array.
Complexity : O(n) time, O(p) space (where p is the number of distincts elements in the array)
*/


struct bornes { //return arguments for main function
    int i;
    int j;
};

typedef struct bornes bornes;

//Set implementation 

const uint8_t empty = 0;
const uint8_t occupied = 1;

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
    uint64_t end = set_end(s);
    while (i < end && s->a[i].status == empty) {
        i++;
    }
    return i;
}

bucket set_get(set* s, uint64_t i){
    return s->a[i];
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
        bucket x = set_get(s, i);
        bool found;
        uint64_t j = set_search(s_new, x.element, &found);
        s_new->a[j].status = occupied;
        s_new->a[j].element = x.element;
        s_new->a[j].occurences = x.occurences;
        s_new->nb_empty--;
    }
    free(s->a);
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
        set_resize(s, s->p + 1);
        j = set_search(s, x, &found);
    }
    s->a[j].status = occupied;
    s->a[j].element = x;
    s->a[j].occurences = 0;
    s->nb_empty--;
}

void set_change_occurence(set* s,T x,int delta){
    bool found;
    int index = set_search(s,x,&found);
    if (found) {
        bucket* tab = s->a;
        tab[index].occurences = tab[index].occurences + delta;
    }
}

int set_get_occs(set* s,T x){
    bool found;
    int index = set_search(s,x,&found);
    return s->a[index].occurences;
}

// set implementation done

bornes* new_bornes(int i,int j){
    bornes* b = malloc(sizeof(bornes));
    b->i = i;
    b->j = j;
    return b;
}

bornes* k_distinct_elements_window(int* arr,int n,int k){
    int i = 0;
    int j = 0;
    int nb_unique = 0;
    set* s = set_new();
    for (int i = 0; i < n;i++){
        set_add(s,arr[i]);
    }
    while (j < n){
        while (j < n && (nb_unique < k || set_get_occs(s,arr[j] > 0))){
            if (set_get_occs(s,arr[j]) == 0) nb_unique++;
            set_change_occurence(s,arr[j],1);
            j++;
        }
        while (nb_unique == k){
            set_change_occurence(s,arr[i],-1);
            if (set_get_occs(s,arr[i]) == 0) nb_unique--;
            i++;
            if (j - i == k){
                return new_bornes(i,j - 1); //[i,j[ interval, so removing 1 closes the interval
            }
        }
    }
    return new_bornes(-1,-1);
}

int main(int argc,char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int* arr = malloc(sizeof(int) * n);
    int t;
    for (int i = 0; i < n;i++){
        scanf("%d",&t);
        arr[i] = t;
    }
    print_tab(arr,n);
    bornes* result = k_distinct_elements_window(arr,n,k);
    printf("Result is : from %d to %d\n\n",result->i,result->j);
    free(arr);
    free(result);
    return 0;
}