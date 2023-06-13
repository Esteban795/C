
/********************************************************************/
/* Concours Centrale-Supélec                                        */
/* Sujet 0 - MPI                                                    */
/* https://www.concours-centrale-supelec.fr                         */
/* CC BY-NC-SA 3.0                                                  */
/********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

struct node_s {
  char* key;
  int val;
  struct node_s* left;
  struct node_s* right;
};
typedef struct node_s node;


int min(int a, int b){
  return a < b ? a : b;
}

int max(int a,int b){
  return a > b ? a : b;
}

int lexcmp(char* s1, char* s2) {
  int l1 = strlen(s1);
  int l2 = strlen(s2);
  for (int i = 0; i < min(l1,l2); i++){
    if (s1[i] < s2[i]) return -1;
  }
  if (l1 < l2) return -1;
  else if (l1 == l2) return 0;
  else return 1;
}

void print_bst_aux(node* bst, int p) {
  if (bst == NULL) {
    return;
  }
  print_bst_aux(bst->right, p + 1);
  for (int i = 0; i < p; i = i + 1) {
    printf("\t");
  }
  printf("%s : %d\n", bst->key, bst->val);
  print_bst_aux(bst->left, p + 1);
}

void print_bst(node* bst) {
  print_bst_aux(bst, 0);
}

node* delete_min(node* bst, node** min) {
  assert(bst != NULL);
  if (bst->left == NULL) {
    *min = bst;
    return bst->right;
  } else {
    bst->left = delete_min(bst->left, min);
    return bst;
  }
}

node* delete(char* key, node* bst) {
  if (bst == NULL) {
    return NULL;
  } else {
    int c = lexcmp(key, bst->key);
    if (c == 0) {
      node* new;
      if (bst->left == NULL) {
        new = bst->right;
      } else if (bst->right == NULL) {
        new = bst->left;
      } else {
        node* right = delete_min(bst->right, &new);
        new->right = right;
        new->left = bst->left;
      }
      free(bst->key);
      free(bst);
      bst = new;
    } else if (c < 0) {
      bst->left = delete(key, bst->left);
    } else {
      bst->right = delete(key, bst->right);
    }
    return bst;
  }
}

bool bst_member(char* key,node* bst){
  if (bst == NULL) return false;
  int cmp = lexcmp(key,bst->key);
  if (cmp == 0) return true;
  else if (cmp == -1) return bst_member(key,bst->left);
  else return bst_member(key,bst->right);
}

node* bst_add(char* key,node* bst){
  if (bst == NULL) {
    node* new = malloc(sizeof(node));
    new->key = malloc(sizeof(char) * 100);
    strcpy(new->key,key);
    new->left = NULL;
    new->right = NULL;
    new->val = 1;
    return new;
  } else {
    int cmp = lexcmp(key,bst->key);
    if (cmp == 0) bst->val++;
    else if (cmp == -1) bst->left = bst_add(key,bst->left);
    else if (cmp == 1) bst->right = bst_add(key,bst->right);
    return bst;
  }
}

void bst_free(node* bst){
  if (bst != NULL){
    bst_free(bst->left);
    bst_free(bst->right);
    free(bst->key);
    free(bst);
  }
}


node* create_lex(char* filename){
  FILE* f_in = fopen(filename,"r");
  char line[100];
  node* bst = NULL;
  while (fscanf(f_in,"%s\n",line) != EOF){
    bst = bst_add(line,bst);
  }
  fclose(f_in);
  return bst;
}

void bst_write(FILE* f,node* bst){
  if (bst != NULL) {
    fprintf(f,"%s %d\n",bst->key,bst->val);
    bst_write(f,bst->left);
    bst_write(f,bst->right);
  }
}

void serialize_lex(char* filename,node* bst){
  FILE* f_out = fopen(filename,"w");
  bst_write(f_out,bst);
}

int bst_nb_nodes(node* bst){
  if (bst != NULL) return 1 + bst_nb_nodes(bst->left) + bst_nb_nodes(bst->right);
  return 0;
}

int bst_height(node* bst){
  return 1 + max(bst_height(bst->left),bst_height(bst->right));
}


void max_freq(node* bst,node* contender){
  if (bst == NULL) return;
  else if (bst->val > contender->val) {
    contender->val = bst->val;
    contender->key = bst->key;
  }
  max_freq(bst->left,contender);
  max_freq(bst->right,contender);
}

void most_frequent_word(node* bst){
  node* candidate = malloc(sizeof(node));
  candidate->key = bst->key;
  candidate->left = NULL;
  candidate->right = NULL;
  candidate->val = bst->val;
  max_freq(bst,candidate);
  printf("Most frequent word : %s with %d",candidate->key,candidate->val);
  free(candidate);
}


void words_with_specific_frequency(node* bst,int frequency){
  if (bst == NULL) return;
  else {
    if (bst->val == frequency) printf("%s",bst->key);
    words_with_specific_frequency(bst->left,frequency);
    words_with_specific_frequency(bst->right,frequency);
  }
}

node* create_bigram(char* filename){
  //to do 
  FILE* f_in = fopen(filename,"r");
  char line[200];
  node* bst = NULL;
  while (fscanf(f_in,"%s\n",line) != EOF){
    bst = bst_add(line,bst);
  }
  fclose(f_in);
  return bst;
}
int main(void) {
  
  node* bst = create_lex("./data/les_miserables.txt");
  print_bst(bst);
  serialize_lex("./data/les_miserables.lex",bst);
  most_frequent_word(bst);
  bst_free(bst);
  return EXIT_SUCCESS;
}
