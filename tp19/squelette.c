#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

typedef uint32_t ui;
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

int main(void){
    ui n[10] = {1,4,56,2,23,95,15,3,56,656};
    radix_sort(n,10);
    print_array(n,10);
    return 0;
}
