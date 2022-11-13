#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "graph.h"
#include "union_find.h"


edge* kruskal(graph_t* g,int* nb_chosen){
    int p;
    edge* edges = get_edges(g,&p); //on récup toute nos arêtes
    sort_edges(edges,p);
    partition_t* part = partition_new(p);
    edge* acm = malloc(sizeof(edge) * ((g->n) - 1)); //tableau probablement plus grand que nécessaire
    int next_free_cfc = 0;
    for (int i = 0; i < p;i++){
        if (nb_sets(part) == 1) break; //on a plus qu'une composante connexe, sert à rien de continuer
        edge e = edges[i];
        if (find(part,e.x) != find(part,e.y)) { //pas dans la même composante connexe, on les merge
            merge(part,e.x,e.y); 
            acm[next_free_cfc] = e;
            next_free_cfc++;
        }
    }
    free(edges);
    partition_free(part);
    *nb_chosen = next_free_cfc;
    return acm;
}

int main(void){
    FILE* f = fopen("exemple.txt","r");
    graph_t* g = read_graph(f);
    int nb_chosen;
    edge* acm = kruskal(g,&nb_chosen);
    double weight = 0;
    for (int i = 0; i < nb_chosen;i++){
        weight += acm[i].rho;
    }
    printf("Poids total : %f",weight);
    printf("\n");
    print_edge_array(acm,nb_chosen);
    free(acm);
    graph_free(g);
    return 0;
}
