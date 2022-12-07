#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "graph.h"

void explore(graph_t* g,int* arr,vertex s,int current_component){
    if (arr[s] < 0) return;
    arr[s] = current_component;
    for (int i = 0; i < g->degrees[s];i++){
        explore(g,arr,g->adj[s][i],current_component);
    }
}

int* get_components(graph_t* g, int* nb_components){
    int* arr = malloc(g->n * sizeof(int)); //
    int current_component = 0;
    for (int i = 0; i < g->n; i++){
        arr[i] = -1;
    }
    for (int i = 0; i < g->n;i++){
        if (arr[i] < 0) {
            current_component++;
            explore(g,arr,i,current_component);
        }
    }
    *nb_components = current_component;
    return arr;
}

graph_t* without_edges(graph_t* g){
    int n = g->n;
    graph_t* g_2 = malloc(sizeof(graph_t));
    g_2->n = n;
    g_2->adj = malloc(sizeof(edge*) * n);
    g_2->degrees = malloc(sizeof(int) * n);
    for (int i = 0; i < n;i++){
        g_2->degrees[i] = 0;
        g_2->adj[i] = malloc(sizeof(edge) * g_2->degrees[i]); //au max g->degrees[i] arêtes car ce sera un sous ensemble, donc on en créera pas qui n'existaient pas avant
    }
    return g_2;
}

edge* get_minimal_edges(graph_t* g,int* components, int nb_components){
    edge* edges = malloc(sizeof(edge) * nb_components); //edges[c] = e <=> l'arête de poids minimal pour passer de la composante connexe c à e.y est celle-ci
    for (int i = 0; i < nb_components;i++){ //on crée des "fausses" arêtes pour initaliser le tableau, sinon on aura un souci à la première comparaison
        edge e;
        e.x = -1;
        e.y = -1;
        e.rho = INFINITY;
        edges[i] = e;
    }
    for (int i = 0; i < g->n;i++){
        for (int j = 0; j < g->degrees[i]; j++){
            int connex_component = components[i];
            edge e = g->adj[i][j];
            if (components[e.y] != connex_component && e.rho < edges[connex_component].rho){ //on vérif s'ils sont bien dans deux CC distinctes, et si le poids de cette arête est plus faible
                edges[connex_component] = e;
            }
        }
    }
    return edges;
}

bool add_edges( graph_t* g,graph_t* t){
    int nb_components;
    int* components = get_components(g,&nb_components);
    if (nb_components == 1) return true;
    edge* edges = get_minimal_edges(g,components,nb_components);
    for (int i = 0; i < nb_components;i++){
        edge e = edges[i];
        int other_cc = components[e.y];
        if (other_cc < i && components[edges[other_cc].y] == components[e.x]) continue;  //on évite de rajouter deux fois l'arête donc si j < i, on a forcément déjà traité le cas, et si j et i sont dans la même cc on fait r
        edge reversed_e;
        reversed_e.x = e.y;
        reversed_e.y = e.x;
        reversed_e.rho = e.rho;
        t->adj[e.x][t->degrees[e.x]] = e;
        t->adj[e.y][t->degrees[e.y]] = reversed_e;
        t->degrees[e.x]++;
        t->degrees[e.y]++;
    }
    return false;
}

graph_t* boruvka(graph_t* g){
    graph_t* acm = without_edges(g);
    while (!add_edges(g,acm)) {}
    return acm;
}

int main(void){
    FILE* f = fopen("exemple.txt","r");
    graph_t* g = read_graph(f);
    int nb_edges;
    edge* edges = get_edges(g,&nb_edges);
    sort_edges(edges,nb_edges);
    print_edge_array(edges,nb_edges);
    fclose(f);
    return 0;
}
