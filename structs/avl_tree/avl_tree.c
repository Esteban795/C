#include "avl_tree.h"

int max(int a,int b){
    return a > b ? a : b;
}

node* new_node(int key){
    node* n = malloc(sizeof(node));
    n->height = 0;
    n->key = key;
    n->left = NULL;
    n->right = NULL;
    return n;
}

avl_tree* avl_new(void){
    avl_tree* t = malloc(sizeof(avl_tree));
    t->root = NULL;
    t->nb_elts = 0;
    return t;
}


bool node_member(node* n, int key){
    if (n == NULL) return false;
    if (key < n->key) node_member(n->left,key);
    if (key > n->key) node_member(n->right,key);
    return true;
}

bool avl_member(avl_tree* t,int key){
    return node_member(t->root,key);
}

int avl_nb_elements(avl_tree* t){
    return t->nb_elts;
}

void node_elements(node* n,int* tab_elts,int* i){
    if (n == NULL) return;
    if (n->left != NULL) node_elements(n->left,tab_elts,i);
    printf("Node : %d, i vaut : %d\n",n->key,*i + 1);
    tab_elts[*i] = n->key;
    (*i)++;
    if (n->right != NULL) node_elements(n->right,tab_elts,i);
}

int* avl_elements(avl_tree* t){
    int* elts = malloc(sizeof(int) * t->nb_elts * 2);
    int i = 0;
    node_elements(t->root,elts,&i);
    return elts;
}

int node_height(node* n){
    if (n == NULL) return -1;
    return 1 + max(node_height(n->left),node_height(n->left)); 
}

int avl_height(avl_tree* t){
    return node_height(t->root);
}

node* left_rotate(node* x){
    node* y = x->right;
    x->right = y->left;
    y->left = x;
    x->height = 1 + max(node_height(x->left),node_height(x->right));
    y->height = 1 + max(node_height(y->left),node_height(y->right));
    return y;
}

node* right_rotate(node* y){
    node* x = y->left;
    y->left = x->right;
    x->right = y;
    x->height = 1 + max(node_height(x->left),node_height(x->right));
    y->height = 1 + max(node_height(y->left),node_height(y->right));
    return x;
}

int get_node_balance(node* n){
    if (n == NULL) return 0;
    return node_height(n->left) - node_height(n->right);
}

node* insert_node(node* n,int key,bool* inserted){
    if (n == NULL){
        *inserted = true;
        return new_node(key);
    }
    if (key < n->key) n->left = insert_node(n->left,key,inserted); //traversing the tree
    if (key > n->key) n->right = insert_node(n->right,key,inserted);
    else return n; //element already exists

    //Re-balancing the tree.
    //updating n's height
    n->height = 1 + max(node_height(n->left),node_height(n->right));
    int n_balance = get_node_balance(n);
    if (n_balance > 1 && key < n->left->key) return right_rotate(n);
    if (n_balance > 1 && key > n->left->key) {
        n->left = left_rotate(n->left);
        return right_rotate(n);
    }
    if (n_balance < -1 && key > n->right->key) return left_rotate(n);
    if (n_balance < -1 && key < n->right->key){
        n->right = right_rotate(n->right);
        return left_rotate(n);
    }
    return n;
}

void avl_insert(avl_tree* t,int key){
    bool inserted = false;
    t->root = insert_node(t->root,key,&inserted);
    if (inserted) t->nb_elts++;
}

node* find_min(node* n) {
    if (n == NULL) return NULL;
    while (n->left){
        n = n->left;
    }
    return n;
}


node* node_remove(node* n,int key,bool* removed){
    if (n == NULL) return NULL;
    if (key < n->key) n->left = node_remove(n->left,key,removed);
    else if (key > n->key) n->right = node_remove(n->right,key,removed);
    else  { //we found the node that is going to get deleted
        *removed = true;
        if (n->left == NULL || n->right == NULL){
            node* temp = n->left == NULL ? n->left : n->right;
            if (temp == NULL) { //leaf node
                temp = n;
                n = NULL;
            } else {
                *n = *temp; //overwrites the parent with child data, free child
            }
            free(temp);
        } else {
            node* temp = find_min(n->right);
            n->key = temp->key;
            n->right = node_remove(n->right,temp->key,removed);
        }
    }
    if (n == NULL) return NULL; //means n was the root, so the tree is now empty

    //Rebalancing tree
    n->height = 1 + max(node_height(n->left),node_height(n->right));
    int n_balance = get_node_balance(n);
    if (n_balance > 1 && get_node_balance(n->left) >= 0) return right_rotate(n);
    if (n_balance > 1 && get_node_balance(n->left) < 0){
        n->left = left_rotate(n->left);
        return right_rotate(n);
    }

    if (n_balance < -1 && get_node_balance(n->right) <= 0) return left_rotate(n);
    if (n_balance < -1 && get_node_balance(n->right) > 0){
        n->right = left_rotate(n->right);
        return left_rotate(n);
    }
    return n;
}

void avl_remove(avl_tree* t,int key){
    bool removed = false;
    t->root = node_remove(t->root,key,&removed);
    if (removed) t->nb_elts--;
    
}

void print_sorted(node* n) {
  if (n != NULL) {
    print_sorted(n->left);
    printf("%d ", n->key);
    print_sorted(n->right);
  }
}

void avl_print_sorted(avl_tree* t){
    print_sorted(t->root);
}

void node_delete(node* n){
    if (n == NULL) return;
    node_delete(n->left);
    node_delete(n->right);
    free(n);
}

void avl_delete(avl_tree* t){
    node_delete(t->root);
    free(t);
}

/*
int main() {
    avl_tree* t = avl_new();
    avl_insert(t, 2);
    avl_insert(t, 1);
    avl_insert(t, 7);
    avl_insert(t, 4);
    avl_insert(t, 5);
    avl_insert(t, 3);
    avl_insert(t, 8);
    avl_insert(t, 2);
    printf("Nb d'éléments : %d\n",t->nb_elts);
    int* elts = avl_elements(t);
    for (int i = 0; i < t->nb_elts;i++){
        printf("%d ",elts[i]);
    }
    printf("\n\n");
    free(elts);
    avl_remove(t,3);
    printf("\nAfter deletion: ");
    avl_print_sorted(t);
    printf("\n");
    avl_delete(t);
    return 0;
}
*/

