#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define ALPHABET_SIZE 26

struct TrieNode{
    struct TrieNode* children[ALPHABET_SIZE];
    bool is_word;
};

typedef struct TrieNode node;

struct Trie{
    struct TrieNode* root;
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
    return t;
}

void insert(node* root,char* string){
    int level = 0;
    node* current = root;
    while (string[level]){
        int index = char_to_index(string[level]);
        if (current->children[index] == NULL){
            current->children[index] = new_node();
        } 
        current = current->children[index];
        level++;
    }
    current->is_word = true;
}

void trie_insert(trie* t,char* word){
    insert(t->root,word);
}

bool search(node* root,char* string){
    int level = 0;
    node* current = root;
    while (string[level]){
        int index = char_to_index(string[level]);
        if (current->children[index] == NULL){
            return NULL;
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

node* _delete_word(node* root,char* string,int depth){//Bottom to top
    if (root == NULL) return NULL;
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
    root->children[index] = _delete_word(root->children[index],string,depth + 1); //moving through the trie
    if (!has_children(root) && root->is_word == false){ //single node with no children 
        free(root);                                       //and doesn't belong to another word
        root = NULL;                                           
    }
    return root;
}

void delete_word(trie* t,char* word){
    assert(t != NULL);
    _delete_word(t->root,word,0);
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

int main(void){
    trie* t = new_trie();
    trie_insert(t,"test");
    trie_insert(t, "testons");
    trie_insert(t, "ab");
    assert(trie_search(t, "test"));
    assert(trie_search(t, "testons"));
    assert(trie_search(t, "ab"));
    assert(!trie_search(t, "a"));
    assert(!trie_search(t, "testb"));
    delete_word(t,"test");
    assert(!trie_search(t,"test"));
    delete_trie(t);
    return 0;
}
