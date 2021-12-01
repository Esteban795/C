#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/***************/
/*  CONSIGNES  */
/***************/

// Compilez avec les options -Wall -Wextra -Wvla -fsanitize=address

// Les fonctions sont déjà *déclarées*, mais pas *définies*
// (sauf exceptions).
// À chaque fois, il faut ajouter le corps de la fonction,
// après avoir supprimé le point-virgule situé à la fin de
// la déclaration.


/* Partie 2 */

struct int_array {
    int len;
    int* data;
};

typedef struct int_array int_array;

int_array* array_create(int len, int x){

    int_array* t = malloc(sizeof(int_array));
    int* data = (int*)malloc(len * sizeof(int));

    for (int i = 0; i < len; i++){
        data[i] = x;
    }
    t->len = len;
    t->data = data;

    return t;
}


int array_get(int_array* t, int i) {
    assert((*t).len > i);
    if (i < 0) {
        return (*t).data[(*t).len + i];
    } else {
        return (*t).data[i];
    }
}

void array_set(int_array* t, int i, int x) {
    assert((*t).len > i);
    if (i < 0) {
        (*t).data[(*t).len + i] = x;
    } else {
        (*t).data[i] = x;
    }
}

void array_delete(int_array* t) {
    free((*t).data);
    free(t);
}

void array_print(int_array* t){
    for (int i = 0; i < t->len; i++){
        printf("%d ", array_get(t, i));
    }
    printf("\n");
}

int array_sum(int_array* t){
    int n = t->len;
    int s = 0;
    for (int i = 0; i < n; i++){
        s += array_get(t, i);
    }
    return s;
}

void test_partie_1(void){
    int size = 100;
    int_array* t = array_create(size, 3);
    assert(array_sum(t) == 3 * size);
    int n = t->len;
    for (int i = 0; i < n; i++){
        array_set(t, i, array_get(t, i) + i);
    }
    assert(array_sum(t) == 3 * size + size * (size - 1) / 2);
    array_delete(t);
}


/************/
/* Partie 3 */
/************/

struct int_dynarray {
    int len;
    int capacity;
    int* data;
};

typedef struct int_dynarray int_dynarray;



/* Partie 3.1 */

int length(int_dynarray* t) {
    return (*t).len;
}

int_dynarray* make_empty(void) {
    int_dynarray* t = malloc(sizeof(int_dynarray));
    (*t).data = NULL;
    (*t).capacity = 0;
    (*t).len = 0;
    return t;
}

int get(int_dynarray* t, int i) {
    assert((*t).len > i);
    if (i < 0) {
        return (*t).data[(*t).len + i];
    } else {
        return (*t).data[i];
    }
}

void set(int_dynarray* t, int i, int x) {
    assert((*t).len > i);
    if (i < 0) {
        (*t).data[(*t).len + i] = x;
    } else {
        (*t).data[i] = x;
    }
}

void resize(int_dynarray* t, int new_capacity) {
    assert((*t).len <= new_capacity);
    int* new_data = malloc(new_capacity * sizeof(int));
    for (int i = 0; i < (*t).len ;i++) {
        new_data[i] = (*t).data[i];
    }
    free((*t).data);
    (*t).data = new_data;
    (*t).capacity = new_capacity;   
}

int pop_naif(int_dynarray* t) {
    assert((*t).len > 0);
    (*t).len--;
    int temp = (*t).data[(*t).len];
    return temp;
}

void push_naif(int_dynarray* t, int x) {
    int temp_capacity = (*t).capacity;
    if ((*t).len == temp_capacity) {
        resize(t,temp_capacity + 1);
    }
    (*t).len++;
    set(t,(*t).capacity -1,x); 
}

void delete(int_dynarray* t) {
    free((*t).data);
    free(t);
}


// Pour tester

void print(int_dynarray* t){
    int n = length(t);
    for (int i = 0; i < n; i++){
        printf(" %d ", get(t, i));
    }
    printf("\n");
}



/* Partie 3.2 */

void push(int_dynarray* t, int x) {
    int temp_capacity = (*t).capacity;
    if ((*t).len == temp_capacity) {
        if (temp_capacity == 0) {
            resize(t,1);
        } else {
            resize(t,temp_capacity * 2);
        }
    }
    (*t).len++;
    set(t,(*t).len -1,x); 
}

int pop(int_dynarray* t) {
    assert((*t).len > 0);
    int x = (*t).data[(*t).len - 1];
    (*t).len--;
    if (2 * (*t).len < (*t).capacity) {
        resize(t,(*t).capacity / 2);
    }
    return x;
}


/* Partie 3.3 */

void insert_at(int_dynarray* t, int i, int x) {
    int l = length(t);
    assert(i < length(t) && i >= 0);
    push(t,x);
    for (int j = l; j > i;j--) {
        set(t,j,get(t,j - 1));
    }
    set(t,i,x);
}

int pop_at(int_dynarray* t, int i) {
    int l = length(t);
    assert(i < length(t) && i >= 0);
    int temp = get(t,i);
    for (int k = i; k < l - 1; k++) {
        set(t, k,get(t,k + 1));
    }
    (*t).len--;
    return temp;
}

int position_linear(int_dynarray* t, int x) {
    int i = length(t) - 1;
    while (i >= 0 && get(t,i) > x) {
        i--;
    }
    return i + 1;
}

int position(int_dynarray* t, int x);

int_dynarray* insertion_sort(int_dynarray* t) {
    int_dynarray* out = make_empty();
    int len = length(t);
    for (int i = 0; i < len;i++) {
        int elt = get(t,i);
        int index = position_linear(out,elt);
        insert_at(t,index,elt);
    }
    return out;
}




int main(void){
    int_dynarray* dyntab = make_empty();
    push(dyntab,3);
    push(dyntab,2);
    push(dyntab,1);
    print(dyntab);
    int temp = pop(dyntab);
    printf("Temp : %d\n",temp);
    insert_at(dyntab,1,4);
    print(dyntab);
    int temp2 = pop_at(dyntab,1);
    printf("Temp2 (pop_at : %d) : %d\n",1,temp2);
    printf("\n\n");
    print(dyntab);
    printf("\n\n");
    int_dynarray* sorted_tab = insertion_sort(dyntab);
    printf("Len sorted tab : %d",length(sorted_tab));
    delete(dyntab);
    delete(sorted_tab); 
    return 0;
}
