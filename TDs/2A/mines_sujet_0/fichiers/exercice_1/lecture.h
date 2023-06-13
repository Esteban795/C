#ifndef LECTURE_H
#define LECTURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct geopoint {
  double latitude, longitude ;
};

struct Route {
  int p1;
  int p2;
  double dist;
  char nom[270];
};

typedef struct Route route;
typedef struct geopoint geopoint;

extern struct Route* routes;
extern struct geopoint* position ;
extern int nb_routes ;
extern int nb_points;

void lit_routes(void) ;
void lit_positions(void);
void nettoie(void);

#endif
