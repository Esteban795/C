#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void question4(void){
    FILE* test = fopen("test","a");
    fprintf(test,"WE DID IT BOYZ");
    fclose(test);
}

void question5(void){
    char* filename = "plot.txt";
    FILE* f = fopen(filename,"w");
    for (int i = 0; i <= 10000;i++){
        fprintf(f,"%d %d\n",i,i*i);
    }
    fclose(f);
}

void copy(FILE* input_stream, FILE* output_stream){
    char c = "\0";
    while (fscanf(input_stream,"%c",&c) == 1) {
        fprintf(output_stream,"%c",c);
    }
}

void copy_corrupted(FILE* input_stream, FILE* output_stream){
    char c = '\0';
    while (fscanf(input_stream,"%c",&c) == 1) {
        if (c == 'a') {c = 'e';}
        else if (c == 'e') {c = 'a';}
        fprintf(output_stream,"%c",c);
    }
}

int question8(){
    FILE* f = fopen("data/prenoms/prenoms.txt","r");
    int compteur = 0;
    char c = '\0';
    while (fscanf(f,"%c",&c) == 1){
        if (c == '\n') {compteur++;}
    }
    return compteur;
}

void question9(char letter,FILE* input_stream){
    int compteur = 0;
    bool right_after_newline = true;
    char c = '\0';
    while (fscanf(input_stream,"%c",&c) == 1){
        if (right_after_newline && c == letter){
            compteur++;
        }
        if (c == '\n') {right_after_newline = true;}
        else {right_after_newline = false;}
    }
    printf("%d",compteur);
}

void question10(FILE* input_stream,int max_longueur_prenom){
    char* ligne = malloc(1 + max_longueur_prenom);
    while (fscanf(input_stream,"%s",ligne) == 1){
        if ((int)strlen(ligne) == max_longueur_prenom){
            printf("\t%s\n",ligne);
        }
    }
    free(ligne);
    printf("\n");
    fclose(input_stream);
}

void question10(FILE* input_stream,int min_longueur_prenom){
    char* ligne = malloc(1 + min_longueur_prenom);
    while (fscanf(input_stream,"%s",ligne) == 1){
        if ((int)strlen(ligne) == min_longueur_prenom){
            printf("\t%s\n",ligne);
        }
    }
    free(ligne);
    printf("\n");
    fclose(input_stream);
}

bool question11(FILE* input_stream, char* firstname,int max_longueur_prenom){
    char* ligne = malloc(max_longueur_prenom + 1);
    while (fscanf(input_stream,"%s",ligne)) {
        if (strcmp(ligne,firstname)){return true;}
    }
    return false;
}

float question12(FILE* input_stream,char letter){
    int total = question8();
    bool newline = true;
    char c = '\0';
    int compteur = 0;
    while (fscanf(input_stream,"%c",&c)){
        if (newline && input_stream == letter){compteur++;}
        if (c == "\n") {newline = true;}
        else {newline = false;}
    }
    return compteur/total;
}

bool is_palindromic(char* str1){
    int len = strlen(str1);
    for (int i = 0; i < len; i++){
        if (str1[i] != str1[len - i - 1]) {return false;}
    }
    return true;
}

int question13(FILE* input_stream,int max_len){
    char* ligne = malloc(max_len + 1);
    int compteur = 0;
    while (fscanf(input_stream,"%s",ligne) == 1){
        if (is_palindromic(ligne)){compteur++;}
    }
    return compteur;
}

int question14(FILE* input_stream){
    printf("Est-ce que 1+1=2 ?");
    printf("Non, strlen(1+1) = 3.Quel dommage ! ");
}

int question18(int n){
    FILE* f = fopen("data/pi/pi.txt","r");
    char c = '\0';
    while (n > 0) {
        fscanf(f,"%c",&c);
    }
    fscanf(f,"%c",c);
    return atoi(c);
}

void question19(FILE* input_stream){
    int* decimals = malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; i++);
    char* c = '\0';
    while (fscanf(input_stream,"%c",&c)){
        decimals[atoi(c)]++;
    }
    for (int i = 0; i < 10; i++){
        printf("%d : %f",i,decimals[i]/10000000);
    }
}

int main(void){
    FILE* f = fopen("data/prenoms/prenoms.txt","r");
    question10(f,100);
    return 0;
}
