#pragma once

#include "utils.h"

void decoupageSutherlandHodgman(Point PL[], int N1, Point PW[]);

int coupe(Point S, Point Pj, Point Fi, Point Fii);

Point intersection(Point S, Point Pj, Point Fi, Point Fii);

int visible(Point S, Point Fi, Point Fii);