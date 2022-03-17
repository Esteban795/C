#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int* greedy_change(int system[],int n,int target){
    int* result = malloc(sizeof(int) * n);
    int i = n - 1;
    while (i >= 0 && target > 0){
        result[i] = target / system[i];
        target = target % system[i];
        i--;
    }
    assert(target == 0);
    return result;
}