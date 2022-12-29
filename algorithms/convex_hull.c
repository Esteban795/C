#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../structs/stack.c"

// Graham Scan (2D plane)

struct point {
    int x;  
    int y;
};

typedef struct point point;

struct result {
    point* arr;
    int len;
};

typedef struct result result;

int vect_product(point A,point B, point C){ //calculating whether left or right turn. >0 is left, < 0 is right
    return (B.x - A.x) * (C.y - A.y) - (C.x - A.x) * (B.y - A.y);
}

bool comp(point A,point B){ 
    if (A.y < B.y) {
        return false;
    } else if (A.y == B.y && B.x < A.x){
        return true;
    }
    return true;
}

int find_pivot(point* points,int n){ //lexical order with (y,x)
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

double find_angle_horizontal(point B){ //find angle between two points and horizontal line
    double oppose = B.y;
    double adjacent = B.x;
    return atan(oppose/adjacent);
}

int compare(const void* A,const void* B){
    point a = *(point*)A;
    point b = *(point*)B;
    double angle1 = find_angle_horizontal(a);
    double angle2 = find_angle_horizontal(b);
    if (angle1 - angle2 > 0) return 1;
    return 0;
}

result graham_scan(point* points,int n){
    int pivot = find_pivot(points,n);
    point temp = points[pivot]; //swap pivot with first element of array
    points[pivot] = points[0];
    points[0] = temp;
    for (int i = 0; i < n;i++){ //on fait comme si le pivot était en (0,0) car impossible de passer le pivot en arg
        points[i].x -= points[0].x;
        points[i].y -= points[0].y;
    }
    qsort(points + 1,n - 1,sizeof(point),compare);
    stack* s = new_stack();
    stack_push(s,1);
    stack_push(s,2);
    for (int i = 3; i < n;i++){
        int first = stack_pop(s);
        int next = stack_pop(s);
        point top = points[first];
        point second = points[next];
        while (s->len > 2 && vect_product(points[i],second,top) < 0) {
            stack_pop(s);
        }
        stack_push(s,i);
    }
    point* arr = malloc(sizeof(point) * s->len);
    for (int i = 0; i < s->len;i++){
        arr[i] = points[stack_pop(s)];
        arr[i].x += points[0].x;
        arr[i].y += points[0].y;
    }
    result res = {.arr = arr,.len = s->len};
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
    for (int i = 0; i < res.len; i++){
        printf("x : %d, y : %d\n",res.arr[i].x,res.arr[i].y);
    }
    free(points);
    return 0;
}