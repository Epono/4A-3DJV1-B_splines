#pragma once

#include "../headers/utils.h"

#include<stdlib.h>
#include<stdio.h>

#include "../headers/utils.h"
#include "../headers/decoupage.h"
#include <math.h>

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
int isCoupe(Point P0, Point P1, Point P2, Point P3);

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
Point returnPoint(Point P0, Point P1, Point P2, Point P3);


int visible(Point S, Point Fi, Point Fii);

//PL contient en entrée le polygone a decouper et en sortie le polygone decoupé
void decoupageWiki(CustomPolygon* entree, CustomPolygon fenetre);













/*
// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
int both(Point P0, Point P1, Point P2, Point P3, Point* intersect, float *i_x, float *i_y) {
Point S1 = {P1.x - P0.x, P1.y - P0.y};
Point S2 = {P3.x - P2.x, P3.y - P2.y};

float s, t;
s = (-S1.y * (P0.x - P2.x) + S1.x * (P0.y - P2.y)) / (-S2.x * S1.y + S1.x * S2.y);
t = (S2.x * (P0.y - P2.y) - S2.y * (P0.x - P2.x)) / (-S2.x * S1.y + S1.x * S2.y);

if(s >= 0 && s <= 1 && t >= 0 && t <= 1) {
intersect->x = P0.x + (t * S1.x);
intersect->y = P0.y + (t * S1.y);
return 1;
}

return 0;
}
*/

/*
// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the floats i_x and i_y.
char get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y,
float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y) {
float s1_x, s1_y, s2_x, s2_y;
s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

float s, t;
s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

if(s >= 0 && s <= 1 && t >= 0 && t <= 1) {
// Collision detected
if(i_x != NULL)
*i_x = p0_x + (t * s1_x);
if(i_y != NULL)
*i_y = p0_y + (t * s1_y);
return 1;
}

return 0; // No collision
}
*/

/*
* Implementation of the Sutherland-Hodgman algorithm (cutting the window ?)
*
* PL : sommets du polygone a decouper
* N1 : nombre de sommets du polygone a decouper
* PW : sommets de la fenetre (4)
*/
/*
int pointsInPS = 0;

[]Point decoupageSutherlandHodgman(Point PL[], int N1, Point PW[]) {
Point S, F, I;
Point PS[256];		//liste des points du polygone de sortie
int N2;				//nombre de points de PS

//Pour chaue point de la window PW (fenetre de decoupage)
for(int i = 0; i < 4; i++) {
N2 = 0;
// PS <- vide

//Pour chaque point du polygone PL (polygone a decouper)
for(int j = 0; j <= N1; j++) {
if(j == 0) {
F = PL[j]; // Sauver le 1er sommet
}
else {
if(coupe(S, PL[j], F[i], F[i + 1])) {
I = intersection(S, PL[j], F[i], F[i + 1]);
Charger(I, &PS); //what ?
N2++;
}
}
S = PL[j];
if(visible(S, F[i], F[i + 1])) {
Charger(S, &PS);
}
N2++;
}

if(N2 > 0) {
//traitement du dernier cote de PL (polygone a decouper)
if(coupe(S, F, F[i], F[i + 1])) {
I = intersection(S, F, F[i], F[i + 1]);
Charger(I, &PS); // what ?
N2++;
}
//Decoupage pour chacun des polygones
PL = PS;
N1 = N2;
}
}
return PS;
}
*/