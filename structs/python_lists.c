#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int max(int n1, int n2) {
    if (n1 > n2) {
        return n1;
    }
    return n2;
}

struct int_dynarray {
    int len;
    int capacity;
    int* data;
};

typedef struct int_dynarray int_dynarray;

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

int_dynarray* make_empty_size(int size) { //New dynamic table with an initial size
    int_dynarray* t = make_empty();
    int* data = malloc(sizeof(int) * size);
    (*t).capacity = size;
    (*t).data = data;
    return t;
}

int get(int_dynarray* t, int i) {
    assert(abs(i) <= (*t).len);
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

void resize(int_dynarray* t, int new_capacity) { //In case it runs out of memory
    assert((*t).len <= new_capacity);
    int* new_data = malloc(new_capacity * sizeof(int));
    for (int i = 0; i < (*t).len ;i++) { //Copies previous datas
        new_data[i] = (*t).data[i];
    }
    free((*t).data);
    (*t).data = new_data;
    (*t).capacity = new_capacity;   
}

void delete(int_dynarray* t) { 
    free((*t).data);
    free(t);
}

void print(int_dynarray* t){
    int n = length(t);
    for (int i = 0; i < n; i++){
        printf(" %d ", get(t, i));
    }
    printf("\n\n");
}

void push(int_dynarray* t, int x) {
    int temp_capacity = (*t).capacity;
    if ((*t).len == temp_capacity) {
        if (temp_capacity == 0) { //dynamic tab is empty
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
    if (2 * (*t).len < (*t).capacity) { //To make sure the program uses as little memory as possible
        resize(t,(*t).capacity / 2);
    }
    return x;
}

void insert_at(int_dynarray* t, int i, int x) {
    int l = length(t);
    assert(i <= l && i >= 0);
    push(t,x);
    for (int j = l; j > i;j--) { //Pushes elements to the left
        set(t,j,get(t,j - 1));
    }
    set(t,i,x);
}

int pop_at(int_dynarray* t, int i) {
    int l = length(t);
    assert(i < length(t) && i >= 0);
    int temp = get(t,i);
    for (int k = i; k < l - 1; k++) { //Pushes elements to the right
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

int position(int_dynarray* t, int x) {
    int start = 0;
    int end = length(t);
    while (end > start) {
        int mid = start + (end - start) / 2;
        int elt = get(t,mid);
        if (elt == x) {
            return mid;
        } else if (elt < mid) {
            end = mid;
        } else {
            start = mid + 1;
        }
    }
    return start;
}

int_dynarray* insertion_sort(int_dynarray* t) {
    int_dynarray* out = make_empty();

    int len = length(t);
    for (int i = 0; i < len;i++) {
        int elt = get(t,i);
        int index = position_linear(out,elt);
        insert_at(out,index,elt);
    }
    return out;
}

int index2(int_dynarray* t,int element) {
    for (int i = 0; i < length(t);i++) {
        if (get(t,i) == element) {
            return i;
        }
    }
    return -1;
}

void remove2(int_dynarray* t,int element) {
    for (int i = 0; i < length(t);i++) {
        if (get(t,i) == element) {
            pop_at(t,i);
            break;
        }
    }
}

void reverse(int_dynarray* t) {
    int l = length(t);
    int_dynarray* newdyntab = make_empty_size(l);
    for (int i = 0; i < l;i++) {
        set(newdyntab,l - i - 1,get(t,i));
    }
}

int_dynarray* copy(int_dynarray* t) {
    int l = length(t);
    int_dynarray* copied = make_empty_size(l);
    for (int i = 0; i < l; i++) {
        set(copied,i,get(t,i));
    }
    return copied;
}

void extend(int_dynarray* t1, int_dynarray*  t2) {
    int l1 = length(t1);
    int l2 = length(t2);
    int m = max(l1 + l2,2 * l1);
    (*t1).data = (int*) realloc((*t1).data,sizeof(int) * m);
    (*t1).capacity = m;
    assert((*t1).data != NULL);
    for (int i = l1; i < l1 + l2;i++) {
        push(t1,get(t2,i - l1));
    }
    delete(t2);
}

int main(void){
/*     int_dynarray* dyntab1 = make_empty();
    push(dyntab1,1);
    push(dyntab1,2);
    push(dyntab1,3);
    int_dynarray* dyntab2 = make_empty();
    push(dyntab2,1);
    push(dyntab2,5);
    print(dyntab1);
    print(dyntab2);
    printf("\n\n");
    extend(dyntab1,dyntab2);
    printf("\n\n");
    print(dyntab1);
    delete(dyntab1); */
    return 0;
}
