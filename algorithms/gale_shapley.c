#include <stdlib.h>
#include <stdio.h>
#include "../structs/queue.c"
#include "../utilities/freers.c"
#include "../utilities/generators.c"
#include "../utilities/printers.c"
#include "../utilities/random.c"


void read_preferences(int** preferences,int n){
    int temp;
    for (int i = 0; i < n;i++){
        for (int j = 0; j < n;j++){
            scanf("%d",&temp);
            preferences[i][j] = temp;
        }
    }
}

int** read_input(int n){
    int** preferences = generate_matrix(n,0);
    read_preferences(preferences,n);
    return preferences;
}

int* gale_shapley(int n, int** men, int** women){
    int** rang = generate_matrix(n,0);
    int* mari = generate_tab(n,-1);
    int* suivante = generate_tab(n,0);
    for (int j = 0; j < n;j++){
        for (int i = 0; i < n;i++){
            rang[j][women[j][i]] = i;
        }
    }
    queue* q = queue_create();
    for (int i = 0; i < n;i++){ //on ajoute tous les hommes à la queue
        queue_push(q,i);
    }
    while (queue_length(q) > 0){
        int m = queue_pop(q);
        int f = men[m][suivante[m]]; //on prend la prochaine femme préférée de m
        if (mari[f] == -1){
            mari[f] = m;
        } else if (rang[f][mari[f]] < rang[f][m]) {
            queue_push(q,m);
        } else {
            queue_push(q,mari[f]);
            mari[f] = m;
        }
    }
    queue_free(q);
    free_matrix(rang,n);
    free(suivante);
    return mari;
}

int main(int argc, char* argv[]){
    if (argc != 2) return EXIT_FAILURE;
    int n = atoi(argv[1]); //number of men and women
    int** men_preferences = read_input(n);
    int** women_preferences = read_input(n);
    int* couplage = gale_shapley(n,men_preferences,women_preferences);
    print_tab(couplage,n);
    free_matrix(men_preferences,n);
    free_matrix(women_preferences,n);
    free(couplage);
    return 0;
}
