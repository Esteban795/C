#include <stdlib.h>
#include <stdio.h>
#include "../utilities/generators.c"
#include "../utilities/freers.c"
#include "math.h"

//This algorithm uses a tree with vertices from 0 to n - 1, in an array where set[i] = u if and 
//only if i is a descendant of u
// The root is the vertex 0.
// BINARY TREES ONLY !!! 


struct shortcuts {
    int* level;
    int** ancestors;
    int depth;
};

typedef struct shortcuts shortcuts;

shortcuts* create_shortcut(int n,int* prec){
    shortcuts* s = malloc(sizeof(shortcuts));
    s->level = generate_arr(n,-1);
    s->level[0] = 0;    //root is vertex 0.
    int maximum_depth = 0;
    for (int i = 0; i < n;i++){
        int vertex_depth = 1 + s->level[prec[i]];  //each node is down one more than its predecessor
        s->level[i] = vertex_depth;
        maximum_depth = maximum_depth > vertex_depth ? maximum_depth : vertex_depth;
    }
    s->depth = maximum_depth;
    s->ancestors = generate_matrix(n,0);
    for (int i = 0; i < n;i++){
        s->ancestors[0][i] = prec[i];
    }
    for (int k = 1; k < maximum_depth;k++){
        for (int i = 0; i < n;i++){
            s->ancestors[k][i] = s->ancestors[k - 1][s->ancestors[k - 1][i]];
        }
    }
    return s;
}

int query(shortcuts* s,int u,int v){
    if (s->level[u] < s->level[v]){
        int temp = u;
        u = v;
        v = temp;
    }
    for (int k = s->depth - 1;k >= 0;k--){ //on ramène v au niveau de u
        if (s->level[u] <= s->level[v] - (1 << k)){
            v = s->ancestors[k][v];
        }
    }
    if (u == v) return u; //on a trouvé notre LCA
    for (int k = s->depth - 1;k >= 0;k--){
        if (s->ancestors[k][u] != s->ancestors[k][v]){
            u = s->ancestors[k][u];
            v = s->ancestors[k][v];
        }
    }
    return s->ancestors[0][u];
}
int lca(int* arr, int n,int u,int v){
    shortcuts* s = create_shortcut(n,arr);
    int result = query(s,u,v);
    free(s->level);
    free_matrix(s->ancestors,n);
    free(s);
    return result;
    
}

int* read_int_arr(int n){
    int* arr = malloc(sizeof(int) * n);
    int temp;
    for (int i = 0; i < n;i++){
        scanf("%d",&temp);
        arr[i] = temp;
    }
    return arr;
}

int main(int argc,char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int n = atoi(argv[1]);
    int* arr = read_int_arr(n);
    int u;
    int v;
    scanf("Quel sommet souhaitez vous ? %d",&u);
    scanf("Quel sommet souhaitez vous ? %d",&v);
    int res = lca(arr,n,u,v);
    printf("Le LCA est : %d",res);
    return 0;
}