#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

struct node {
    struct node *up;
    struct node *down;
    struct node *left;
    struct node *right;
    struct node *header;
    int index;
    int data;
};

typedef struct node Node;


void print_solution(Node **solution, int k);


void print_torus(Node *root){
    printf("root count: %d\n", root->data);
    for (Node *c = root->right; c != root; c = c->right) {
        printf("%d: ", c->data);
        for (Node *r = c->down; r != c; r = r->down) {
            printf("%d ", r->data);
        }
        printf("\n");
    }
}


void remove_lr(Node *x){
    Node* prev = x->left;
    Node* next = x->right;
    prev->right = next;
    next->left = prev;
}

void restore_lr(Node *x){
    Node* prev = x->left;
    Node* next = x->right;
    prev->right = x;
    next->left = x;
}

void remove_ud(Node *x){
    Node* prev = x->up;
    Node* next = x->down;
    prev->down = next;
    next->up = prev;
}

void restore_ud(Node *x){
    Node* prev = x->left;
    Node* next = x->right;
    prev->down = x;
    next->up = x;
}


Node **create_node_matrix(bool **mat, int nb_rows, int nb_columns){
    int n = nb_rows;
    int p = nb_columns;
    Node **node_mat = malloc((n + 1) * sizeof(Node*));
    node_mat[n] = malloc((p + 1) * sizeof(Node));
    for (int i = 0; i < n; i++) {
        node_mat[i] = malloc(p * sizeof(Node));
    }
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j < p; j++) {
            int left = (j == 0) ? p - 1 : j - 1;
            int right = (j == p - 1) ? 0 : j + 1;
            int down = (i == 0) ? n : i - 1;
            int up = (i == n) ? 0 : i + 1;
            node_mat[i][j].left = &node_mat[i][left];
            node_mat[i][j].right = &node_mat[i][right];
            node_mat[i][j].up = &node_mat[up][j];
            node_mat[i][j].down = &node_mat[down][j];
            node_mat[i][j].header = &node_mat[n][j];
            node_mat[i][j].data = i;
        }
    }
    for (int j = 0; j <= p; j++) {
        node_mat[n][j].data = 0;
        node_mat[n][j].index = j;
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < p; j++) {
            if (!mat[i][j]) {
                remove_lr(&node_mat[i][j]);
                remove_ud(&node_mat[i][j]);
            }
            else {
                node_mat[n][j].data++;
            }
        }
    }
    node_mat[n][p].left = &node_mat[n][p - 1];
    node_mat[n][p - 1].right = &node_mat[n][p];
    node_mat[n][p].right = &node_mat[n][0];
    node_mat[n][0].left = &node_mat[n][p];
    node_mat[n][p].down = NULL;
    node_mat[n][p].up = NULL;
    node_mat[n][p].header = NULL;
    return node_mat;
}

Node *from_node_matrix(Node **node_mat, int nb_rows, int nb_columns){
    return &node_mat[nb_rows][nb_columns];
}

void free_node_matrix(Node **node_mat, int nb_rows){
    for (int i = 0; i <= nb_rows; i++) {
        free(node_mat[i]);
    }
    free(node_mat);
}

void cover_column(Node *c){
    Node* j = c->down;
    remove_lr(c);
    while (j != c){
        Node* i = j->right;
        while (i != j) {
            remove_ud(i);
            j->header->data--;
            i = i->right;
        }
        j = j->down;
    }
}

void uncover_column(Node *c){
    Node* j = c->up;
    while (j != c){
        Node* i = j->right;
        while (i != j) {
            restore_ud(i);
            j->header->data++;
            i = i->right;
        }
        j = j->up;
    }
}

void select_row(Node *n){
    Node* i = n;
    do {
        cover_column(i);
        i = i->right;
    } while (i != n);
}

void unselect_row(Node *n){
    Node* i = n;
    do {
        uncover_column(i);
        i = i->left;
    } while (i != n);
}

void print_solution(Node** solution,int k){
    printf("Potential solution :\n");
    for (int i = 0; i < k;i++){
        printf("Line %d chosen\n",solution[i]->index);
    }
}

Node *choose_column(Node *root){
    Node* node_min = root->right;
    Node* temp = root->right;
    while (temp != root){
        node_min = node_min->data < temp->data ? node_min : temp;
        temp = temp->right;
    }
    return node_min;
}


void enumerate(Node *root, int k, Node **solution, bool print_flag){
    Node* next_column = choose_column(root);
    if (next_column->data == 0){
        if (print_flag) {
            print_solution(solution,k);
        }
    }
    Node* next = next_column->down;
    while (next != next_column){
        select_row(next);
        solution[k] = next;
        enumerate(root,k + 1,solution,print_flag);
        unselect_row(next);
        next = next->down;
    }
}

bool **read_matrix(FILE* f,int *n, int *p){
    int n;
    int p;
    fscanf(f,"%d %d\n",&n,&p);
    bool** m = malloc(*n * sizeof(bool*));
    int temp;
    for (int i = 0; i < *n;i++){
        m[i] = malloc(*p * sizeof(bool));
        for (int j = 0; j < *p;j++){
            fscanf(f,"%d",&temp);
            m[i][j] = temp == 1 ? true: false;
        }
    }
    return m;
}

void free_bool_matrix(bool **mat, int n){
    for (int i = 0; i < n;i++){
        free(mat[i]);
    }
    free(mat);
}

void print_matrix(bool **mat, int n, int p){
    for (int i = 0; i < n;i++){
        for (int j = 0; j < p;j++){
            printf("%d ",mat[i][j]);
        }
        printf("\n");
    }
}

int main(void){
    return 0;
}
