#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define HEIGHT 400
#define WIDTH 600

typedef int rgb[3];

rgb red = {255, 0, 0};
rgb green = {0, 255, 0};
rgb blue = {0, 0, 255};
rgb black = {0, 0, 0};
rgb white = {255, 255, 255};
rgb lightgray = {150, 150, 150};

rgb canvas[HEIGHT][WIDTH];

void put_pixel(int x, int y, rgb c){
    int i = HEIGHT - 1 - y;
    int j = x;
    if (i < 0 || i >= HEIGHT || j < 0 || j > WIDTH) return;
    for (int k = 0; k < 3; k++){
        canvas[i][j][k] = c[k];
    }
}

/***********************/
/* Création du fichier */
/***********************/

/* Exercice 1 */

void print_canvas(void){
    printf("P3\n");
    printf("%d %d\n", WIDTH, HEIGHT);
    printf("255\n");
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            for (int k = 0; k < 3; k++){
                printf("%d ", canvas[i][j][k]);
            }
            printf("\n");
        }
    }
}



/***********************/
/*  Primitives simples */
/***********************/

/* Exercice 3 */

void draw_h_line(int y, int x0, int x1, rgb c) {
    for (int i = x0; i < x1; i++) {
        put_pixel(i,y,c);
    }
}

void draw_v_line(int x, int y0, int y1, rgb c){
    for (int i = y0; i < y1; i++) {
        put_pixel(x,i,c);
    }
}


/* Exercice 4 */

void draw_rectangle(int xmin, int ymin, int xmax, int ymax, rgb c) {
    draw_h_line(ymin,xmin,xmax,c);
    draw_h_line(ymax,xmin,xmax,c);
    draw_v_line(xmin,ymin,ymax,c);
    draw_v_line(xmax,ymin,ymax,c);
}

void fill_rectangle(int xmin, int ymin, int xmax, int ymax, rgb c) {
    for (int i = ymin; i < ymax;i++) {
        draw_h_line(i,xmin,xmax,c);
    }
}

/* Exercice 5 */

void fill_disk(int xc, int yc, int radius, rgb c){
    int xmin = xc - radius;
    int xmax = xc + radius;
    int ymin = yc - radius;
    int ymax = yc + radius;
    int temp = radius * radius;
    for (int i = xmin; i < xmax; i++) {
        for (int j = ymin; j < ymax; j++) {
            if (distance(xc,i,yc,j) <= temp) {
                put_pixel(i,j,c);
            }
        }
    }
}

/***********************/
/* Mélange de couleurs */
/***********************/

/* Exercice 6 */

int clamp(double x) {
    if (x >= 255) {
        return 255;
    } else if (x <= 0) {
        return 0;
    } else {
        return x/1;
    }
}

void mix(rgb c0, rgb c1, double alpha, double beta, rgb result) {
    for (int i = 0; i < 3; i++) {
        result[i] = clamp(alpha * c0[i] + beta * c1[i]);
    }
}


/* Exercice 7 */

void draw_h_gradient(int y, int x0, int x1, rgb c0, rgb c1) {
    if (x1 < x0) {
        return draw_h_gradient(y,x1,x0,c0,c1);
    }
    int temp[3] = {0,0,0};
    put_pixel(x0,y,c0);
    put_pixel(x1,y,c1);
    for (int i = x0 + 1; i < x1; i++) {
        double r = (double)(i - x0)/ (double)(x1 - x0);
        mix(c0,c1,1 - r,r,temp);
        put_pixel(i,y,temp);
    }
}

int distance(int x0, int x1,int y0, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    return (dx * dx) + (dy * dy);
}

void fill_disk_gradient(int xc, int yc, int radius, rgb c_center, rgb c_edge) {    
    int xmin = xc - radius;
    int xmax = xc + radius;
    int ymin = yc - radius;
    int ymax = yc + radius;
    int temp2[3] = {0,0,0};
    for (int i = xmin; i < xmax; i++) {
        for (int j = ymin; j < ymax; j++) {
            double ratio = 1.0 * distance(xc,i,yc,j)/(radius * radius);
            if (ratio <= 1.) {
                mix(c_center,c_edge,1 - ratio,ratio,temp2);
                put_pixel(i,j,temp2);
            }
        }
    }
}


/* Exercice 8 */

void get_pixel(int x, int y, rgb result);

void mix_pixel(int x, int y, double alpha, double beta, rgb c);

void add_disk(int xc, int yc, int radius, rgb c);



/*******************/
/* Tracé de lignes */
/*******************/

/* Exercice 9 */

void draw_line(int x0, int y0, int x1, int y1, rgb c);

/* Exercice 10 */

void draw_spokes(int xc, int yc, int radius, int nb_spokes, rgb c);


/* Exercice 11 */

void bresenham_low(int x0, int y0, int x1, int y1, rgb c);

void bresenham_high(int x0, int y0, int x1, int y1, rgb c);

void bresenham(int x0, int y0, int x1, int y1, rgb c);


/* Exercice 12 */

void draw_circle(int xc, int yc, int radius, rgb c);


/*****************/
/* Fonction main */
/*****************/

int main(void){
    fill_disk_gradient(300,200,100,red,blue);
    print_canvas();
}
