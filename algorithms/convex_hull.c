#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../structs/stack.c"

// Graham Scan (2D plane)

struct point {
    int x;  
    int y;
};

typedef struct point point;
point pivot;


struct result {
    point* arr;
    int len;
};

typedef struct result result;

void swap(point* points,int i, int j){
    point temp = points[i];
    points[i] = points[j];
    points[j] = temp;
}

/* Find the orientation between 3 points.
# 0 --> A, B and C are collinear
# 1 --> Clockwise
# 2 --> Counterclockwise
*/
int orientation(point A,point B, point C){
    int res = ((B.y - A.y) * (C.x - B.x) - (B.x - A.x) * (C.y - B.y));
    if (res == 0) return 0;
    if (res > 0) return 2;
    if (res < 0) return 1;
}

int dist(point A,point B){
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y)* (A.y - B.y);
}

/*
Sorts an array of points with respect to the first point.
*/
int compare(const void* A,const void* B){ 
    point a = *(point*)A;
    point b = *(point*)B;
    int o = orientation(pivot,a,b);
    if (o == 0){ //colinear
        if (dist(pivot,b) >= dist(pivot,a)) return -1;
        return 1;
    }
    if (o == 2) return -1;
    if (o == 1) return 1;
}


bool comp(point A,point B){//lexical order with (y,x) coordinates
    if (A.y < B.y) {
        return false;
    } else if (A.y == B.y && B.x > A.x){
        return false;
    }
    return true;
}

/*
Searches for point with minimal value with respect to (y,x) lexical order.
Returns its index in 'points' array.
*/
int find_pivot(point* points,int n){ //lexical order with (y,x) ok
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

point* remove_same_angle_points(point* points,int n,int* new_len){
    int nb_same_angle = 0;
    for (int i = 1; i < n - 1;i++){ //first, check for the number of points that are colinear
        if (orientation(pivot,points[i],points[i+1]) == 0){
            nb_same_angle++;
        }
    }
    int m = 1;
    *new_len = n - nb_same_angle;
    point* new_points = malloc(sizeof(point) * (*new_len));
    for (int i = 1; i < n ;i++){
        while (i < n - 1 && orientation(pivot,points[i],points[i+ 1]) == 0){
            i++;
        }
        new_points[m] = points[i];
        m++;
    }
    free(points);
    return new_points;
}

result graham_scan(point* points,int n){
    int piv = find_pivot(points,n);
    pivot = points[piv];
    swap(points,0,piv);
    qsort(points,n,sizeof(point),compare);
    //We now need to remove points that have the same angle, and only keep the farthest from pivot ones.
    int new_len;
    point* unique_angle_points = remove_same_angle_points(points,n,&new_len);
    unique_angle_points[0] = pivot;
    stack* s = stack_new();
    stack_push(s,0);
    stack_push(s,1);
    stack_push(s,2);
    for (int i = 3; i < new_len;i++){
        while (s->len > 1 && orientation(unique_angle_points[stack_peek_second(s)],unique_angle_points[stack_peek(s)],unique_angle_points[i]) != 2  ) {
            stack_pop(s);
        }
        stack_push(s,i);
    }
    int* indexes = stack_to_arr(s);
    point* convex_hull = malloc(sizeof(point) * s->len);
    for (int i = 0; i < s->len;i++){
        convex_hull[i] = unique_angle_points[i];
    }
    result res = {.arr = convex_hull,.len=s->len};
    free(unique_angle_points);
    free(indexes);
    free_stack(s);
    return res;
}

int main(int argc,char* argv[]){
    if (argc != 2) return 1;
    int n = atoi(argv[1]);
    point* points = malloc(sizeof(point) * n);
    int temp_x,temp_y;
    for (int i = 0;i < n;i++){
        scanf("%d %d",&temp_x,&temp_y);
        point p = {.x = temp_x,.y = temp_y};
        points[i] = p;
    }
    result res = graham_scan(points,n);
    for (int i = 0; i < res.len;i++){
        printf("Point (%d,%d)\n",res.arr[i].x,res.arr[i].y);
    }
    free(res.arr);

    return 0;
}