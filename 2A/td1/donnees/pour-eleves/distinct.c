#include <stdio.h>
#include <stdlib.h>

#include "bst.h"


int main(void) {
    bst* tree = bst_make_empty();
    int x;
    while (true) {
        int found = scanf("%d",&x);
        if (found != 1) break;
        bst_insert(tree,x);
    }
    int size_tree = bst_size(tree);
    item* tab = bst_to_array(tree,size_tree);
    printf(" %d distincts elements !\n",size_tree);
    printf("Min : %d\n",tab[0]);
    printf("Max : %d\n",tab[size_tree - 1]);
    printf("Mediane : %d\n",tab[size_tree / 2]);
    bst_free(tree);
    free(tab);
    return 0;
}