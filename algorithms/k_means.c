#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Point {
    float x; 
    float y;
} point;

float d2(point p, point q){
    return (p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y);
}


void swap(point arr[],int i,int j){
    point temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

point* random_choice(point data[],int len,int k){
    point* points = malloc(sizeof(point) * k);
    int* indices = malloc(sizeof(point) * k);
    for (int i = 0; i < k;i++){
        int j = i + random () % len;
        swap(data,i,j);
        indices[i] = j;
        points[i] = points[j];
    }

    //restorer le premier tableau
    for (int i = k - 1; i >= 0; i--){
        swap(data,i,indices[i]);
    }
    return points;
}

int nearest(point p,point centers[],int k){
    int min_dist = d2(centers[0],p);
    int i = 0;
    for (int j = 0; j < k;j++){
        float dist = d2(centers[j],p);
        if (min_dist > dist){
            min_dist = dist;
            i = j;
        }
    }
    return i;
}

bool update_assignment(point data[],int len,point centers[],int len_centers,int assignment[]){
    bool changed = false;
    for (int i = 0; i < len; i++){
        int near = nearest(data[i],centers,len_centers);
        if (near == assignment[i]) continue;
        assignment[i] = near;
        changed = true;
    }
    return changed;
}

void update_centers(point data[],int len, point centers[],int len_centers,int assignment[]){
    int* count = malloc(len_centers * sizeof(int));
    for (int i = 0; i < len_centers;i++){
        count[i] = 0;
        centers[i].x = 0.;
        centers[i].y = 0.;
    }
    for (int i = 0; i < len; i++){
        int c = assignment[i];
        centers[c].x += data[i].x;
        centers[c].y += data[i].y;
        count[c]++;
    }

    for (int c = 0; c < len_centers; c++){
        if (count[c] == 0) continue;
        centers[c].x /= count[c];
        centers[c].y /= count[c];
    }
}

int* kmeans(int data[], int n, int k,int itermax){
    point* centers = random_choice(data,n,k);
    int* assignment = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++){
        assignment[i] = -1;
    }

    int iter = 0;
    while (update_assignment(data,n,centers,k,assignment) && iter < itermax){
        update_centers(data,n,centers,k,assignment);
    }
    free(centers);
    return assignment;
}