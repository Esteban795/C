#include "lecture.h"


struct geopoint* position ;
struct Route* routes;
int nb_routes ;
int nb_points;


void lit_routes() {
  FILE * fp = fopen ("routes.txt", "r"); // ouvre le fichier routes.txt en lecture

  
  fscanf(fp,"%d",&nb_routes); // lit la première ligne
  // qui donne le nombre de lignes dans le fichier
  routes = malloc(sizeof(route) * nb_routes);
  for(int id_route = 0 ; id_route < nb_routes ; id_route++) {  // itère pour chaque ligne
    route r;
    fscanf(fp,"%d %d %lf  %[^\n]\n",&r.p1,&r.p2,&r.dist,r.nom);
    routes[id_route] = r;
    // lit une ligne et stocke le résultat dans les variables passées
    // en paramètre. On sait donc qu'il y a un segment de route de
    // depart à fin de longueur distance et la route s'appelle nomRoute
  }
  fclose(fp); // ferme le fichier routes.txt
}


void lit_positions(void) {
  // on rappelle que fscanf(fp,"%lf %lf\n",longitude,latitude);
  // lit une paire de coordonnées sur une ligne 
  FILE* f_in = fopen("positions.txt","r");
  fscanf(f_in,"%d\n",&nb_points);
  position = malloc(sizeof(geopoint) * nb_points);
  double lat,lon;
  for (int i = 0; i < nb_points;i++){
    fscanf(f_in,"%lf %lf\n",&lat,&lon);
    geopoint p = {.latitude = lat,.longitude = lon};
    position[i] = p;
  }
  fclose(f_in);
}

void nettoie(void) {
  free(position);
  free(routes);
}
