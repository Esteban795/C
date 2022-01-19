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


int main(void){
    return 1;
}
