#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Comme uint64_t est un peu pénible à taper, on utilise
// un typedef :
typedef uint64_t ui;

#define HEAP_SIZE 32
uint64_t block_size = 8;

ui heap[HEAP_SIZE];

ui heap_index(ui* p){
    return p - heap;
}



void init_heap(void) {
    heap[0] = 1;
}

ui* malloc_ui(uint64_t size) {
    if (size + heap[0] > HEAP_SIZE) {
        return NULL;
    }
    heap[0] += size;
    return &heap[heap[0] - size];
}

void free_ui(uint64_t* p) {}

void init_heap2(void) {
    heap[0] = 2;
}

bool is_free(uint64_t i) {
    if (heap[i] - 1 == 0) {
        return true;
    }
    return false;
}

void set_free(uint64_t i) {
    heap[i - 1] = 0;
}

void set_used(uint64_t i) {
    heap[i - 1] = 1;
}

ui* malloc_ui_2(uint64_t taille) {
    if (taille > block_size) {
        return NULL;
    }
    uint64_t k = 2;
    while (k < HEAP_SIZE) {
        if (heap[k - 1] == 0) {
            if (k >= heap[0]) {
                heap[0] = k + block_size - 1;
            }
            heap[k - 1] = 1;
            return &heap[k];
        }
        k += block_size;
    }
    heap[0] += taille;
    heap[heap[0] - taille  - 1] = 1;
    return &heap[heap[0] - taille];
}

void free_ui_2(uint64_t i) {
    int temp = heap_index(&heap[i]);
    if (temp + block_size + 1 > HEAP_SIZE) {
        heap[0] = temp;
    } else {
        heap[i - 1] = 0;
    }
}
// Cette fonction convertit un pointeur (qui doit être issu de
// malloc_ui) en un indice dans le tableau heap.
// Vous en aurez besoin pour écrire les différentes versions
// de free_ui (juste un appel au début, ensuite on ne manipule plus
// que des indices), mais il est complètement normal de ne pas
// comprendre comment elle fonctionne : c'est de l'arithmétique des
// pointeurs, dont je n'ai pas encore parlé.


// Cette fonction initialise le tas à une valeur particulière, que
// vous avez peu de chance d'utiliser par hasard. Cela nous
// permettra en pratique de repérer les cases dont la valeur n'a
// jamais été modifiée quand on affiche le contenu du tas.
// Elle est destinée à être appelée une unique fois, tout au début
// de l'exécution du programme.
void pre_initialize_heap(void){
    for (ui i = 0; i < HEAP_SIZE; i++){
        heap[i] = 0xFFFFFFFF;
    }
}

// La fonction suivante affiche le contenu du tas. Les cases
// identifiées comme n'ayant jamais été modifiées sont affichées
// de manière particulière.
void print_heap(void){
    for (ui i = 0; i < HEAP_SIZE; i++){
        ui x = heap[i];
        if (x == 0xFFFFFFFF){
            printf("... ");
        } else {
            printf("%3llu ", (long long unsigned)x);
        }
    }
    printf("\n");
}

void set_memory(ui* p, ui size, ui value){
    for (ui i = 0; i < size; i++){
        p[i] = value;
    }
}


int main(void){
    init_heap2();
    ui* p1 = malloc_ui_2(8);
    set_memory(p1,block_size - 1,42);
    ui* p2 = malloc_ui_2(8);
    set_memory(p2,block_size - 1,69);
    print_heap();
    //free_ui_2(heap_index(p1));
    printf("\n\n");
    print_heap();
    printf("\n\n");
    ui* p3 = malloc_ui_2(8);
    set_memory(p3,block_size - 1,10);
    print_heap();
    return 0;
}
