#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>


typedef uint64_t T;

T *read_elements(FILE *fp, int *len, uint64_t *goal){
    fscanf(fp, "%d %" PRIu64, len, goal);
    uint64_t *elements = malloc(*len * sizeof(uint64_t));
    for (int i = 0; i < *len; i++) {
        fscanf(fp, "%" PRIu64, elements + i);
    }
    return elements;
}

void print_solution(FILE *fp, uint64_t elements[], int len, bool solution[]){
    for (int i = 0; i < len; i++){
        if (solution[i]) fprintf(fp, "%" PRIu64 " ", elements[i]);
    }
    fprintf(fp, "\n");
}

bool naive_decision(T arr[],int len,T goal){
    if (goal == 0) return true;
    if (len == 0) return true;
    return naive_decision(&arr[1],len - 1, goal - arr[0]) || naive_decision(&arr[1],len - 1,goal);
}

bool naive_solution_aux(T arr[],int len,T goal,bool sol[]){
    if (goal == 0) return true;
    if (len == 0) return false;
    sol[0] = true;
    if (naive_solution_aux(&arr[1],len - 1,goal - arr[0],&sol[1])) {return true;}
    else {
        sol[0] = false;
        naive_solution_aux(&arr[1],len - 1,goal,&sol[1]);
    }
}

bool* naive_solution(T arr[], int len, T goal){
    bool* sol = malloc(sizeof(bool) * len);
    naive_solution_aux(arr,len,goal,sol);
    return sol;
}

typedef uint64_t set_t;

void compute_sums(T arr[],set_t set, int i,T sum,T* sums){
    if (i < 0){
        sums[set] = sum;
    }
    set_t new_set = set | (1ull << i);
    compute_sums(arr,new_set,i - 1,sum + arr[i],sums);
    compute_sums(arr,set,i - 1,sum,sums);
}

bool exists_sum(T goal,T sA[], int n, T sB[],int p){
    T len_A = 1ull << n;
    T len_B = 1ull << p;
    T iA = 0;
    T iB = len_B - 1;
    while (iA < len_A && iB > iB - 1){
        T result = sA[iA] + sB[iB];
        if (result == goal) return true;
        if (result < goal) iA++;
        else iB--;
    }
    return false;
}

//L.6.3
typedef uint64_t ui;
const int BLOCK_SIZE = 5;
const int RADIX = 2 << BLOCK_SIZE;
const int MASK = (2 << BLOCK_SIZE) - 1;

void print_array(ui* t, int len){
    if (len > 20) { len = 20; }
    for (int i = 0; i < len; i++){
        printf("%llu\n", (long long unsigned)t[i]);
    }
    printf("\n");
}

bool is_sorted(ui *arr, int len){
    for (int i = 0; i < len - 1; i++){
        if (arr[i] > arr[i + 1]){
            return false;
        }
    }
    return true;
}

ui* rand_array(int len){
    ui* t = malloc(len * sizeof(ui));
    for (int i = 0; i < len; i++){
        t[i] = (ui)rand() * (ui)rand();
    }
    return t;
}

void copy(ui* out,ui* in, int len) {
    for (int i = 0; i < len; i++) {
        out[i] = in[i];
    }
}

void zero_out(int* arr,int len) {
    for (int i = 0; i < len; i++) {
        arr[i] = 0;
    }
}

ui extract_radix(ui n, int k) {
    return (n >> (k * BLOCK_SIZE)) & MASK;
}

int* histogram(ui* arr, int len, int k) {
    int* temp = malloc(sizeof(int) * RADIX);
    zero_out(temp,RADIX);
    for (int i = 0; i < RADIX; i++) {
        int rad = extract_radix(arr[i],k);
        temp[rad]++;
    }
}

int* prefix_sums(int* hist, int len) {
    int* temp = malloc(sizeof(int) * len);
    int sum = 0;
    for (int i = 1; i < len; i++) {
        temp[i] = sum;
        sum += hist[i];
    }
    return temp;
}

void radix_pass(ui* out,ui* in,int len, int k) {
    int* histo = histogram(in,len,k);
    int* s = prefix_sums(histo,RADIX);
    for (int i = 0; i < len; i++) {
        int n = extract_radix(in[i],k);
        out[s[n]] = in[i];
        s[n]++;
    }
    free(histo);
    free(s);
}

void radix_sort(ui* arr,int len) {
    int nb_digits = 1 + (sizeof(ui) * 8 - 1) /BLOCK_SIZE;
    for (int k = 0; k < nb_digits;k++) {
        ui* tmp = malloc(len * sizeof(ui));
        radix_pass(tmp,arr,len,k);
        copy(arr,tmp,len);
        free(tmp);
    }
}
void sort_sums(T arr,int len){
    radix_sort(arr,len);
}

bool decision(T arr[],int len, T goal){
    T sums = malloc((1ull << len) * sizeof(T));
    compute_sums(arr,)


int main(int argc, char *argv[]){
    /*
    FILE* out = stdout;
    FILE* in = stdin;
    int* len;
    T* goal;
    if (argc > 1) {in = fopen(argv[1],"r");}
    if (in == NULL){
        fprintf(stderr,"file not found");
        exit(EXIT_FAILURE);
    }
    if (argc > 2){
        out = fopen(argv[2],"r");
    }
    if (out == NULL){
        fprintf(stderr,"File not found");
        exit(EXIT_FAILURE);
    }
    T* elts = read_elements(in,&len,&goal);
    bool* solution = naive_solution(elts,*len,*goal);
    if (solution){
        fprintf(out,"Yes");
        print_solution(out,elts,*len,*solution);  
    } else {
        fprintf(out,"No");
    }
    free(elts);
    free(solution);
    return 0;
    */

}
