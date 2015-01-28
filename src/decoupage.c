#include<stdlib.h>
#include<stdio.h>

#include "../headers/utils.h"
#include "../headers/decoupage.h"
#include <math.h>

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
int isCoupe(Point P0, Point P1, Point P2, Point P3) {
	Point S1 = {P1.x - P0.x, P1.y - P0.y};
	Point S2 = {P3.x - P2.x, P3.y - P2.y};

	int s = (-S1.y * (P0.x - P2.x) + S1.x * (P0.y - P2.y)) / (-S2.x * S1.y + S1.x * S2.y);
	int t = (S2.x * (P0.y - P2.y) - S2.y * (P0.x - P2.x)) / (-S2.x * S1.y + S1.x * S2.y);

	if(s >= 0 && s <= 1 && t >= 0 && t <= 1) {
		return 1;
	}

	return 0;
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
Point returnPoint(Point P0, Point P1, Point P2, Point P3) {
	Point S1 = {P1.x - P0.x, P1.y - P0.y};
	Point S2 = {P3.x - P2.x, P3.y - P2.y};

	int s = (-S1.y * (P0.x - P2.x) + S1.x * (P0.y - P2.y)) / (-S2.x * S1.y + S1.x * S2.y);
	int t = (S2.x * (P0.y - P2.y) - S2.y * (P0.x - P2.x)) / (-S2.x * S1.y + S1.x * S2.y);

	Point intersectPoint = {P0.x + (t * S1.x), P0.y + (t * S1.y)};
	printf("Le point d'intersection est (%d, %d)", intersectPoint.x, intersectPoint.y);
	return intersectPoint;
}


int visible(Point S, Point Fi, Point Fii) {
	//retournant un booléen si S est visible par rapport à(Fi Fi+1)
	// Si le point S est du côté intérieur du polygone par rapport aux côtés Fi Fi+1
	//Vector A = {Fi.x - S.x, Fi.y - S.y};
	//Vector B = {Fii.x - S.x, Fii.y - S.y};

	float longueurX_FiS = abs(Fi.x - S.x);
	//printf("Longueur(x) Fi -> S : %f\n", Fi.x, S.x, longueurX_FiS);
	float longueurY_FiS = abs(Fi.y - S.y);
	//printf("Longueur(y) Fi -> S : %f\n", longueurY_FiS);
	float longueurX_FiFii = abs(Fi.x - Fii.x);
	//printf("Longueur(x) S -> Fii : %f\n", longueurX_FiFii);
	float longueurY_FiFii = abs(Fi.y - Fii.y);
	//printf("Longueur(y) S -> Fii : %f\n", longueurY_FiFii);


	float longueurFiS = sqrt((longueurX_FiS * longueurX_FiS) + (longueurY_FiS * longueurY_FiS));
	float longueurSFii = sqrt((longueurX_FiFii * longueurX_FiFii) + (longueurY_FiFii * longueurY_FiFii));

	float longueurX_FiS_N = longueurX_FiS / longueurFiS;
	float longueurY_FiS_N = longueurY_FiS / longueurFiS;
	float longueurX_FiFii_N = longueurX_FiFii / longueurSFii;
	float longueurY_FiFii_N = longueurY_FiFii / longueurSFii;

	//Produit scalaire = (longueur * longueur) + (hauteur * hauteur) (normalisé)
	float produitScalaire = (longueurX_FiS_N * longueurX_FiFii_N) + (longueurY_FiS_N * longueurY_FiFii_N);


	printf("Le point (%d, %d) est il visible par rapport au segment (%d, %d)(%d, %d) (produit scalaire : %f)?\n", S.x, S.y, Fi.x, Fi.y, Fii.x, Fii.y, produitScalaire);
	if(produitScalaire > 0) {
		//a droite
		printf("Oui\n");
		return 1;
	}
	else if(produitScalaire < 0) {
		//a gauche
		printf("Non\n");
		return 0;
	}
	else {
		//sur le segment
		printf("Oui\n");
		return 1;
	}
}

//PL contient en entrée le polygone a decouper et en sortie le polygone decoupé
void decoupageWiki(CustomPolygon* entree, CustomPolygon fenetre) {
	CustomPolygon* outputList = (CustomPolygon*) malloc(sizeof(CustomPolygon));
	copyVertices(outputList->vertices, entree->vertices, entree->nbVertices);//outputList->vertices = entree->vertices;
	outputList->nbVertices = entree->nbVertices;

	CustomPolygon* inputList = (CustomPolygon*) malloc(sizeof(CustomPolygon));

	for(int i = 0; i < fenetre.nbVertices; i++) {
		Line clipEdge = {fenetre.vertices[i], fenetre.vertices[(i + 1) % fenetre.nbVertices]};

		copyVertices(inputList->vertices, outputList->vertices, outputList->nbVertices);//inputList->vertices = outputList->vertices;
		inputList->nbVertices = outputList->nbVertices;

		outputList->nbVertices = 0;

		Point S = inputList->vertices[inputList->nbVertices - 1];
		//printf("Point S : (%d, %d)\n", S.x, S.y);

		for(int j = 0; j < inputList->nbVertices; j++) {
			Point E = inputList->vertices[j];
			//printf("Point E : (%d, %d)\n", E.x, E.y);

			if(visible(E, clipEdge.startPoint, clipEdge.endPoint)) {
				if(!visible(S, clipEdge.startPoint, clipEdge.endPoint)) {
					outputList->vertices[outputList->nbVertices] = returnPoint(S, E, clipEdge.startPoint, clipEdge.endPoint);
					outputList->nbVertices++;
				}
				outputList->vertices[outputList->nbVertices] = E;
				outputList->nbVertices++;
			}
			else if(visible(S, clipEdge.startPoint, clipEdge.endPoint)) {
				if(isCoupe(S, E, clipEdge.startPoint, clipEdge.endPoint)) {
					outputList->vertices[outputList->nbVertices] = returnPoint(S, E, clipEdge.startPoint, clipEdge.endPoint);
					outputList->nbVertices++;
				}
			}
			S = E;
		}
	}

	entree->nbVertices = outputList->nbVertices;
	copyVertices(entree->vertices, outputList->vertices, entree->nbVertices);//entree->vertices = outputList->vertices;

	printf("Poly decoupe : \n");
	for(int i = 0; i < entree->nbVertices; i++) {
		printf("Point %d : (%d, %d)\n", i, entree->vertices[i].x, entree->vertices[i].y);
	}
	printf("Fin poly decoupe\n");
}

void copyVertices(Point dest[], Point src[], int size) {
	for(int i = 0; i < size; i++) {
		Point temp = {src[i].x, src[i].y};
		dest[i] = temp;
	}
	return src;
}













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
