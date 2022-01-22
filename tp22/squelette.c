#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/*************/
/*  Partie 1 */
/*************/

typedef int datatype;

struct Node {
  datatype data;
  struct Node *next;
};

typedef struct Node node;

// Exercice 1

node* new_node(datatype data){
    node* result = malloc(sizeof(node));
    (*result).data = data;
    (*result).next = NULL;
    return result;
}

node* cons(node *list, datatype data){
    node* first_elt = new_node(data);
    first_elt->next = list;
    return first_elt;
}

node* from_array(datatype array[], int len){
    node* current = NULL;
    for (int i = len - 1; i >= 0;i--){
        current = cons(current,array[i]);
    }
    return current;
}


// Exercice 2

void free_list(node *n) {
    if (n == NULL) {return;}
    free_list(n->next);
    free(n);
}

int length(node* list){
    int len = 0;
    while (list != NULL){
        len++;
        list = (*list).next;
    }
    return len;
}

void test_length(void){
  assert(length(NULL) == 0);
  node *u = cons(cons(NULL, 3), 4);
  assert(length(u) == 2);
  int t[3] = {2, 4, 6};
  free_list(u);
  u = from_array(t, 3);
  assert(length(u) == 3);
  assert(u->data == 2);
  free_list(u);
}

void print_list(node* n, bool newline){
    printf("[");
    while (n != NULL && (*n).next != NULL){
        printf("%d ",(*n).data);
        n = n->next;
    }
    if (n != NULL){
        printf("%d",n->data);
    }
    printf("]");
    if (newline){
        printf("\n");
    }
}

datatype* to_array(node *u){
    int len = length(u);
    datatype* array = malloc(len * sizeof(datatype));
    for (int i = 0; i < len; i++){
        array[i] = (*u).data;
        u = u->next;
    }
    return array;
}

void test_to_array(void){
  int t[5] = {1, 2, 3, 4, 5};
  int n = 5;
  node *u = from_array(t, n);
  int *t2 = to_array(u);
  for (int i = 0; i < n; i++){
    assert(t[i] == t2[i]);
  }
  free(t2);
  free_list(u);
}

bool is_equal(node *u, node *v){
    while (u != NULL && v != NULL) {
        if ((*u).data != (*v).data) {return false;}
        u = (*u).next;
        v = (*v).next;
    }
    return (v == NULL && u == NULL);
}

void test_is_equal(void){
  int a[4] = {1, 2, 3, 4};
  int b[4] = {1, 2, 4, 3};

  node *u = from_array(a, 3);
  node *v = from_array(a, 4);
  assert(!is_equal(u, v));
  free_list(u);
  free_list(v);

  u = from_array(a, 4);
  v = from_array(b, 4);
  assert(!is_equal(u, v));
  free_list(u);
  free_list(v);

  u = from_array(a, 2);
  v = from_array(b, 2);
  assert(is_equal(u, v));

  free_list(u);
  free_list(v);
}


// Exercice 3

bool is_sorted(node *u){
    if (u == NULL) {return true;}
    while (u->next != NULL){
        if (u->data > u->next->data) {return false;}
        u = u->next;
    }
    return true;
}

void test_is_sorted(void){
  int t[5] = {1, 2, 4, 3, 5};

  node *u = from_array(t, 0);
  assert(is_sorted(u));
  free_list(u);

  u = from_array(t, 1);
  assert(is_sorted(u));
  free_list(u);

  u = from_array(t, 3);
  assert(is_sorted(u));
  free_list(u);

  u = from_array(t, 4);
  assert(!is_sorted(u));
  free_list(u);

  u = from_array(t, 5);
  assert(!is_sorted(u));
  free_list(u);
}

node *insert_rec(node *u, datatype x){
    if (u == NULL || u->data >= x) {return cons(u,x);}
    u->next = insert_rec(u->next,x);
    return u;
}

void test_insert_rec(void){
  node *u = NULL;
  u = insert_rec(u, 4);
  u = insert_rec(u, 2);
  u = insert_rec(u, 3);
  u = insert_rec(u, 5);

  int t[4] = {2, 3, 4, 5};
  node *v = from_array(t, 4);
  assert(is_equal(u, v));

  free_list(u);
  free_list(v);
}

node *insertion_sort_rec(node *u){
    if (u == NULL) {return NULL;}
    return insert_rec(insertion_sort_rec(u->next),u->data);
}

void test_insertion_sort_rec(void){
  int t[5] = {0, 4, 2, 1, 3};
  int t_sorted[5] = {0, 1, 2, 3, 4};

  node *u = from_array(t, 5);
  node *v = insertion_sort_rec(u);
  node *w = from_array(t_sorted, 5);

  assert(is_equal(v, w));

  free_list(u);
  free_list(v);
  free_list(w);
}

