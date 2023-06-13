#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const int INFINI = 100000;

struct graph_s {
  // Nombre de sommets
  int n;
  // Degré de chaque somment
  int degre[100];
  // Listes d'adjacences (tableau de tableaux)
  int voisins[100][10];
  // Listes des poids des arcs
  int poids[100][10];
};

typedef struct graph_s graph;

int* creer_tableau_distances(struct graph_s g, int s){
    int* dist = malloc(sizeof(int) * g.n);
    for (int i = 0; i < g.n;i++){
      dist[i] = s == i ? 0 : INFINI;
    }
    return dist;
}

// Cette fonction effectue un parcours en profondeur du graphe g depuis un sommet s
// et enregistre les sommets parcourus par ordre décroissant de fin de traitement
// dans le tableau liste.
void parcours_prof_rec(graph g,int s, bool vu[100], int liste[100], int* indice){
    if (!vu[s]){
        vu[s] = true;
        for (int i = 0; i < g.degre[s]; i++){
            parcours_prof_rec(g, g.voisins[s][i], vu, liste, indice);
        }
        liste[*indice] = s;
        (*indice)--;
        
    }
    
}

void tri_topologique(graph g, bool vu[100], int liste[100]){
    for (int i = 0; i < 100; i++) {
      vu[i] = false;
      liste[i] = -1;
    }
    int index_lst = g.n - 1;
    for (int i = 0; i < g.n; i++){
        if (!vu[i]){
            parcours_prof_rec(g,i,vu,liste,&index_lst);
        }
    }
}

int* plus_court_chemin(graph g, int s){
    int* dist = creer_tableau_distances(g,s);
    bool vu[100];
    bool liste[100];
    tri_topologique(g,vu,liste);
    for (int i = 0; i < g.n;i++){
      for (int adj = 0; adj < g.degre[liste[i]];i++){
        if (dist)
      }
    }
    return dist
}

int main(void) {

  graph g_exemple = {
    .n = 9,
    .degre = {0, 2, 1, 2, 2, 3, 1, 1, 0},
    .voisins = {
    /* 0 */ {-1}, // Degré 0 : valeur ignorée
    /* 1 */ {0, 4},
    /* 2 */ {4},
    /* 3 */ {0, 4},
    /* 4 */ {6, 7},
    /* 5 */ {2, 4, 8},
    /* 6 */ {7},
    /* 7 */ {8},
    /* 8 */ {-1} // Degré 0 : valeur ignorée
    },
    .poids = {
    /* 0 */ {-1}, // degré 0 : valeur ignorée
    /* 1 */ {2,3},
    /* 2 */ {5},
    /* 3 */ {1,6},
    /* 4 */ {8,2},
    /* 5 */ {1,2,1},
    /* 6 */ {4},
    /* 7 */ {3},
    /* 8 */ {-1} // degré 0 : valeur ignorée
      }
  };
    
    
  int liste[100];
    
  bool vu[100];
  tri_topologique(g_exemple,vu,liste);
  for (int i = 0; i < g_exemple.n;i++){
    printf("%d\n",liste[i]);
  }
  graph* g = &g_exemple;



  printf("\n");
  int* d = plus_court_chemin(g_exemple, 4);
  for (int s = 0; s < g->n; s += 1) {
    //printf("(%d)\n", d[s]);
  }
  printf("\n");
  free(d);
  d = plus_court_chemin(g_exemple, 1);
  for (int s = 0; s < g->n; s += 1) {
    //printf("(%d)\n", d[s]);
  }
  free(d);
}


