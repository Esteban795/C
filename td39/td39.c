#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>


void matrix_print(bool **m, int n, int p){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < p; j++) {
            printf("%d ", m[i][j]);
        }
        printf("\n");
    }
}

bool **matrix_new(int n, int p){
    bool** matrix = malloc(sizeof(bool*) * n);
    for (int i = 0; i < n; i++){
        matrix[i] = malloc(sizeof(bool) *p);
        for (int j = 0; j < p;j++){
            matrix[i][j] = false;
        }
    }
    return matrix;
}

void matrix_free(bool **m, int n){
    for (int i = 0; i < n;i++){
        free(m[i]);
    }
    free(m);
}

bool **identity(int n){
    bool** m = matrix_new(n,n);
    for (int i = 0; i < n;i++){
        m[i][i] = true;
    }
    return m;
}

// Reads a graph from standard input and returns
// its adjacency matrix. Data format is :
//   - first line : two integers n and p, separated by a space.
//     n is the number of vertices, p the number of edges.
//     Vertices are numbered 0..n-1.
//   - next p lines : two integers x and y per line, separated by a space.
//     Corresponds to an edge from vertex x to vertex y.
//
// Output paramter n is set to the number of vertices by the call
// to read_data.
bool **read_data(int *n){
    int nb_edges;
    scanf("%d %d", n, &nb_edges);
    bool **a = matrix_new(*n, *n);
    for (int k = 0; k < nb_edges; k++) {
        int i, j;
        scanf("%d %d", &i, &j);
        a[i][j] = true;
    }
    return a;
}

bool **product(bool **a, bool **b, int n, int p, int q){
    bool** m = matrix_new(n,q);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < q;j++){
            for (int k = 0;k < p; k++){
                m[i][j] = m[i][j] || (a[i][k] && b[k][j]);
            }
        }
    }
    return m;
}


bool** power_matrix(bool** a, int n, int pow){
    if (pow == 0) return identity(n);
    bool** m2 = product(a,a,n,n,n);
    bool** b = power_matrix(m2,n,pow/2);
    if (pow % 2 == 0) {
        matrix_free(m2,n);
        return b;
    } else {
        bool** res = product(b,a,n,n,n);
        matrix_free(m2,n);
        matrix_free(b,n);
        return res;
    }
}

bool **closure(bool **a, int n){
    return power_matrix(a,n,n-1);
}


void explore(bool** m,int n,int i, bool* seen) {
    seen[i] = true;
    for (int j = 0; j < n;j++) {
        if (m[i][j] && !seen[j]) explore(m,n,j,seen);
    }
}

bool *accessible(bool **a, int n, int i){
    bool* seen = malloc(n * sizeof(bool));
    for (int i = 0; i < n;i++) {
        seen[i] = false;
    }
    explore(a,n,i,seen);
    return seen;
}

bool **closure_dfs(bool **a, int n){
    bool** matrix_access = malloc(sizeof(bool*) * n);
    for (int i = 0; i < n;i++){
        matrix_access[i] = accessible(a,n,i);
    }
    return matrix_access;
}

bool is_axiom(bool **b, int n, int i){
    for (int j = 0; j < n;j++){
        if (b[j][i] && !b[i][j]) return false;
    }
    return true;
}

bool *axiom_system(bool **b, int n);

void print_system(bool *system, int n){
    for (int i = 0; i < n; i++) {
        if (system[i]) printf("%d ", i);
    }
    printf("\n");
}

int main(void){
    int n;
    bool** m = read_data(&n);
    bool** t = closure(m,n);
    printf("\n\n\n");
    matrix_print(t,n,n);
    printf("\n\n\n");
    printf("x1 est un axiome %d",is_axiom(t,n,1));
    matrix_free(m,n);
    matrix_free(t,n);
    return 0;
}
