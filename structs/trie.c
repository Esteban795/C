#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const int ALPHABET_SIZE = 26;

struct TrieNode{
    struct TrieNode *children[ALPHABET_SIZE];
    bool is_word;
};

typedef struct TrieNode node;

int char_to_index(char c){
    return ((int)c - (int)'a');
}

node* new_node(){
    node* tn = malloc(sizeof(node));
    tn->is_word = false;
    return tn;
}

void insert(node* root,char* string){
    int len = strlen(string);
    node* current = root;
    for (int level = 0; level < len; level++){
        int index = char_to_index(string[level]);
        if (current->children[index] == NULL){
            current->children[index] = new_node();
        }
        current = current->children[index];
    }
    current->is_word = true;
}

bool search(node* root,char* string){
    int len = strlen(string);
    node* current = root;
    for (int level = 0; level < len; level++){
        int index = char_to_index(string[level]);
        if (current->children[index] == NULL){
            return false;
        }
        current = current->children[index];
    }
    return (current->is_word);
}

bool has_children(node* root){
    for (int i = 0; i < ALPHABET_SIZE;i++){
        if (root->children[i] != NULL) return true;
    }
    return false;
}

node* delete(node* root,char* string,int depth){//Bottom to top
    if (root == NULL) return NULL;
    int len = strlen(string);
    depth = depth ? depth : 0; //Default value ??
    if (depth == len) {
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
    root->children[index] = delete(root->children[index],string,depth + 1); //moving through the trie
    if (!has_children(root) && root->is_word == false){ //single node with no children 
        free(root);                                       //and doesn't belong to another word
        root = NULL;                                           
    }
    return root;
}

void _delete_subtrie(node* root){
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE;i++){
        if (root->children[i] != NULL){
            _delete_subtrie(root->children[i]);
        }
    }
    free(root);
}

void delete_trie(node* root){
    if (root == NULL) return;
    _delete_subtrie(root);
}