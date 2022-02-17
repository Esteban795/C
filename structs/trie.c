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

bool insert(node* root,char* string){
    int level = 0;
    node* current = root;
    bool size_increased = false;
    while (string[level]){
        int index = char_to_index(string[level]);
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

bool search(node* root,char* string){
    int level = 0;
    node* current = root;
    while (string[level]){
        int index = char_to_index(string[level]);
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

node* _delete_word(node* root,char* string,int depth,bool* word_found){//Bottom to top
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
    root->children[index] = _delete_word(root->children[index],string,depth + 1,word_found); //moving through the trie
    if (!has_children(root) && root->is_word == false){ //single node with no children 
        free(root);                                       //and doesn't belong to another word
        root = NULL;

    }
    return root;
}


void node_elements(node* n,int level,char str[]){
    if (n->is_word){
        str[level] = 0; //zero terminated string
        printf(str);
        printf("\n");
    }
    for (int i = 0; i < ALPHABET_SIZE;i++){
        if (n->children[i]){ // children exists, we go deeper in the tree;
            str[level] = i + 'a'; //adds correct letter to the string
            node_elements(n->children[i],level + 1,str);
        }
    }
}

void trie_elements(trie* t){
    char str[35];
    node_elements(t->root,0,str);
}

void delete_word(trie* t,char* word){
    assert(t != NULL);
    bool word_exists = true;
    _delete_word(t->root,word,0,&word_exists);
    t->size += word_exists ? -1 : 0;
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

int trie_size(trie* t){
    return t->size;
}

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
    delete_word(t,"test");
    delete_word(t,"ab");
    assert(!trie_search(t,"test"));
    printf("Trie size is : %d\n",trie_size(t));
    printf("\n");
    trie_elements(t);
    printf("Trie height is : %d",trie_height(t));
    printf("\n");
    delete_word(t,"intergouvernementalisations");
    printf("Trie height is now  : %d",trie_height(t));
    printf("\n");
    delete_trie(t);
    return 0;
}
