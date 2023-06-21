#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

const uint64_t FNV_OFFSET = 14695981039346656037ULL;
const uint64_t FNV_PRIME = 1099511628211ULL;

struct table {
  int capacite;
  char **clefs;
  int nb_elements;
};

typedef struct table table;


void reallocation(table* t){
  table tmp = creation(2 * t->capacite);
  for (int i = 0; i < t->capacite; i++){
    if (t->clefs[i] != NULL && strcmp(t->clefs[i],"") != 0) insertion(&tmp,t->clefs[i]);
  }
  free(t->clefs);
  t->clefs = tmp.clefs;
  t->capacite = tmp.capacite;
  t->nb_elements = tmp.nb_elements;
}
/** renvoie la valeur de FNV-1a sur clef
 * clef : chaîne de caractères
 */
uint64_t hash_key(char *clef){
  uint64_t hash = FNV_OFFSET;
  int i = 0;

  while(clef[i] != '\0'){
    hash = hash ^ (uint64_t) clef[i]; // et bit à bit 
    hash = hash * FNV_PRIME;
    i = i + 1;
  }
  return hash;
}

table creation(int capacite){
  table t = {.capacite = capacite, .clefs = NULL};
  t.clefs = malloc(sizeof(char*) * capacite);
  for (int i = 0; i < capacite; i++){
    t.clefs[i] = NULL;
  }
  return t;
}

void insertion(table* t, char* clef){
  int indice = hash_key(clef) % t->capacite;
  while (t->clefs[indice] != NULL && strcmp(t->clefs[indice],"") != 0){
    indice = (indice + 1) % t->capacite;
  }
  t->clefs[indice] = clef;
  t->nb_elements++;
  if (2 * t->nb_elements > t->capacite){
    reallocation(t);
  }
}

bool apparait(table t,char* clef){
  int indice = hash_key(clef) % t.capacite;
  while (t.clefs[indice] != NULL){
    if (strcmp(clef,t.clefs[indice] == 0)) return true;
    indice = (indice + 1) % t.capacite;
  }
  return false;
}

void suppression(table* t, char* clef){
  int indice = hash_key(clef) % t->capacite;
  int indice = hash_key(clef) % t->capacite;
  while (t->clefs[indice] != NULL && strcmp(t->clefs[indice],"") != 0){
    if (strcmp(clef,t->clefs[indice] == 0)){
      t->clefs[indice] =  "";
      t->nb_elements--;
      return;
    }
    indice = (indice + 1) % t->capacite;
  }
}