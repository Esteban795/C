#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include "../structs/queue.c"

int min(int x,int y){
    if (x < y) return x;
    else return y;
}

int max(int x,int y){
    if (x > y) return x;
    else return y;
}

struct flow_graph {
    int n;
    vertex s;
    vertex t;
    vertex** adj;
    int* degrees;
    int** capacity;
};

typedef struct flow_graph flow_graph;

int** zeroes(int n){
    int** mat = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++){
        mat[i] = malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++){
             mat[i][j] = 0;
        }
    }
    return mat;
}

void free_matrix(int** mat,int n){
    for (int i = 0; i < n ;i++) {
        free(mat[i]);
    }
    free(mat);
}


vertex* bfs_residual(flow_graph* g,int** f){
    queue* q = queue_create();
    queue_push(q,g->s);
    vertex* parents = malloc(g->n * sizeof(vertex));
    for (int i = 0; i < g->n;i++){
        parents[i] = -1;
    }
    parents[g->s] = g->s;
    while (queue_length(q) > 0) {
        vertex v = queue_pop(q);
        for (int i = 0; i < g->degrees[v];i++){
            vertex w = g->adj[v][i];
            if (parents[w] != -1 || g->capacity[v][w] - f[v][w] <= 0) continue;
            parents[w] = v;
            queue_push(q,w);
        }
    }
    queue_free(q);
    return parents;
}