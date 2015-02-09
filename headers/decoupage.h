#ifndef DECOUPAGE_H
#define DECOUPAGE_H

#include "utils.h"

void normale(Point, Point, Point*);

int coupe(Point, Point, Point, Point);

Point intersection(Point, Point, Point, Point);

int visible(Point, Point, Point);

void SutherlandHodgman(CustomPolygon, CustomPolygon, CustomPolygon *);

#endif //DECOUPAGE_H