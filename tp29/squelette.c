#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STBI_NO_FAILURE_STRINGS
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb_image.h"
#include "stb_image_write.h"

#include "seam_carving.h"


image *image_load(char *filename){
    int w, h, channels;
    uint8_t *data = stbi_load(filename, &w, &h, &channels, 0);
    if (!data) {
        fprintf(stderr, "Erreur de lecture.\n");
        stbi_failure_reason();
        exit(EXIT_FAILURE);
    }
    if (channels != 1){
        fprintf(stdout, "Pas une image en niveaux de gris.\n");
        exit(EXIT_FAILURE);
    }
    image *im = image_new(h, w);
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            im->at[i][j] = data[j + i * w];
        }
    }
    free(data);
    return im;
}

void image_save(image *im, char *filename){
    int h = im->h;
    int w = im->w;
    int stride_length = w;
    uint8_t *data = malloc(w * h * sizeof(uint8_t));
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            data[j + i * w] = im->at[i][j];
        }
    }
    if (!stbi_write_png(filename, w, h, 1, data, stride_length)){
        fprintf(stderr, "Erreur d'écriture.\n");
        image_delete(im);
        free(data);
        exit(EXIT_FAILURE);
    }
    free(data);
}


image *image_new(int h, int w){
    image* img = malloc(sizeof(image));
    img->at = malloc(h * sizeof(uint8_t*));
    for (int i = 0; i < h;i++) {
        img->at[i] = malloc(w * sizeof(int8_t));
    }
    img->h = h;
    img->w = w;
    return img;
}

void image_delete(image *im){
    for (int i = 0;i < im->h;i++) {
        free(im->at[i]);
    }
    free(im->at);
    free(im);
}

void invert(image *im) {
    for(int i = 0; i < im->h; i++) {
        for(int j = 0; j < im->w; j++) {
            im->at[i][j] = 255 - im->at[i][j];
        }
    }
}

void binarize(image *im){
    for (int i = 0; i < im->h;i++) {
        for (int j = 0; j < im->w;j++) {
            im->at[i][j] = (im->at[i][j] >= 128) ? 255 : 0; 
        }
    }
}

void flip_horizontal(image *im){
    for (int i = 0; i < im->h;i++){
        int n1 = 0;
        int n2 = im->w - 1;
        while (n1 < n2){
            int8_t v = im->at[i][n1];
            im->at[i][n1] = im->at[i][n2];
            im->at[i][n2] = v;
            n1++;
            n2--;
        }
    }
}


int min(int x,int y){
    return (x <= y) ? x:y;
}

energy *energy_new(int h, int w){
    energy* e = malloc(sizeof(energy));
    e->at = malloc(h * sizeof(double *));
    for (int i = 0; i < h;i++){
        e->at[i] = malloc(w * sizeof(double));
    }
    e->h = h;
    e->w = w;
    return e;
}

void energy_delete(energy *e){
    for (int i = 0;i < e->h;i++) {
        free(e->at[i]);
    }
    free(e->at);
    free(e);
}

void compute_energy(image *im, energy *e){
    for (int i = 0; i < im->h;i++){
        int it = (i > 0) ? i - 1 : i;
        int ib = (i < im->h - 1) ? i + 1 : i;
        for (int j = 0; j < im->w;j++){
            int jr = (j < im->w - 1) ? j + 1 : j;
            int jl = (j > 0) ? j - 1 : j;
            double delta_i = (double)(im->at[it][j]) - (double)(im->at[ib][j]);
            double delta_j = (double)(im->at[i][jr]) - (double)(im->at[i][jl]);
            e->at[i][j] = fabs(delta_i)/(ib - it) + fabs(delta_j)/(jr - jl);
        }
    }
}

image *energy_to_image(energy *e){
    image* new_img = image_new(e->h,e->w);
    int h = e->h;
    int w = e->w;
    double mini = e->at[0][0];
    double maxi = e->at[0][0];
    for (int i = 0; i < h;i++){
        for (int j = 0;j < w;j++){
            if (e->at[i][j] < mini) {
                mini = e->at[i][j];
            }
            if (e->at[i][j] > maxi) {
                maxi = e->at[i][j];
            }
        }
    }
    for (int i = 0; i < h;i++){
        for (int j = 0;j < w;j++){
            double value = e->at[i][j];
            new_img->at[i][j] = (uint8_t) (255 * (value - mini)/ (maxi - mini));
        }
    }
    return new_img;
}

