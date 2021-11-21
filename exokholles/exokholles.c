#include <stdio.h>
#include <stdbool.h>

int maximum(int t[],int taille) {
    int maxi = t[0];
    for (int i = 1; i < taille;i++) {
        if (maxi < t[i]) {
            maxi = t[i];
        }
    }
    return maxi;
}

void maxi_pointeur(int t[],int taille,int* maxi) {
    *maxi = t[0];
    for (int i = 1; i < taille; i++) {
        if (t[i] > *maxi) {
            *maxi = t[i];
        }
    }
}


int second_plus_grand_element(int t[],int taille) {
    int maxi1 = t[0];
    int maxi2 = t[1];
    if (maxi1 < maxi2) {
        int temp = maxi1;
        maxi2 = maxi1;
        maxi1 = maxi2;
    }
    for (int i = 2; i < taille; i++) {
        if (t[i] > maxi1) {
            maxi2 = maxi1;
            maxi1 = t[i];
        } else if (t[i] < maxi1 && t[i] > maxi2) {
            maxi2 = t[i];
        }
    }
    return maxi2;
}

int max_subarray_k_length(int tab[],int taille,int k) {
    int sum = 0;
    int index = 0;
    for (int i = 0; i < k; i++) {
        sum += tab[i];
    }
    int maxi = sum;
    for (int j = 1; j < taille - k + 1;j++) {
        sum = sum - tab[j - 1] + tab[j + k - 1];
        if (maxi < sum) {
            maxi = sum;
            index = j;
        }
    }
    return index;
}

bool tous_nombres_presents(int row,int col,int matrix[row][col]) {
    int presents[row * col];
    for (int k = 0; k < row * col; k++) {
        presents[k] = 0;
    }
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (presents[matrix[i][j]] != 0) {
                return false;
            } else {
                presents[matrix[i][j]] = 1;
            }
        }
    }
    return true;
}

bool carre_magique(int row,int col,int matrix[row][col]) {
    /* initialisation */
    int init = 0;
    int sum;
    for (int j = 0; j < col; j ++) {
        init += matrix[0][j];
    }
    /* On vérifie les lignes, le plus simple */
    for (int i = 1; i < row; i ++) {
        sum = 0;
        for (int j = 0; j < col; j++) {
            sum += matrix[i][j];
        }
        if (sum != init) {
            return false;
        }
    }
    /* on vérifie les colonnes */
    for (int j = 0; j < col; j++) {
        sum = 0;
        for (int i = 0; i < row; i++) {
            sum += matrix[i][j];
        }
        if (sum != init) {
            return false;
        }
    }
    /* On vérifie les diagonales */
    int k = 0;
    sum = 0;
    for (int n = 0; n < row; n++) {
        sum += matrix[k][n];
        k += 1;
    }
    if (sum != init) {
        return false;
    }
    sum = 0;
    k = row - 1;
    for (int n = 0 ; n < row; n++) {
        sum += matrix[k][row - n - 1];
        k--; 
    }
    if (sum != init) {
        return false;
    }
    return true;
}


int main(void) {
    int t[3][3] = {{6,1,8},{7,5,3},{2,9,4}};
    bool test = carre_magique(3,3,t);
    printf("%s",test ? "yes" : "no");
    return 0;
}