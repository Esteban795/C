#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int naive_string_search(char* m,char* t){
    int len_m = strlen(m);
    int len_t = strlen(t);
    bool found = true;
    int nb_occs = 0;
    for (int i = 0; i < len_t - len_m; i++){
        if (t[i] == m[i]) {
            found = true;
            for (int j = 1; j < len_m; j++){
                if (t[j + i] != m[j + i]){
                    found = false;
                    break;
                }
            }
            if (found) {
                printf("%d ",i);
                nb_occs++;
            } 
        }
    }
    return nb_occs;  
}

int** build_table(char* m,int len_m){
    int** tab = malloc(sizeof(int*) * len_m);
    for (int i = 0; i < len_m;i++){
        tab[i] = malloc(sizeof(int) * 256);
        for (int c = 0; c < 256;c++){
            tab[i][c] = -1;
        }
        for (int k = 0; k < i;k++){
            u_int8_t c = m[k];
            tab[i][c] = k;
        }
    }
    return tab;
}

void free_table(int** table,int lm){
    for (int i = 0; i < lm;i++){
        free(table[i]);
    }
    free(table);
}

int boyer_moore(char* m,char* t){
    int len_m = strlen(m);
    int len_t = strlen(t);

    int** tab = build_table(m,len_m);
    int nb_occs = 0;
    for (int i = 0; i < len_t - len_m;i++){
        int k = 0;
        for (int j = len_m - 1; j >= 0;j--){
            u_int8_t c = t[i + j];
            if (c != m[j]) {
                k = j - tab[j][c];
                break;
            }
        }
        if (k == 0){
            printf("On a une occurence en position %d\n",i);
            nb_occs++;
            continue;
        }
        i += k;
    }
    free_table(tab,len_m);
    return nb_occs;
}

int main(void){
    char* txt = "chercher,rechercher et rechercher encore";
    char* motif = "chercher";
    printf("On a %d occurences\n",boyer_moore(motif,txt));
    return 0;
} 