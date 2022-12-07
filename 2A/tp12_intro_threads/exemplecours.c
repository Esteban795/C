#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int n = 1000000;
int p = 3;
const int nb_fils = 3;

void* f(void* index){
    int ind = *(int*)index;
        printf("Le fil %d a démarré\n",ind);
        for (int i = 0; i < n*p;i++){
            if (i % n == 0) {
                printf("Le fil %d a atteint %d\n",ind,i);
            }
        }
    return NULL;
}

int main(int argc, char* argv[]){
    printf("Avant.\n");
    pthread_t* threads = malloc(sizeof(pthread_t) * nb_fils);
    int index[nb_fils];
    for (int i = 0; i < nb_fils;i++){
        index[i] = i;
        pthread_create(&threads[i],NULL,f,(void*)&index[i]);
    }
    printf("Pendant.\n");
    for (int i = 0; i < nb_fils;i++){
        pthread_join(threads[i],NULL);
    }
    printf("Après\n");
    free(threads);
}