#include <stdio.h>
#include <string.h> 
#include <stdlib.h>


void print(int l1,int l2,int t[l1][l2]) {
    for (int i = 0; i < l1; i++) {
        for (int j = 0; j < l2;j++) {
            printf(" %d ",t[i][j]);
        }
        printf("\n");
    }
}
int minimum(int a,int b,int c) {
    if (a < b) {
        if (a < c) {
            return a;
        } else {
            return c;
        }
    } else {
        if (b < c) {
            return b;
        } else {
            return c;
        }
    }
}

void levenshtein(char* string1,char* string2,int* result,const int lines,const int columns) {
    int temp;
    int matrix[lines + 1][columns + 1];
    for (int i = 0; i < lines + 1;i++) {
        for (int j = 0; j < columns + 1;j++) {
            matrix[i][j] = i + j;
        }
    }
    for (int i = 0;i < lines ;i++) {
        for (int j = 0; j < columns;j++) {
            if (string1[i] != string2[j]) {
                temp = 1;
            } else {
                temp = 0;
            }
            matrix[i + 1][j + 1] = minimum(
                matrix[i][j + 1] + 1,
                matrix[i + 1][j] + 1,
                (matrix[i][j] + temp));
        }
    }
    print(lines + 1,columns + 1,matrix);
    *result = matrix[lines][columns];
}


int main(int argc,char* argv[]) {
    int res;
    if (argc == 1) {
        return 1;
    }
    int l1 = strlen(argv[1]);
    int l2 = strlen(argv[2]);
    levenshtein(argv[1],argv[2],&res,l1,l2);
    printf("Distance de Levenshtein : %d\n",res);
    return 0;
}