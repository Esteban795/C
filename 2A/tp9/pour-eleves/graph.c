#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

void graph_free(graph_t *g){
    free(g->degrees);
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

graph_t *read_graph(FILE *fp){
    int n;
    fscanf(fp, "%d", &n);
    int *degrees = malloc(n * sizeof(int));
    edge **adj = malloc(n * sizeof(edge*));
    for (int i = 0; i < n; i++){
        fscanf(fp, "%d", &degrees[i]);
        printf("%d : %d\n", i, degrees[i]);
        adj[i] = malloc(degrees[i] * sizeof(edge));
        for (int j = 0; j < degrees[i]; j++) {
            edge e;
            e.x = i;
            fscanf(fp, " (%d, %f)", &e.y, &e.rho);
            adj[i][j] = e;
        }
    }
    graph_t *g = malloc(sizeof(graph_t));
    g->degrees = degrees;
    g->n = n;
    g->adj = adj;
    return g;
}

void print_graph(graph_t *g, FILE *fp){
    fprintf(fp, "%d\n", g->n);
    for (int i = 0; i < g->n; i++) {
        fprintf(fp, "%d ", g->degrees[i]);
        for (int j = 0; j < g->degrees[i]; j++) {
            edge e = g->adj[i][j];
            fprintf(fp, "(%d, %.3f) ", e.y, e.rho);
        }
        fprintf(fp, "\n");
    }
}

int number_of_edges(graph_t* g){
    int p = 0;
    for (int s = 0; s < g->n;s++){
        p += g->degrees[s];
    }
    return p/2;
}

edge* get_edges(graph_t* g,int* nb_edges){
    int p = number_of_edges(g);
    *nb_edges = p;
    edge* arr = malloc(sizeof(edge) * p);
    int next_free_index = 0;
    for (int s = 0; s < g->n; s++){
        for (int i = 0; i < g->degrees[s]; i++){
            edge e = g->adj[s][i];
            if (e.x < e.y) {
                arr[next_free_index] = e;
                next_free_index++;
            }
        }
    }
    return arr;
}

int compare(const void* e1,const void* e2){
    weight_t p1 = ((edge*)e1)->rho;
    weight_t p2 = ((edge*)e2)->rho;
    return p1 - p2;
}

void sort_edges(edge* edges,int p){
    qsort(edges,p,sizeof(edge),compare);
}

void print_edge_array(edge* edges,int len){
    for (int i = 0; i < len;i++){
        printf("%d --- %.2f ---> %d",edges[i].x,edges[i].rho,edges[i].y);
        printf("\n");
    }
    printf("\n");
}

