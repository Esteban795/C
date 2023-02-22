#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

struct image {
  int width;
  int height;
  int maxc;
  uint16_t **pixels;
};

typedef struct image image;

/** Fabrique une image à partir du contenu d'un fichier supposé au
    format PGM ASCII */
image *charger_image(char *nom_fichier) {
  image* img = malloc(sizeof(image));
  FILE* f = fopen(nom_fichier,"r");
  fscanf(f,"P2");
  int width,height;
  fscanf(f,"%d %d\n",&width,&height);
  uint16_t** pixels = malloc(sizeof(uint16_t*) * height);
  int temp;
  uint16_t temp16;
  for (int i = 0; i < height;i++){
    pixels[i] = malloc(sizeof(uint16_t) * width);
    for (int j = 0; j < width; j++){
      fscanf(f,"%d ",&temp);
      if (temp <= 65535 && temp > 0){
        temp16 = (uint16_t)temp;
        pixels[i][j] = temp;
      } else {
        return NULL;
      }
    }
    fscanf(f,"\n");
  }
  img->pixels = pixels;
  img->height = height;
  img->width = width;
  img->maxc = 65535;
  fclose(f);
  return img;
}

/** Libère une image */
void liberer_image(image *img) {
  for (int i = 0; i < img->height; i++) {
    free(img->pixels[i]);
  }
  free(img->pixels);
  free(img);
}

/** Renvoie le caractère caché dans les `8` premières cases d'un
    tableau supposé de longueur au moins `8` */
char caractere(uint16_t *tab) {
  int temp = 0;
  for (int i = 0; i < 8;i++){
    int bit = tab[i] & 1;
    temp += bit * (1 << (8 - i));
  }
  return (char)temp;
}

/** Écrit dans le flux le message caché dans l'image. On suppose que
    l'image, et le flux sont valides, et en particulier non `NULL`. On
    suppose également que le message est valide et donc, en
    particuler, que `img->larg >= 8`. */
int sauvegarder_message(image *img, char *nom_sortie){
  FILE* output = fopen(nom_sortie,"w");
  if (output == NULL) return 1;
  int start = 0;
  int nb_chars_per_line = img->width / 8;
  for (int i = 0; i < img->height; i++){
    for (int j = 0; j < nb_chars_per_line;j++){
      char c = caractere(img->pixels[i] + start);
      fprintf(output,"%c",c);
    }
  }
  fclose(output);
  return 0;
}

/** Insère le caractère dans les `8` premières cases du tableau. On
    suppose que le tableau est de taille suffisante. */
void inserer_caractere(char c, uint16_t *tab) {
}

/** Cache un message dans une image. On suppose que l'image est de
    hauteur et de largeur suffisante. */
int cacher(image *img, char *nom_entree) {
  return 1;
}

/** Sauvegarde une image dans un fichier au format PGM ASCII. On
    suppose que l'image est valide. */
int sauvegarder_image(image *img, char *nom_fichier) {
  return 1;
}

int main(int argc, char* argv[]) {
}