void remove_pixel(uint8_t *line, double *e, int w){
    double e_min = e[0];
    double j_min = 0;
    for (int j = 1; j < w; j++){
        if (e[j] < e_min){
            e_min = e[j];
            j_min = j;
        }
    }
    for (int j = j_min; j < w - 1;j++){
        line[j] = line[j + 1];
    }
}

void reduce_one_pixel(image *im, energy *e){
    compute_energy(im,e);
    for (int i = 0; i < im->h;i++){
        remove_pixel(im->at[i],e->at[i],im->w);
    }
    im->w--;
    e->w--;
}

void reduce_pixels(image *im, int n){
    energy* e = energy_new(im->h,im->w);
    for (int i = 0; i < n;i++){
        reduce_one_pixel(im,e);
    }
    energy_delete(e);
}



int best_column(energy *e){
    int mini = 0;
    int j_min = -1;
    for (int j = 0; j < e->w; j++){
        double s = 0;
        for (int i = 0; i < e->h ;i++){
            s = s + e->at[i][j];  
        }
        if (j_min == -1 || s < mini){
            mini = s;
            j_min = j;
        }
    }
    return j_min;
}

void reduce_one_column(image *im, energy *e){
    compute_energy(im,e);
    int j_min = best_column(e);
    for (int i = 0; i < im->h; i++){
        for (int j = j_min; j < im->w - 1;++j){
            im->at[i][j] = im->at[i][j + 1];
        }
    }
    im->w--;
    e->w--;
}

void reduce_column(image *im, int n){
    energy* e = energy_new(im->h,im->w);
    for (int i = 0; i < n;i++){
        reduce_one_column(im,e);
    }
    energy_delete(e);
}



void energy_min_path(energy *e){
    for (int i = 1; i < e->h; i++) {
        for (int j = 0; j < e->w;j++){
            int jr = (j < e->w - 1) ? j + 1 : j;
            int jl = (j > 0) ? j - 1 : j;
            double energy_min = e->at[i - 1][jl];
            for (int k = jl + 1; k <= jr;k++) {
                if (e->at[i - 1][k] < energy_min) {
                    energy_min = e->at[i - 1][k];
                }
            }
            e->at[i][j] += energy_min;
        }
    }
}

path *path_new(int n){
    path* new_path = malloc(sizeof(path));
    new_path->at = malloc(sizeof(int) * n);
    new_path->size = n;
    return new_path;
}

void path_delete(path *p){
    free(p->at);
    free(p);
}


void compute_min_path(energy *e, path *p){
    int h = e->h;
    int j_min = 0;
    int e_min = e->at[h - 1][0];
    for (int j = 1; j < e->w;j++){
        if (e->at[h - 1][j] < e_min) {
            e_min = e->at[h - 1][j];
            j_min = j;
        }
    }
    p->at[h - 1] = j_min;
    int j = j_min;
    for (int i = h - 2; i >= 0;i--) {
        int jr = (j < e->w - 1) ? j + 1 : j;
        int jl = (j > 0) ? j - 1 : j;
        e_min = e->at[i][jl];
        j_min = jl;
        for (int k = jl + 1; k <= jr;k++){
            if (e->at[i][k] < e_min) {
                e_min = e->at[i][k];
                j_min = k;
            }
        }
        p->at[i] = j_min;
        j = j_min;
    }
}

void reduce_seam_carving(image *im, int n){
    energy* e = energy_new(im->h,im->w);
    path* p = path_new(im->h);
    for (int k = 0; k < n; k++){
        compute_energy(im,e);
        energy_min_path(e);
        compute_min_path(e,p);
        for (int i = 0; i < im->h;i++){
            for (int j = p->at[i]; j < im->w - 1;++j) {
                im->at[i][j] = im->at[i][j + 1];
            }
        }
        im->w--;
        e->w--;
    }
    energy_delete(e);
    path_delete(p);
}



int main(int argc, char *argv[]) {
    if (argc < 3){
        printf("Fournir le fichier d'entrée et de sortie.\n");
        exit(EXIT_FAILURE);
    }
    char *f_in = argv[1];
    char *f_out = argv[2];
    image *im = image_load(f_in);
    reduce_seam_carving(im,100);
    image_save(im, f_out);
    image_delete(im);
    return 0;
}
