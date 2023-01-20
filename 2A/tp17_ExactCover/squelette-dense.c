#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>


struct dense {
    int nb_rows;
    int nb_columns;
    bool **data;
};

typedef struct dense Dense;

struct partial {
    bool *selected_rows;
    bool *remaining_rows;
    bool *remaining_columns;
};

typedef struct partial Partial;


void dense_free(Dense *inst){
    for (int r = 0; r < inst->nb_rows; r++) {
        free(inst->data[r]);
    }
    free(inst->data);
    free(inst);
}

bool *all_false(int n){
    bool *t = malloc(n * sizeof(bool));
    for (int i = 0; i < n; i++) t[i] = false;
    return t;
}

bool *all_true(int n){
    bool *t = malloc(n * sizeof(bool));
    for (int i = 0; i < n; i++) t[i] = true;
    return t;
}

void partial_free(Partial *partial){
    free(partial->selected_rows);
    free(partial->remaining_rows);
    free(partial->remaining_columns);
    free(partial);
}


bool *copy(bool *arr, int len){
    bool *new_arr = malloc(len * sizeof(bool));
    for (int i = 0; i < len; i++) new_arr[i] = arr[i];
    return new_arr;
}

Partial *partial_copy(Partial *partial, int n, int p){
    Partial *new_partial = malloc(sizeof(Partial));
    new_partial->selected_rows = copy(partial->selected_rows, n);
    new_partial->remaining_rows = copy(partial->remaining_rows, n);
    new_partial->remaining_columns = copy(partial->remaining_columns, p);
    return new_partial;
}

Partial *partial_new(Dense *inst){
    Partial *partial = malloc(sizeof(Partial));
    partial->remaining_rows = all_true(inst->nb_rows);
    partial->selected_rows = all_false(inst->nb_rows);
    partial->remaining_columns = all_true(inst->nb_columns);
    return partial;
}

// returns true iff i is an available row, j an
// uncovered column and i covers j
bool get(Dense *inst, Partial *partial, int i, int j){
    return
        partial->remaining_rows[i]
        && partial->remaining_columns[j]
        && inst->data[i][j];
}


// Exercice 4

bool **read_matrix(FILE* f,int *n, int *p){
    bool** m = malloc(*n * sizeof(bool*));
    int temp;
    for (int i = 0; i < *n;i++){
        m[i] = malloc(*p * sizeof(bool));
        for (int j = 0; j < *p;j++){
            fscanf(f,"%d",&temp);
            m[i][j] = temp == 1 ? true: false;
        }
    }
    return m;
}


Dense *read_instance(FILE* f){
    Dense* instance = malloc(sizeof(Dense));
    int n;
    int p;
    fscanf(f,"%d %d\n",&n,&p);
    instance->nb_columns = p;
    instance->nb_rows = n;
    instance->data = read_matrix(f,&n,&p);
    return instance;
}

void print_partial_instance(Dense *inst, Partial *partial){
    int n = inst->nb_rows;
    int p = inst->nb_columns;
    printf("Remaining colums : \n");
    for (int j = 0; j < p;j++){
        if (partial->remaining_columns[j]) {
            for (int i = 0; i < n;i++){
                printf("%d",inst->data[i][j]);
            }
            printf("\n");
        }
    }
    printf("\n");
    printf("Rows remaining : \n");
    for (int i = 0; i < n; i++){
        if (partial->remaining_rows[i]) {
            for (int j = 0; j < p;j++){
                printf("%d ",inst->data[i][j]);
            }
            printf("\n");
        }
    }
    printf("\n");
}

void print_partial_solution(Dense *inst, Partial *partial){
    for (int i = 0; i < inst->nb_rows;i++){
        if (partial->selected_rows[i]) printf("%d, ",i);
    }
    printf("\n");
}

void print_m(bool** m,int n,int p){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < p;j++){
            printf("%d ",m[i][j] == 1 ? 1 : 0);
        }
        printf("\n");
    }
}

// Exercice 5

void cover_column(Dense *inst, Partial *partial, int r, int c){
    for (int i = 0; i < inst->nb_rows;i++){
        if (!get(inst,partial,i,c) || i == r) continue;
        partial->remaining_rows[i] = false;
    }
    partial->remaining_columns[c] = false;
}

void select_row(Dense *inst, Partial *partial, int r){
    partial->selected_rows[r] = true;
    for (int j = 0; j < inst->nb_columns; j++){
        if (!get(inst,partial,r,j)) continue;
        cover_column(inst,partial,r,j);
    }
    partial->remaining_rows[r] = false;
}


// Exercice 6

int counter_true(bool* arr,int len){
    int s = 0; 
    for (int i = 0; i < len;i++){
        if (arr[i]) s++;
    }
    return s;
}


int choose_first_column(Dense *inst, Partial *partial){
    for (int c = 0; c < inst->nb_columns;c++){
        if (partial->remaining_columns[c]) return c;
    }
    return -1;
}



int count(Dense *inst, Partial *partial){
    int n = inst->nb_rows;
    int p = inst->nb_columns;
    if (counter_true(partial->remaining_columns,p) == 0) return 1;
    if (counter_true(partial->remaining_rows,n) == 0) return 0;
    int c = choose_first_column(inst,partial);
    int nb_sol = 0;
    for (int i = 0; i < n;i++){
        if (get(inst,partial,i,c)){
            Partial* copy_partial = partial_copy(partial,n,p);
            select_row(inst,copy_partial,i);
            nb_sol += count(inst,copy_partial);
            partial_free(copy_partial);
        }
    }
    return nb_sol;
}


bool search(Dense *inst, Partial *partial, Partial **solution){
    int n = inst->nb_rows;
    int p = inst->nb_columns;
    if (counter_true(partial->remaining_columns,p) == 0) {
        *solution = partial_copy(partial,n,p);
        return true;
    }
    if (counter_true(partial->remaining_rows,n) == 0) return false;
    int c = choose_first_column(inst,partial);
    for (int i = 0; i < n;i++){
        if (!get(inst,partial,i,c)) continue;
        Partial* copy_partial = partial_copy(partial,n,p);
        select_row(inst,partial,i);
        bool res = search(inst,partial,solution);
        partial_free(copy_partial);
        if (res) return true; //on ne veut pas sortir de la boucle direct si on ne trouve pas de solutions du premier coup...
    }
    return false;
}


int main(int argc, char **argv){
    if (argc != 2) return EXIT_FAILURE;
    FILE* f = fopen(argv[1],"r");
    Dense* inst = read_instance(f);
    fclose(f);
    Partial* part = partial_new(inst);
    int nb = count(inst,part);
    dense_free(inst);
    partial_free(part);
    printf("Nombre de solutions : %d\n",nb);
    return 0;
}
