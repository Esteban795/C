#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


void append_line(void){
    FILE* test = fopen("test","a");
    fprintf(test,"WE DID IT BOYZ");
    fclose(test);
}

void x_squared(void){
    char* filename = "plot.txt";
    FILE* f = fopen(filename,"w");
    for (int i = 0; i <= 10000;i++){
        fprintf(f,"%d %d\n",i,i*i);
    }
    fclose(f);
}

void copy_file(FILE* input_stream, FILE* output_stream){
    char c = "\0";
    if (input_stream == NULL || output_stream == NULL) return EXIT_FAILURE;
    while (fscanf(input_stream,"%c",&c) == 1) {
        fprintf(output_stream,"%c",c);
    }
}

void copy_corrupted(FILE* input_stream, FILE* output_stream){
    char c = '\0';
    while (fscanf(input_stream,"%c",&c) == 1) {
        if (c == 'a') {c = 'e';} //replaces a by e, and e by a
        else if (c == 'e') {c = 'a';}
        fprintf(output_stream,"%c",c);
    }
}

int how_many_lines(FILE* input_stream){
    if (input_stream == NULL) return EXIT_FAILURE;
    int counter = 0;
    char c = '\0';
    while (fscanf(input_stream,"%c",&c) == 1){
        if (c == '\n') counter++;
    }
    return counter;
}

int words_starts_with_letter(FILE* input_stream,char letter){
    int counter = 0;
    bool right_after_newline = true;
    char c = '\0';
    while (fscanf(input_stream,"%c",&c) == 1){
        if (right_after_newline && c == letter) counter++;//we just started a new line and first letter is the one we're looking for
        if (c == '\n') {right_after_newline = true;}
        else {right_after_newline = false;}
    }
    return counter;
}

void print_longest_names(FILE* input_stream,int max_name_len){
    char* line = malloc(1 + max_name_len);
    while (fscanf(input_stream,"%s",line) == 1){
        if ((int)strlen(line) == max_name_len){
            printf("\t%s\n",line);
        }
    }
    free(line);
}

void print_shortest_names(FILE* input_stream,int min_name_len){
    char* line = malloc(1 + min_name_len);
    while (fscanf(input_stream,"%s",line) == 1){
        if ((int)strlen(line) == min_name_len){
            printf("\t%s\n",line);
        }
    }
    free(line);
    printf("\n");
}

bool specific_name(FILE* input_stream, char* firstname,int max_name_len){
    char* line = malloc(max_name_len + 1);
    while (fscanf(input_stream,"%s",line)) {
        if (strcmp(line,firstname)) return true;
    }
    return false;
}

float frequency_of_letter(FILE* input_stream,char letter){
    int total = how_many_lines(input_stream);
    fseek(input_stream,0,0);
    bool newline = true;
    char c = '\0';
    int counter = 0;
    while (fscanf(input_stream,"%c",&c)){
        if (newline && input_stream == letter) counter++;
        if (c == "\n") newline = true;
        else newline = false;
    }
    return (float)counter/(float)total;
}

bool is_palindromic(char* str1){
    int len = strlen(str1);
    for (int i = 0; i < len; i++){
        if (str1[i] != str1[len - i - 1]) return false;
    }
    return true;
}

int question13(FILE* input_stream,int max_len){
    char* line = malloc(max_len + 1);
    while (fscanf(input_stream,"%s",line) == 1){
        if (is_palindromic(line)) printf("\t%s\n",line);
    }
    return line;
}

int question14(FILE* input_stream){
    printf("Est-ce que 1+1=2 ?");
    printf("Non, strlen(1+1) = 3.Quel dommage ! ");
}

int nth_decimal_of_pi(int n){
    FILE* f = fopen("data/pi/pi.txt","r");
    char c = '\0';
    while (n > 0) {
        fscanf(f,"%c",&c);
    }
    fscanf(f,"%c",c);
    return atoi(c);
}

void frequency_histogram_decimals_pi(FILE* input_stream){
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
    return 0;
}
