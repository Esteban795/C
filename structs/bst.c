#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef int item;
struct BST
{
    item key;
    struct BST *right;
    struct BST *left;
};

typedef struct BST bst;

bst* new_node(item x){
    bst* node = malloc(sizeof(bst));
    node->key = x;
    node->right = NULL;
    node->left = NULL;
    return node;
}

void bst_free(bst* t){
    if (t == NULL) {return;}
    bst_free(t->right);
    bst_free(t->left);
    free(t);
}

bool bst_member(bst* t,item x){
    if (t == NULL) {return false;}
    if (x == t->key) {return true;}
    if (x > t->key) {
        return bst_member(t->right,x);
    } else {
        return bst_member(t->left,x);
    }
}

int minimum(int a, int b){
    if (a <= b){
        return a;
    }
    return b;
}

item bst_minimum(bst* t){
    if (t == NULL) {return -1;}
    bst* temp = t;
    while (t != NULL) {
        temp = t;
        t = t->left;
    }
    return temp->key;
}

bst* bst_insert(bst* t, item x){
    if (t == NULL) return new_node(x);
    bst* current = t;
    while (true) {
        item y = current->key;
        if (x == y) return t;
        if (x < y && current->left == NULL) {
            current->left = new_node(x);
            return t;
        } else if (x < y){
            current = current->left;
        }
        if (x > y && current->right == NULL){
            current->right = new_node(x);
            return t;
        } else {
            current = current->right;
        }
    }
}

bst* build(item t[],int len) {
    bst* tree = NULL;
    for (int i = 0; i < len; i++) {
        tree = bst_insert(tree,t[i]);
    }
    return tree;
}

bst* delete_min(bst* t){
    if (t == NULL) return t;
    while (t->left != NULL){
        bst* temp = t;
        t = t->left;
        free(temp);
    }
    return t->right;
}

bst* delete(bst* t,item x){
    if (t == NULL) return NULL;
    if (t->key < x){ /*moving through the bst */
        t->right = delete(t->right,x);
        return t;
    }
    if (t->key > x){
        t->left = delete(t->left,x);
        return t;
    }
    if (t->left == NULL){ /*t->key == NULL */
        bst* temp = t->right;
        free(t);
        return temp;
    }
    if (t->right == NULL){
        bst* temp = t->left;
        free(t);
        return temp;
    }
    item min = bst_minimum(t);
    t->right = delete_min(t->right);
    t->key = min;
    return t;
}

int main(void){
    return 1;
}
