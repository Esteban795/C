#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "dicts.h"

struct TTT {
    int n;
    int k;
    int* grille;
};

typedef struct TTT ttt;

ttt* init_jeu(int k, int n){
    ttt* jeu = malloc(sizeof(ttt));
    jeu->grille = malloc(sizeof(int) * n);
    for (int i = 0; i < n;i++){
        jeu->grille[i] = 0;
    }
    jeu->k = k;
    jeu->n = n;
    return jeu;
}

void liberer_jeu(ttt* jeu){
    free(jeu->grille);
    free(jeu);
}

int* repartition(ttt* jeu){
    int* histogram = malloc(sizeof(int) * 3);
    for (int i = 0; i < 3;i++){
        histogram[i] = 0;
    }
    for (int i = 0; i < jeu->n;i++){
        histogram[jeu->grille[i]] += 1;
    }
    return histogram;
}

int joueur_courant(ttt* jeu){
    int* histogram = repartition(jeu);
    if (histogram[0] == 0) {
        free(histogram);
        return 0;
    }
    if (histogram[1] > histogram[2]) {
        free(histogram);
        return 2;
    }
    free(histogram); 
    return 1;
}

void jouer_coup(ttt* jeu, int lgn, int cln){
    int cp = joueur_courant(jeu);
    if (cp == 0) return EXIT_FAILURE;
    int index = jeu->n * lgn + cln;
    if (jeu->grille[index] != 0) return EXIT_FAILURE;
    jeu->grille[index] = cp;
}

bool alignement(ttt* jeu, int i, int di, int joueur){
    int k = jeu->k;
    int n = jeu->n;
    int ligne = i % n;
    int colonne = i / n;
    int dl = (di + 1) / n;
    int dc = (di + 1) % n - 1;
    for (int j = 0; j < jeu->k;j++){
        if (ligne < 0 || ligne >= n || colonne < 0 || colonne >= n) return false;
        i = ligne * jeu->n + colonne;
        if (jeu->grille[i] != joueur){
            return false;
        }
        colonne += dc;
        ligne += dl;
    }
    return true;
}

bool gagnant(ttt* jeu, int joueur){
    int n = jeu->n;
    int dep[] = {n, n - 1, 1, n + 1};
    for (int i = 0; i < n * n;i++){
        for (int j = 0; j < 4; j++){
            if (alignement(jeu,i,dep[j],joueur)) return true;
        }
    }
    return false;
}

int encodage(ttt* jeu){
    int nb = 0;
    for (int i = 0; i < jeu->n * jeu->n;i++){
        nb = 3 * nb + jeu->grille[i];
    }
    return nb;
}

int attracteur(ttt* jeu, dict* d){
    int encoded = encodage(jeu);
    int n = jeu->n;
    if (!member(d,encoded)) {
        if (gagnant(jeu,1)) add(d,encoded,1);
        else if (gagnant(jeu,2)) add(d,encoded,2);
        else {
            int tab_attr[] = {0,0,0};
            int current_player = joueur_courant(jeu);
            for (int i = 0;i < n * n; i++){
                if (jeu->grille[i] == 0){
                    jeu->grille[i] = current_player;
                    int attr = attracteur(jeu,d);
                    jeu->grille[i] = 0;
                    if (attr == current_player) break;
                }
            }
            if (tab_attr[current_player] > 0) add(d,encoded,current_player);
            if (tab_attr[0] > 0) add(d,encodage,0); 
            if (tab_attr[3 - current_player] > 0) add(d,encoded,3 - current_player);
        }
    }
    return get(d,encoded);
}

int strategie_optimale(ttt* jeu, dict* d){
    int attr = attracteur(jeu,d);
    int joueur = joueur_courant(jeu);
    for (int i = 0; i < jeu->n * jeu->n;i++){
        if (jeu->grille[i] == 0){
            jeu->grille[i] = joueur;
            int next_attr = attracteur(jeu,d);
            jeu->grille[i] = 0;
            if (attr == next_attr) return i;
        }
    }
    return -1;
}


void print_sep(ttt* jeu){
    printf("+");
    for (int i = 0; i < jeu->n;i++){
        printf("-+");
    }
    printf("\n");
}

void print_ligne(ttt* jeu){
    for (int i = 0; i < jeu->n;i++){
        if (jeu->grille[i] == 0) continue;
        if (jeu->grille[i] == 1) printf(" 0 |");
        if (jeu->grille[i] == 2) printf(" X |");
    }
    printf("\n");
}

void afficher(ttt* jeu){
    for (int i = 0; i < jeu->n;i++){
        printf("%d ",i);
    }
    print_sep(jeu);
    for (int i = 0; i < jeu->n;i++){
        printf("%d |",i);
        print_ligne(jeu->grille + jeu->n * i);
    }
    print_sep(jeu);
}

void jouer_partie(int k, int n){
    ttt* jeu = init_jeu(k,n);
    char c;
    int IA,cln,lgn;
    while (true) {
        printf("Voulez-vous commencer ? (o/n)\n");
        scanf("%c",&c);
        if (c == 'o'){
            IA = 2;
            break;
        } else if (c == 'n') {
            IA = 1;
            break;
        }
    }
    dict* d = create();
    int joueur = 1;
    while (joueur != 0 & !gagnant(jeu,1) && !gagnant(jeu,2)){
        afficher(jeu);
        if (joueur == IA){
            int i = strategie_optimale(jeu,d);
            cln = i%n;
            lgn = i/n;
            printf("Le bot joue ligne %d et colonne %d\n",lgn,cln);
            jouer_coup(jeu,cln,lgn);
        } else {
            printf("A vous de jouer\n");
            printf("Saisir ligne colonne :\n");
            scanf("%d %d",&lgn,&cln);
            if (cln  < 0 || cln >= jeu->n || lgn < 0 || lgn > jeu->n){
                printf("Out of bounds");
            } else {
                jouer_coup(jeu,cln,lgn);
            }
        }
        joueur = joueur_courant(jeu);
    }
    afficher(jeu);
    if (gagnant(jeu,IA)) printf("Le bot a gagné");
    else if (gagnant(jeu,3 - IA)) printf("Vous avez gagné");
    else printf("match nul, chanceux");
    liberer_jeu(jeu);
    dict_free(d);
}

int main(void){
    return 0;
}