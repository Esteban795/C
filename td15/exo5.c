#include <stdio.h>
#include <stdlib.h>

#define ROWS 4000
#define COLS 4000

int arr[ROWS][COLS];


int divergence(double xc,double yc,int itermax) {
    int n = 0;
    double re = 0.0;
    double im = 0.0;
    while (re*re + im*im <= 4 && n <= itermax) {
        double temp = re;
        re = re*re - im*im + xc;
        im = 2.* temp * im + yc;
        n++;
    }
    return n;
    
}

double re(int j,double xmin, double xmax) {
    double ratio = (double)j / (COLS- 1);
    return xmin + ratio  * (xmax - xmin);
}

double im(int i,double ymin, double ymax) {
    double ratio = (double)i / (ROWS- 1);
    return ymax + ratio  * (ymin - ymax);
}

void fill_tab(double xmin, double xmax, double ymin, double ymax,int itermax) {
    for (int i = 0; i < ROWS;i++) {
        for (int j = 0;j < COLS;j++) {
            double x = re(j,xmin,xmax);
            double y = im(i,ymin,ymax);
            arr[i][j] = divergence(x,y,itermax);
        }
    }
}

void print_pixel_bw(int i, int j,int itermax) {
    if (arr[i][j] >itermax) {
        printf("255 255 255\n");
    } else {
        printf("0 0 0\n");
    }
}

void print_tab(int itermax) {
    printf("P3\n");
    printf("%d %d\n",COLS,ROWS);
    printf("255\n");
    for (int i = 0;i < ROWS;i++) {
        for (int j = 0; j < COLS;j++) {
            print_pixel_bw(i,j,itermax);
        }
    }
}
int main(void) {
    double xmin = -2.;
    double xmax = 2.0;
    double ymin = -2.;
    double ymax = 2.0;
    fill_tab(xmin,xmax,ymin,ymax,20);
    print_tab(20);
    return 0;
}