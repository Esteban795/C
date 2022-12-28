#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Graham Scan (2D plane)

struct point {
    int x;
    int y;
};

typedef struct point point;

int vect_product(point* A,point* B, point* C){ //calculating angle of point A and segment BC
    return (B->x - A->x) * (C->y - A->y) - (C->x - A->x) * (B->y - A->y);
}

bool comp(point A,point B){
    if (A.y < B.y) {
        return false;
    } else if (A.y == B.y && B.x < A.x){
        return true;
    }
    return true;
}

int find_pivot(point* points,int n){
    int index = 0;
    point minimum = points[0];
    for (int i = 0; i < n;i++){
        if (comp(minimum,points[i])) {
            minimum = points[i];
            index = i;
        }
    }
    return index;
}

double find_angle_horizontal(point A,point B){
    double oppose = B.y - A.y;
    double adjacent = B.x - A.x;
    return atan(oppose/adjacent);
}

double comp(point pivot,point A, point B){
    double angle1 = find_angle_horizontal(pivot,A);
    double angle2 = find_angle_horizontal(pivot,B);
    return angle1 - angle2;
}

point* graham_scan(point* points,int n){
    point pivot = points[find_pivot(points,n)];
    qsort(points + 1,n,sizeof(point),comp);
    return NULL;
}


int main(void){
    point A = {.x=0,.y=0};
    point B = {.x= 20,.y=20};
    double angle = find_angle_horizontal(A,B);
    printf("Angle is : %f",angle);
    return 0;
}