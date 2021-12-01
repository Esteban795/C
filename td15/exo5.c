#include <stdio.h>
#include <stdlib.h>

#define ROWS 900
#define COLS 900

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
void print_pixel_gs(int i, int j, int itermax) {
    int c  = 255 * arr[i][j] / itermax;
    for (int i = 0; i < 3; i++) {
        printf("%d ",c);
    }
    printf("\n");
}

void print_tab(int itermax) {
    printf("P3\n");
    printf("%d %d\n",COLS,ROWS);
    printf("255\n");
    for (int i = 0;i < ROWS;i++) {
        for (int j = 0; j < COLS;j++) {
            print_pixel_gs(i,j,itermax);
        }
    }
}
int main(int argc, char* argv[]) {
    int itermax;
    double xmin;
    double xmax;
    double ymin;
    double ymax;
    if (argc == 1) {
        itermax = 20;
        xmin = -2.;
        xmax = 2.0;
        ymin = -2.;
        ymax = 2.0;
    } else if (argc == 2) {
        itermax = atoi(argv[1]);
    } else if (argc == 6) {
        itermax = atoi(argv[1]);
        xmin = atof(argv[2]);
        xmax = atof(argv[3]);
        ymin = atof(argv[4]);
        ymax = atof(argv[5]);
    }
    fill_tab(xmin,xmax,ymin,ymax,itermax);
    print_tab(itermax);
    return 0;
}