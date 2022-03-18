#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


/*
Relation de récurrence : 

- c[i][j] = 0 si i = 0 ou j = 0;
- c[i][j] = 1 + c[i - 1][j - 1] si x[i] = y[j] 
- c[i][j] = max(c[i - 1][j], c[i][j - 1])
*/

int max(int a,int b){
    if (a >= b) return a;
    return b;
}


char* longest_common_subsequence(char* w1, char* w2,int len1,int len2,int* len_subseq){
    int matrix[len1 + 1][len2 + 1];
    for (int i = 0; i < len1 + 1;i++) {
        for (int j = 0; j < len2 + 1;j++) {
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < len1; i++){
        for (int j = 0; j < len2;j++){
            if (w1[i] == w2[j]) {
                matrix[i + 1][j + 1] = 1 + matrix[i][j];
            } else {
                matrix[i + 1][j + 1] = max(matrix[i][j + 1],matrix[i + 1][j]);
            }
        }
    }
    char* sol = malloc(sizeof(char) * matrix[len1][len2]);
    *len_subseq = matrix[len1][len2];
    int k = matrix[len1][len2] - 1;
    int i = len1;
    int j = len2;
    while (matrix[i][j] > 0){
        if (matrix[i][j] == matrix[i - 1][j]) i--;
        else if (matrix[i][j] == matrix[i][j - 1]) j--;
        else {
            i--;
            j--;
            sol[k] = w1[i];
            k--;
        }
    }
    return sol;
}

int main(void){
    int len;
    char* word1 = "set";
    char* word2 = "test2";
    char* result = longest_common_subsequence(word1,word2,3,5,&len);
    for (int i = 0; i < len;i++){
        printf("%c",result[i]);
    }
    printf("\n");
    free(result);
    return 0;
}