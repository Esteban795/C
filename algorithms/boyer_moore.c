#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_CHAR 256

int max(int a, int b){
    if (a > b) return a;
    return b;
}

void boyer_moore(char* text,char* substring){
    int len_sub = strlen(substring);
    int len_text = strlen(text);
    int last_occs[NB_CHAR]; //tableau des dernières occurences de chaque caractère
    for (int i = 0; i < NB_CHAR; i++){
        last_occs[i] = -1;
    }
    for (int i = 0; i < len_sub; i++){
        last_occs[(int) substring[i]] = i;
    }
    int shift = 0;
    while (shift + len_sub < len_text ){ //on déborde pas du texte
        int j = len_sub - 1;
        while (j >= 0 && substring[j] == text[shift + j]){ //vérifie les cases de la dernière à la première
            j--;
        }
        if (j < 0){
            printf("Substring found at shift : %d\n",shift);
            return;
        }
        else {
            shift += max(1,j - last_occs[(int) text[shift + j]]); //text[shift + j] = première lettre différente de substring
        }                                                   // On calcule donc le plus grand décalage possible pour avoir text[shift + j] qui s'aligne sur notre substring                                             
    }
    printf("No sequence matched.\n");
}


int main(int argc,char* argv[]){
    if (argc != 3) return EXIT_FAILURE;
    boyer_moore(argv[1], argv[2]);
    return 0;
}