#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_VAR 128


int plus_frequent_successeur(char c, char* chaine){
    int* histogramme = malloc(sizeof(int) * MAX_VAR);
    int len = strlen(chaine);
    for (int i = 0; i < MAX_VAR; i++){
        histogramme[i] = 0;
    }
    for (int i = 0; i < len - 1;i++){
        if (chaine[i] == c) histogramme[(int)chaine[i + 1]] += 1;
    }
    int maxi = 0;
    int index_maxi = 0;
    for (int i = 0; i < MAX_VAR; i++){
        if (maxi < histogramme[i]) {
            maxi = histogramme[i];
            index_maxi = i;
        }
    }
    free(histogramme);
    return index_maxi;
}

int* init_modele(char* chaine){
    int* modele = malloc(sizeof(int) * MAX_VAR);
    for (int i = 0; i < MAX_VAR; i++){
        modele[i] = plus_frequent_successeur((char)i,chaine);
    }
    return modele;
}

int** matrice_confusion(int* M,char* test){
    int** m = malloc(sizeof(int*) * MAX_VAR);
    for (int i = 0; i < MAX_VAR; i++){
        m[i] = malloc(sizeof(int) * MAX_VAR);
        for (int j = 0; j < MAX_VAR; j++){
            m[i][j] = 0;
        }
    }
    int i = 1;  
    while (test[i] != '\0'){
        m[(int)test[i]][M[(int)test[i - 1]]] += 1;
        i++;
    }
    return m;
}

double erreur(int** m){
    double s = 0;
    double s_sans_diag = 0;
    for (int i = 0; i < MAX_VAR;i++){
        for (int j = 0; j < MAX_VAR;j++){
            s += m[i][j];
            if (i != j) s_sans_diag += m[i][j];
        }
    }
    return s_sans_diag/s;
}

int main(void){
    char* s_modele = "Bonjour, comment allez-vous ? Ca va, ca va aller bien mieux";
    char* s_donnee = "Bonjour, ca va bien ? Oui ! Bien mieux, et vous, ca va ?";
    int* modele = init_modele(s_modele);

    int** m = matrice_confusion(modele,s_donnee);
    for (int i = 0; i < MAX_VAR;i++){
        for (int j = 0; j < MAX_VAR;j++){
            printf("%d",m[i][j]);
        }
        printf("\n");
    }

    printf("\n\nErreur globale : %f\n",erreur(m) * 100);

    //clean
    for (int i = 0; i < MAX_VAR; i++){
        free(m[i]);
    }
    free(m);
    free(modele);
    return 0;
}

//gcc code.c -o code -Wall -Wextra -Wvla -fsanitize=address