#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "stack.h"
#include "graph.h"


int *read_data(int *nb_vertex, int *nb_edges){
    scanf("%d %d",nb_vertex,nb_edges);
    int x,y;
    int* tab = malloc(sizeof(int) * 2 * (*nb_edges));
    for (int i = 0; i < (*nb_edges);i++){
        scanf("%d %d",&x,&y);
        tab[2 * i] = x;
        tab[2 * i + 1] = y;
    }
    return tab;
}

stack* euler_tour(graph g){
    int size = g.nb_edges;
    stack* euler = stack_new(2 * size);
    stack* courant = stack_new(2 * size);
    stack_push(courant,0);
    while (stack_is_empty(courant)){
        int sommet = stack_peek(courant);
        if (!has_available_edge(g,sommet)){
            stack_pop(courant);
            stack_push(euler,sommet);
        } else {
            edge e = get_edge(g,sommet);
            stack_push(courant,e.to);
            delete_edge(g,e);
        }
    }
    stack_free(courant);
    return euler;
}

int main(void){
    int nb_vertex;
    int nb_edges;
    int* data = read_data(&nb_vertex,&nb_edges);
    graph g = build_graph(data,nb_vertex,nb_edges);
    stack* tour = euler_tour(g);
    while (!stack_is_empty(tour)){
        int sommet = stack_pop(tour);
        printf("%d ",sommet);
    }

    stack_free(tour);
    graph_free(g);
    return 0;
}
