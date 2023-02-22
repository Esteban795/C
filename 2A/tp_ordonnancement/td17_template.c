#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>

struct processus {
  char *exec;
  int pid;
};

typedef struct processus processus;

struct process {
  processus *actif;
  struct process *suivant;
  struct process *precedent;
};

typedef struct process process;


processus *lance_processus(char *exec) {

  // Variable allouée une fois pour tout en tout début de programme et
  // partagée par tous les appels, grâce au mot-clé `static` [HP]
  static int next_pid = 0;

  processus* p = malloc(sizeof(processus));

  // On va copier le nom, être responsable de libérer la mémoire
  // allouée mais pas celle du pointeur passé en paramètre
  p->exec = malloc(1 + strlen(exec));
  strcpy(p->exec, exec);
  p->pid = next_pid;
  next_pid++;

  // Lancement fictif du processus
  printf("* Lancement du processus %s\n", p->exec);

  return p;

}

// Comportement aléatoire juste pour tester
bool est_fini(processus *p) {
  assert (p != NULL);  // Surtout pour ignorer l'avertissement
  if (rand() % 10 == 0) {
    return true;
  } else {
    return false;
  }
}

void arrete(processus *p) {

  // Arrêt fictif du processus
  printf("* Arrêt du processus %s\n", p->exec);

  free(p->exec);
  free(p);

}

void cpu_quantum(processus *p) {

  assert(p != NULL);

  // Fait tourner fictivement le processus p

  return;
}

void ps(process** ordonnanceur){
  if (*ordonnanceur == NULL) return;
  process* current = *ordonnanceur;
  do {
    printf("%d : %s\n",current->actif->pid,current->actif->exec);
    current = current->suivant;
  } while (current != *ordonnanceur);
  printf("\n");
}

void ajoute_process(process** ordonnanceur,processus* p){
  if (p == NULL) return;
  process* item = malloc(sizeof(process));
  item->actif = p;
  if (*ordonnanceur == NULL){
    item->suivant = item;
    item->precedent = item;
    *ordonnanceur = item;
  } else {
    process* prev = (*ordonnanceur)->precedent;
    prev->suivant = item;
    item->precedent = prev;
    item->suivant = *ordonnanceur;
    (*ordonnanceur)->precedent = item;
  }
}

void delete(process* maillon){
  assert(maillon->suivant != maillon && maillon->precedent != maillon);
  maillon->precedent->suivant = maillon->suivant;
  maillon->suivant->precedent = maillon->precedent;
  arrete(maillon->actif);
  free(maillon);
}

void delete_current(process** ordonnanceur){
  assert(*ordonnanceur != NULL);

  process* current = *ordonnanceur;
  if (current->suivant == current && current->precedent == current){ //single node queue
    arrete(current->actif);
    free(current);
    *ordonnanceur = NULL;
  } else {
    *ordonnanceur = current->suivant;
    delete(current);
  }
}

void kill(process** ordonnanceur,int pid){
  process* current = *ordonnanceur;
  if (current->actif->pid == pid){
    delete_current(ordonnanceur);
    return;
  }
  current = current->suivant;
  while (current != (*ordonnanceur)){
    if (current->actif->pid == pid){
      delete(current);
      return;
    }
    current = current->suivant;
  }
}

void killall(process** ordonnanceur,char* exec){
  if (*ordonnanceur == NULL) return;
  process* current = (*ordonnanceur)->suivant;
  while (current != (*ordonnanceur)){
    process* next = current->suivant;
    if (strcmp(exec,current->actif->exec) == 0){
      delete(current);
    }
    current = next;
  }

  if (strcmp(exec,current->actif->exec) == 0){ //on a fait le tour
    delete_current(ordonnanceur);
  }
}

void round_robin(process** ordonnanceur){
  while (*ordonnanceur != NULL){
    processus* current = (*ordonnanceur)->actif;
    printf("On lance le process %s\n",current->exec);
    cpu_quantum(current);
    if (est_fini(current)){
      printf("Le process %s est fini\n",current->exec);
      delete_current(ordonnanceur);
    }
    if (*ordonnanceur != NULL){
      *ordonnanceur = (*ordonnanceur)->suivant;
    }
  }
}


int main(void) {

  srand(time(NULL));
  processus* rocketleague = lance_processus("RocketLeague.exe");
  processus* vscode = lance_processus("VisualStudioCode.exe");
  processus* rlclone = lance_processus("RocketLeague.exe");
  processus* git = lance_processus("git.exe");
  processus* emacs = lance_processus("emacs");
  
  process* ordonnanceur = NULL;
  ajoute_process(&ordonnanceur,rocketleague);
  ajoute_process(&ordonnanceur,vscode);
  ajoute_process(&ordonnanceur,rlclone);
  ajoute_process(&ordonnanceur,git);
  ajoute_process(&ordonnanceur,emacs);
  ps(&ordonnanceur);


  kill(&ordonnanceur,vscode->pid);
  ps(&ordonnanceur);

  killall(&ordonnanceur,"RocketLeague.exe");
  ps(&ordonnanceur);

  round_robin(&ordonnanceur);
  return 0;


}
