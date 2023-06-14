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
};



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