// Exercice 4

node* reverse_copy(node* u){
    node* temp = NULL;
    while (u != NULL){
        temp = cons(temp,u->data);
        u = u->next;
    }
    return temp;
}

void test_reverse_copy(void){
  int t[4] = {1, 2, 3, 4};
  int t_rev[4] = {4, 3, 2, 1};
  node *u = from_array(t, 4);
  node *v = from_array(t_rev, 4);

  node *u_rev = reverse_copy(u);
  assert(is_equal(v, u_rev));

  free_list(u);
  free_list(v);
  free_list(u_rev);
}

node *copy_rec(node *u){
    if (u == NULL) {return NULL;}
    return cons(copy_rec(u->next),u->data);
}

void test_copy_rec(void){
  int t1[4] = {1, 2, 3, 4};
  node *u = from_array(t1, 4);
  node *v = copy_rec(u);

  assert(is_equal(u, v));
  free_list(u);
  free_list(v);
}

node* copy(node* u){
    if (u == NULL) {return NULL;}
    node* current = new_node(u->data);
    node* start = current;
    while (u->next != NULL){
        u = u->next;
        node* new = new_node(u->data);
        current->next = new;
        current = current->next;
    };
    return start;
}

void test_copy(void){
  int t1[4] = {1, 2, 3, 4};
  node *u = from_array(t1, 4);
  node *v = copy(u);

  assert(is_equal(u, v));
  free_list(u);
  free_list(v);

  u = NULL;
  v = copy(u);
  assert(is_equal(u, v));
}

node* reverse(node* u){
    if (u == NULL) {return NULL;}
    node* current = u;
    node* next = u->next;
    current->next = NULL;
    while (next != NULL) {
        node* temp = current;
        current = next;
        next = next->next;
        current->next = temp;
    }
    return current;
}

void test_reverse(void){
  int t[4] = {1, 2, 3, 4};
  node *u = from_array(t, 4);
  node *u_rev = reverse_copy(u);


  u = reverse(u);
  assert(is_equal(u, u_rev));

  free_list(u);
  free_list(u_rev);

  u = NULL;
  u = reverse(u);
  assert(is_equal(u, NULL));
}


// Exercice 5

node* read_list(void){
  node* lst = NULL;
  while (true){
    int n;
    int nb = scanf("%d",&n);
    if (nb == 1){
      lst = cons(lst,n);
    } else {
      return lst;
    }
  }
}

void print_list_2(node *u){
  while (u != NULL) {
    printf("%d\n",u->data);
    u = u->next;
  }
}

/*************/
/*  Partie 2 */
/*************/

struct Stack {
  int len;
  node* top;
};

typedef struct Stack stack;

// Exercice 6

stack *empty_stack(void){
  stack* s = malloc(sizeof(stack));
  s->len = 0;
  s->top = NULL;
  return s;
}

datatype peek(stack *s){
  assert(s->len != 0);
  return s->top->data;
}

void push(stack *s, datatype x){
  s->top = cons(s->top,x);
  s->len = s->len + 1;
}

datatype pop(stack *s){
  assert(s->len > 0);
  node* top = s->top;
  datatype result = top->data;
  s->top = s->top->next;
  s->len = s->len - 1;
  free(top);
  return result;
}

void free_stack(stack *s){
  free_list(s->top);
  free(s);
}

void test_stack(void){
  stack *s = empty_stack();
  push(s, 1);
  push(s, 2);
  assert(pop(s) == 2);
  push(s, 3);
  assert(pop(s) == 3);
  assert(pop(s) == 1);
  assert(s->len == 0);
  push(s, 10);
  assert(pop(s) == 10);
  free_stack(s);
}


// Files

struct Queue {
  int len;
  node *left;
  node *right;
};

typedef struct Queue queue;


// Exercice 7

queue *empty_queue(void){
  queue* q = malloc(sizeof(q));
  q->len = 0;
  q->left = NULL;
  q->right = NULL;
  return q;
}

void free_queue(queue *q){
  free_list(q->left);
  free(q);
}

datatype peek_left(queue *q){
  assert(q->len != 0);
  return q->left->data;
}

void push_right(queue *q, datatype data){
   node* new = new_node(data);
   if (q->right == NULL){
     q->right = new;
     q->left = new;
   } else {
     q->right->next = new;
     q->right = new;
   }
   q->len++;
}

