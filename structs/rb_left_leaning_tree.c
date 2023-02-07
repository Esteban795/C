#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

typedef int datatype;

struct Node {
    struct Node* left;
    struct Node* right;
    datatype key;
    bool red;
};

typedef struct Node node;


struct rb_Tree {
    node* root;
};

typedef struct rb_Tree rb_tree;


node* new_node(datatype k){
    node* n = malloc(sizeof(node));
    n->left = NULL;
    n->right = NULL;
    n->key = k;
    n->red = true;
    return n;
}

bool is_red(node* n){
    return n != NULL && n->red;
}


//Checks for red node don't have red children
bool check_node_balance(node* n){
    if (n == NULL) return true;
    if (is_red(n->right)) return false;
    if (is_red(n) && is_red(n->left)) return false;
    return check_node_balance(n->left) && check_node_balance(n->right);
}

//Checks for ordering properties of r&b trees
bool check_node_order(node* n,datatype* max,bool* first){
    if (n == NULL) return true;
    if (!check_node_order(n->left,max,first)) return false;
    if (*first){
        *max = n->key;
        *first = false;
    } else if (*max >= n->key) {
        *max = n->key;
        return false;
    }
    return check_node_order(n->right,max,first);
}

/*
Makes sure that every contraints on red & black leaning left tree are followed, which are : 
- red node don't have red children
- right child of a node is black
- root is black
- every path from root to NULL node have the same number of black nodes
*/
bool check_rb(rb_tree* t){
    datatype maxi = 256;
    bool first = true;
    return !is_red(t->root) && check_node_order(t->root,&maxi,&first) && check_node_balance(t->root);
}



/* 
Does the following operation (the edge between x and y stays the same color) : 

          y                         x
         |  \                      /  |
        x    c     ------>        a    y
      /   \                           /  \
      a    b                         b    c
*/                    
node* rotate_right(node* y){
    assert(y->left != NULL);
    node* x = y->left;
    y->left = x->right;
    x->right = y;
    bool y_red = y->red;
    y->red = x->red;
    x->red = y_red;
    return x;
}


/* 
Does the following operation (the edge between x and y stays the same color), just the reverse of the above :

          y                         x
         |  \                      /  |
        x    c     <------        a    y
      /   \                           /  \
      a    b                         b    c
*/  
node* rotate_left(node* x){
    assert(x->right != NULL);
    node* y = x->right;
    x->right = y->left;
    y->left = x;
    bool x_red = x->red;
    x->red = y->red;
    y->red = x_red;
    return y;
}

/*
Fixes up this situation :
                                        |
            x                           x
           |  |      -------->         /  \
           y  z                       y    z
*/
void flip_colors(node* n){
    assert(n != NULL && !is_red(n) && is_red(n->left) && is_red(n->right)); //
    n->red = true;
    n->left->red = false;
    n->right->red = false;
}

node* node_insert(node* n,datatype k){
    if (n == NULL) return new_node(k); //new node is red by default
    if (k == n->key) return n; //node already exists
    if (k < n->key) n->left = node_insert(n->left,k); //traversing tree
    if (k > n->key) n->right = node_insert(n->right,k);

    if (is_red(n->right) && !is_red(n->left)) n = rotate_left(n); 
    if (is_red(n->left) && is_red(n->left->left)) n = rotate_right(n);
    if (is_red(n->right) && is_red(n->left)) flip_colors(n);
}

void rb_insert(rb_tree* t,datatype k){
    t->root = node_insert(t->root,k);
    t->root->red = false;
}

/* Balance the tree such that : 

1) 
        x                   z
       /  |       ---->    |  \
      y    z               x   ...
            \             /
             ...         y
2) etc ...
*/            
node* balancing(node* n){
    if (is_red(n->right)) n = rotate_left(n);
    if (is_red(n->left) && is_red(n->left->left)) n = rotate_right(n);
    if (is_red(n->left) && is_red(n->right)) flip_colors(n);
    return n;
}


/*
Does this : 

        x                           x                              |
          \                           |                            z  
           y       ----->              z        ----->            /  \
          |                              \                       x    y
          z                               y
*/
node* move_red_to_left(node* n){
    flip_colors(n);
    if (n != NULL && n->right != NULL && is_red(n->right->left)){
        n->right = rotate_right(n->right);
        n = rotate_left(n);
        flip_colors(n);
    }
    return n;
}

/*
See function above. It does the same thing, just with reversed rotations.
*/
node* move_red_to_right(node* n){
    flip_colors(n);
    if (n != NULL && n->left != NULL && is_red(n->left->right)){
        n->left = rotate_left(n->left);
        n = rotate_right(n);
        flip_colors(n);
    }
    return n;
}


/*
Attemps to remove the minimum from the subtree which root is node n
*/
node* remove_min(node* n){
    if (n == NULL) return NULL;
    if (n->left == NULL){ //No children
        free(n);
        return NULL;
    }
    if (!is_red(n->left) && !is_red(n->left->left)){
        n = move_red_to_left(n);
    }
    n->left = remove_min(n->left);
    return balancing(n);
}


/*
Finds the minimum of the subtree which root is node n.
Since red and black trees are sophisticated binary search trees, it just goes full left.
*/
node* find_min(node* n) {
    if (n == NULL) return NULL;
    while (n->left){
        n = n->left;
    }
    return n;
}

node* remove_key(node* n,datatype key){
    node* temp;
    if (n == NULL) return NULL;
    if (key < n->key){
        if (n->left != NULL){ //if it is NULL, it means the key doesn't actually exist in the tree
            if (!is_red(n->left) && !is_red(n->left->left)){
                n = move_red_to_left(n);
            }
            n->left = remove_key(n->left,key);
        } else {
            return NULL; 
        }
    } else {
        if (is_red(n->left)) n = rotate_right(n);
        if ( key != n->key && !is_red(n->right->left)){
            free(n);
            return NULL;
        }
        if (n->right != NULL) {
            if (!is_red(n->right) && !is_red(n->right->left)) n = move_red_to_right(n);
        }
        if (key != n->key) {
            temp = find_min(n->right);
            n->key = temp->key;
            n->right = remove_min(n->right);
        } else {
            n->right = remove_key(n->right,key);
        }
    }
    return balancing(n);
}

void delete_node(node* n){
    if (n == NULL) return;
    if (n->left) delete_node(n->left);
    if (n ->right) delete_node(n->right);
    free(n);
}

void delete_rbtree(rb_tree* t){
    delete_node(t->root);
}