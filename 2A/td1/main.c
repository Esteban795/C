#include <stdio.h>
#include <stdlib.h>

//1.1

struct v2D {
    double x;
    double y;
};

typedef struct v2D v2D;


double calc_norm(int x,int y){
    return x * x + y * y;
}

int comp_norm(v2D* v1, v2D* v2){
    int norm_1 =  calc_norm(v1->x,v1->y);
    int norm_2 = calc_norm(v2->x,v2->y);
    return norm_1 - norm_2;
}

void sort_by_norms(v2D points[], int len){
    qsort(&points,len,sizeof(v2D),comp_norm);
}