datatype pop_left(queue *q){
  assert(q->len > 0);
  datatype result = q->left->data;
  if (q->len == 1){
    free(q->left);
    q->left = NULL;
    q->right = NULL;
  } else {
    node* temp = q->left->next;
    free(q->left);
    q->left = temp;
  }
  q->len--;
  return result;
}

void test_queue(void){
  queue *q = empty_queue();
  push_right(q, 1);
  push_right(q, 2);
  assert(pop_left(q) == 1);
  push_right(q, 3);
  push_right(q, 4);
  assert(pop_left(q) == 2);
  assert(pop_left(q) == 3);
  assert(pop_left(q) == 4);
  push_right(q, 5);
  assert(pop_left(q) == 5);
  assert(q->len == 0);
  free_queue(q);
}


// Exercice 8

int minimum(int a, int b){
  if (a <= b) return a;
  return b;
}

int* hamming(int n){
  int* t = malloc(sizeof(int) * n);
  queue* f2 = empty_queue();
  queue* f3 = empty_queue();
  queue* f5 = empty_queue();
  push_right(f2,1);
  push_right(f3,1);
  push_right(f5,1);
  for (int i = 0; i < n; i++){
    int elt2 = peek_left(f2);
    int elt3 = peek_left(f3);
    int elt5 = peek_left(f5);
    int mini = minimum(minimum(elt2,elt3),elt5);
    t[i] = mini;
    if (elt2 == mini){
      pop_left(f2);
    }
    if (elt3 == mini){
      pop_left(f3);
    }
    if (elt5 == mini){
      pop_left(f5);
    }
    push_right(f5,5 * mini);
    if (mini % 5 != 0){
      push_right(f3,3 * mini);
      if (mini % 3 != 0){
        push_right(f2,2 * mini);
      }
    }
  }
  free(f2);
  free(f3);
  free(f5);
  return t;
}

void test_hamming(void){
  int n = 14;
  int h_ref[14] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20};
  int *h = hamming(14);
  for (int i = 0; i < n; i++){
    assert(h[i] == h_ref[i]);
  }

  free(h);
}

// Exercice 9

node* insert_it(node *u, datatype x){
  if (u == NULL || u->data == x) {return cons(u,x);}
  node* current = u;
  while (current != NULL) {
    if (current->next != NULL && current->next->data < x){
      current = current->next;
    } else {
      current->next = cons(current->next,x);
      break;
    }
  }
  return u;
}

node* insertion_sort_it(node *u){
  node* temp = NULL;
  while (u != NULL){
    temp = insert_it(temp,u->data);
    u = u->next;
  }
  return temp;
}

void test_insertion_sort_it(void){
  int t[5] = {0, 4, 2, 1, 3};
  int t_sorted[5] = {0, 1, 2, 3, 4};

  node *u = from_array(t, 5);
  node *v = insertion_sort_it(u);
  node *w = from_array(t_sorted, 5);
  assert(is_equal(v, w));

  free_list(u);
  free_list(v);
  free_list(w);
}


// Exercice 10

node *split(node *u, int n){
  node* v = u;
  while (n > 1) {
    assert(v != NULL);
    n--;
    v = v->next;
  }
  assert(v != NULL);
  node* temp = v->next;
  v->next = NULL;
  return temp;
}

node *merge(node *u, node *v){
  if (u == NULL) { return v; }
  if (v == NULL) { return u; }
  node *res = NULL;
  if (u->data <= v->data){
  res = u;
  u = u->next;
  } else {
  res = v;
  v = v->next;
  }
  node *current = res;
  while (u != NULL || v != NULL){
    if (v == NULL || (u != NULL && u->data <= v->data)){
    current->next = u;
    u = u->next;
      current = current->next;
    } else {
      current->next = v;
      v = v->next;
      current = current->next;
    }
  }
  return res;
}

node *merge_sort(node *u){
  int n = length(u);
  if (n < 2) {return u;}
  node* snd = merge_sort(split(u,n/2));
  node* fst = merge_sort(u);
  return merge(fst,snd);
}

void test_merge_sort(void){
  int t[10] = {1, 4, 0, 2, 3, 8, 5, 6, 6, 7};
  node *u = from_array(t, 10);

  u = merge_sort(u);
  node *u_sorted = insertion_sort_it(u);
  assert(is_equal(u, u_sorted));

  free_list(u);
  free_list(u_sorted);
}



int main(void) {
  test_length();
  test_to_array();
  test_is_equal();
  test_is_sorted();
  test_insert_rec();
  test_insertion_sort_rec();
  test_reverse_copy();
  test_copy_rec();
  test_copy();
  test_reverse();
  test_stack();
  test_queue();
  test_hamming();
  test_insertion_sort_it();
  test_merge_sort();
  return 0;
}
