#include <stdio.h>
#include <string.h> 
#include <stdlib.h>

void print(int* m,int l1, int l2) {
    printf("\n");
    for (int i = 0; i < l1;i++) {
        for (int j = 0; j < l2; j++) {
            printf("%d ",m[i * l1 + j]);
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

int levenshtein(char* string1,char* string2) {
    int lines = strlen(string1);
    int columns = strlen(string2);
    int temp;
    printf("Lines : %d, Columns : %d\n",lines,columns);
    int* matrix = malloc(sizeof(int) * lines * columns);
    for (int i = 0; i < lines;i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i * lines + j] = i + j;
        }
    }
    for (int i = 0;i < lines;i++) {
        for (int j = 0; j < lines;j++) {
            if (string1[i * lines] != string2[i * lines + j]) {
                temp = 1;
            } else {
                temp = 0;
            }
            matrix[(i + 1) * lines + (j + 1)] = minimum(
                matrix[i * lines + j + 1] + 1,
                matrix[(i + 1) * lines + j] + 1,
                matrix[i * lines + j] + temp
            );
        }
    }
    print(matrix,lines,columns);
    
    free(matrix);
    return 1;
}


int main(int argc,char* argv[]) {
    if (argc == 1) {
        return 1;
    }
    int distance = levenshtein(argv[1],argv[2]);
    printf("Distance de Levenshtein : %d",distance);
    return 0;
}