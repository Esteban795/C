#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define ALPHABET_SIZE 26

int max(int a, int b){
    if (a <= b) return b;
    return a;
}

char* char_deepcopy(char* str,int size){
    char* temp = malloc(sizeof(char) * size);
    for (int i = 0;i < size;i++){
        temp[i] = str[i];
    }
    return temp;
}


struct TrieNode{
    struct TrieNode* children[ALPHABET_SIZE];
    bool is_word;
};

typedef struct TrieNode node;

struct Trie{
    struct TrieNode* root;
    int size;
};

typedef struct Trie trie;

struct CharTab{
    char** tab;
    int size;
};

typedef struct CharTab chartab;

int char_to_index(char c){
    return ((int)c - (int)'a');
}

node* new_node(){
    node* tn = malloc(sizeof(node));
    for (int i = 0; i < ALPHABET_SIZE; i++){
    	tn->children[i] = NULL;
    }
    tn->is_word = false;
    return tn;
}


trie* new_trie(void){
    trie* t = malloc(sizeof(trie));
    t->root = new_node();
    t->size = 0;
    return t;
}

chartab* new_chartab(int size){
    chartab* ct = malloc(sizeof(chartab));
    ct->tab = malloc(sizeof(char*) * size);
    ct->size = size;
    return ct;
}

/* Size anf height */
int node_height(node* n){
    if (n == NULL) return -1;
    int h = 0;
    int s = 0;
    for (int i = 0; i < ALPHABET_SIZE;i++){
        if (n->children[i] != NULL){
            s = 1 + node_height(n->children[i]);
        }
        h = max(h,s);
        s = 0;
    }
    return h;
}

int trie_height(trie* t){
    return node_height(t->root);
}

int trie_size(trie* t){
    return t->size;
}

////////////////////////////////////////////////


bool insert(node* root,char* prefix){
    int level = 0;
    node* current = root;
    bool size_increased = false;
    while (prefix[level]){
        int index = char_to_index(prefix[level]);
        if (current->children[index] == NULL){
            current->children[index] = new_node();
            size_increased = true;
        } 
        current = current->children[index];
        level++;
    }
    current->is_word = true;
    return size_increased;
}

void trie_insert(trie* t,char* word){
    bool size_increased = insert(t->root,word);
    t->size += size_increased ? 1 : 0;
}



bool search(node* root,char* prefix){
    int level = 0;
    node* current = root;
    while (prefix[level]){
        int index = char_to_index(prefix[level]);
        if (current->children[index] == NULL){
            return false;
        } 
        current = current->children[index];
        level++;
    }
    return (current->is_word);
}

bool trie_search(trie* t,char* word){
    return search(t->root,word);
}




bool has_children(node* root){
    for (int i = 0; i < ALPHABET_SIZE;i++){
        if (root->children[i] != NULL) return true;
    }
    return false;
}

node* node_delete_word(node* root,char* string,int depth,bool* word_found){//Bottom to top
    if (root == NULL){
        *word_found = false;
        return NULL;
    } 
    if (string[depth] == 0) {
        if (root->is_word) {
            root->is_word = false;
        }
        if (!has_children(root)){
            free(root);
            root = NULL;
        }
        
        return root;
    }
    int index = char_to_index(string[depth]); 
    root->children[index] = node_delete_word(root->children[index],string,depth + 1,word_found); //moving through the trie
    if (!has_children(root) && root->is_word == false){ //node with no children 
        free(root);                                       //and doesn't belong to another word
        root = NULL;

    }
    return root;
}

void trie_delete_word(trie* t,char* word){
    assert(t != NULL);
    bool word_exists = true;
    node_delete_word(t->root,word,0,&word_exists);
    t->size += word_exists ? -1 : 0;
}


void node_elements(node* n,int level,char str[],char** tab,int* j){
    if (n->is_word){
        str[level] = 0; //zero terminated string
        char* temp = char_deepcopy(str,level + 1);
        tab[*j] = temp;
        (*j)++;
    }
    for (int i = 0; i < ALPHABET_SIZE;i++){
        if (n->children[i]){ // children exists, we go deeper in the tree;
            str[level] = i + 'a'; //adds correct letter to the string
            node_elements(n->children[i],level + 1,str,tab,j);
        }
    }
}

chartab* trie_elements(trie* t){
    const int height = trie_height(t);
    char prefix[height + 1];
    int index = 0;
    chartab* ct = new_chartab(t->size);
    node_elements(t->root,0,prefix,ct->tab,&index);
    return ct;
}

void delete_node(node* root){
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE;i++){
        if (root->children[i] != NULL){
            delete_node(root->children[i]);
        }
    }
    free(root);
}

void delete_trie(trie* t){
    delete_node(t->root);
    free(t);
}

void delete_chartab(chartab* ct){
    for (int i = 0; i < ct->size; i++){
        free(ct->tab[i]);
    }
    free(ct->tab);
    free(ct);
}

void print_trie(trie* t){
    chartab* ct = trie_elements(t);
    for (int i = 0; i < ct->size;i++){
        printf(ct->tab[i]);
        printf("\n");
    }
    delete_chartab(ct);
}

int main(void){
    trie* t = new_trie();
    trie_insert(t,"test");
    trie_insert(t, "testons");
    trie_insert(t, "ab");
    trie_insert(t,"intergouvernementalisations");
    assert(trie_search(t, "test"));
    assert(trie_search(t, "testons"));
    assert(trie_search(t, "ab"));
    assert(!trie_search(t, "a"));
    assert(!trie_search(t, "testb"));
    print_trie(t);
    trie_delete_word(t,"test");
    trie_delete_word(t,"ab");
    assert(!trie_search(t,"test"));
    printf("\n\n\n\n");
    print_trie(t);
    delete_trie(t);
    return 0;
}
