#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int item;

struct Node {
    item key;
    struct Node* left;
    struct Node* right;
};

typedef struct Node node;

struct BST {
    node* root;
};

typedef struct BST bst;

int maximum(int a,int b){
    if (a < b) {
        return b;
    }
    return a;
}

node* new_node(item x){
    node* new = malloc(sizeof(node));
    new->key = x;
    new->right = NULL;
    new->left = NULL;
    return new;
}

bst* bst_make_empty(void){
    bst* new_bst = malloc(sizeof(bst));
    new_bst->root = NULL;
    return new_bst;
}

void free_node(node* n){
    if (n == NULL) return;
    free_node(n->right);
    free_node(n->left);
    free(n);
}

void bst_free(bst* t){
    free_node(t->root);
    free(t);
}

node* node_insert(node* t,item x){
    if (t == NULL) return new_node(x);
    node* current = t;
    while (true) {
        item y = t->key;
        if (x == y) return t;
        if (x < y && current->left == NULL){
            current->left = new_node(x);
            return t;
        } else if (x < y) {
            current = current->left;
        } else if (x > y && current->right == NULL) {
            current->right = new_node(x);
            return t;
        } else {
            current = current->right;
        }
    }
}

void bst_insert(bst* t,item x){
    t->root = node_insert(t->root,x);
}

bst* bst_from_array(item t[],int len) {
    bst* tree = bst_make_empty();
    for (int i = 0; i < len; i++) {
        bst_insert(tree,t[i]);
    }
    return tree;
}

item node_min(node* n){
    if (n == NULL) {return -1;}
    while (n->left != NULL) {
        n = n->left;
    }
    return n->key;
}

item bst_min(bst* t){
    return node_min(t->root);
}

bool node_member(node* n, item x){
    if (n == NULL) {return false;}
    if (x == n->key) {return true;}
    if (x > n->key) {
        return node_member(n->right,x);
    } else {
        return node_member(n->left,x);
    }
}

bool bst_member(bst* t,item x){
    return node_member(t->root,x);
}

int node_size(node* n){
    if (n == NULL) return 0;
    return 1 + node_size(n->left) + node_size(n->right);
}

int bst_size(bst* t){
    return node_size(t->root);
}

int node_height(node* n){
    if (n == NULL) return -1;
    return 1 + maximum(node_height(n->left),node_height(n->right));
}

int bst_height(bst* t){
    return node_height(t->root);
}

void node_write_to_array(node* n, item arr[],int *offset_ptr){
    if (n == NULL) {return;}
    node_write_to_array(n->left,arr,offset_ptr);
    arr[*offset_ptr] = n->key;
    *offset_ptr += 1;
    node_write_to_array(n->right,arr,offset_ptr);
}

item* bst_to_array(bst *t,int* nb_elts){
    *nb_elts = 0;
    item* tab = malloc(sizeof(item) * bst_size(t));
    node_write_to_array(t->root,tab,nb_elts);
    return tab;
}

node* node_extract_min(node* n, int *min_ptr){
    if (n == NULL) {
        *min_ptr = NULL;
        return NULL;
    }
    node* actuel = n;
    if (n->left == NULL) {
        node* temp = n->right;
        n->right = NULL;
        free_node(n);
        return temp;
    }
    while (actuel->left->left != NULL){
        actuel = actuel->left;
    }
    node* to_delete = actuel->left;
    actuel->left = to_delete->right;
    to_delete->right = NULL;
    free_node(to_delete);
    *min_ptr = actuel->key;
    return n;
}

node* node_delete(node* n,item x){
    
}

int main(void){
    int len;
    int min_arbre;
    item tab[5] = {3,2,1,4,5};
    bst* t = bst_from_array(tab,5);
    
    bst_free(t);
    return 0;
}