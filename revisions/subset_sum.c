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

bool naive_decision(T arr[],int len, T goal){
    if (goal == 0 || len == 0) return true;
    return naive_decision(arr + 1,len - 1, goal - arr[0]) || naive_decision(arr + 1,len - 1, goal); 
}