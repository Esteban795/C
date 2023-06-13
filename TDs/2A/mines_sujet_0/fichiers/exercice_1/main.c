#include "lecture.h"
//#include "liste.h"
#include <math.h>


const double DEGREE_VERS_RADIAN =  M_PI / 180 ;
const double DIAMETRE_TERRE_EN_KM = 12742 ;


double ll_distance(struct geopoint p1, struct geopoint p2) {
  double y1 = DEGREE_VERS_RADIAN * p1.latitude;
  double y2 = DEGREE_VERS_RADIAN * p2.latitude;
  double x1 = DEGREE_VERS_RADIAN * p1.longitude;
  double x2 = DEGREE_VERS_RADIAN * p2.longitude;
  double first_square = sin((y1 - y2) / 2) * sin((y1 - y2) / 2);
  double second_square = sin((x1 - x2) / 2) * sin((x1 - x2) / 2);
  double under_sqrt =  first_square + cos(y1) * cos(y2) * second_square;
  return DIAMETRE_TERRE_EN_KM* 10e-3* asin(sqrt(under_sqrt));
}

void q3(void){
  FILE * fp = fopen("routes.txt", "r");
  double tot_h = 0;
  double max_h = 0;
  double tot_r = 0;
  double max_r = 0;
  fscanf(fp,"%d\n",&nb_routes);
  for(int id_route = 0 ; id_route < nb_routes ; id_route++ ) { 
    int depart, arrivee;
    double distance ;
    char nomRoute[270];
    fscanf(fp,"%d %d %lf  %[^\n]\n",&depart,&arrivee,&distance,nomRoute);
    double h_dist = ll_distance(position[depart],position[arrivee]);
    tot_r += distance;
    max_r = distance > max_r ? distance : max_r;
    tot_h += h_dist;
    max_h = h_dist > max_h ? h_dist : max_h;
  }
  fclose(fp);
  printf("Mean read : %lf, Max read : %lf\n",tot_r / nb_routes, max_r);
  printf("Mean dist : %lf, Max dist : %lf\n",tot_h / nb_routes, max_h);
}

void sauvegarde_graphe(void){
  FILE* f_out = fopen("graphe.txt","w");
  fprintf(f_out,"%d\n",nb_points);
  int index= 0;
  for (int i = 0; i < nb_points;i++){
    int i_dep = i;
    int dep = routes[index].p1;
    int nb_voisins = 0;
    while (dep == i){
      nb_voisins++;
      if (index == nb_routes) break;
      index++;
      dep = routes[index].p1;
    }
    fprintf(f_out,"%d",nb_voisins);
    for (int j = i_dep; j < index;j++){
      fprintf(f_out,"%s %lf",routes[j].nom,routes[j].dist);
    }
    fprintf(f_out,"\n");
  }
  fclose(f_out);
}
int main() {
  lit_positions();
  lit_routes();
  //nettoie();
  q3();
  return 0;
}
