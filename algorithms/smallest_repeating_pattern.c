#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NB_CHAR 256


struct result {
    char* pattern;
    int power;
};

typedef struct result result;

int max(int a,int b){
    if (a > b) return a;
    return b;
}
result* generate_result(char* text,int len,int power){
    result* res = malloc(sizeof(result));
    res->power = power;
    res->pattern = malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len;i++){
        res->pattern[i] = text[i];
    }
    res->pattern[len] = '\0';
    return res;
}

void free_result(result* res){
    free(res->pattern);
    free(res);
}

int boyer_moore(char* text,char* substring){
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
            return shift + 1; //found an occurence starting at shift
        }
        else {
            shift += max(1,j - last_occs[(int) text[shift + j]]); //text[shift + j] = première lettre différente de substring
        }                                                   // On calcule donc le plus grand décalage possible pour avoir text[shift + j] qui s'aligne sur notre substring                                             
    }
    return -1; //no occurence found
}

result* smallest_repeating_pattern(char* string,int n){
    char* text = malloc(1 + 2 * n);
    strcpy(text,string);
    strcpy(text + n,string);
    int index = boyer_moore(text + 1,string); // impossible de se retrouver dans le cas où ça vaut -1..
    int power = n / index;
    free(text);
    return generate_result(string,n / power,power);
}

int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int len = strlen(argv[1]);
    result* res = smallest_repeating_pattern(argv[1],len);
    printf("Result is : %s, repeated %d times\n\n",res->pattern,res->power);
    free_result(res);
    return 0;
}