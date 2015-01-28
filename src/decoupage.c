#include<stdlib.h>
#include<stdio.h>

#include "utils.h"
#include "decoupage.h"

/*
 * Implementation of the Sutherland-Hodgman algorithm (cutting the window ?)
 *
 * PL : sommets du polygone a decouper
 * N1 : nombre de sommets du polygone a decouper
 * PW : sommets de la fenetre (4)
 */
/*
void decoupageSutherlandHodgman(Point PL[], int N1, Point PW[]) {
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
Charger(I, PS); //what ?
N2++;
}
}
S = PL[j];
if(visible(S, F[i], F[i + 1])) {
Charger(S, PS);
}
N2++;
}

if(N2 > 0) {
//traitement du dernier cote de PL (polygone a decouper)
if(coupe(S, F, F[i], F[i + 1])) {
I = intersection(S, F, F[i], F[i + 1]);
Charger(I, PS); // what ?
N2++;
}
//Decoupage pour chacun des polygones
PL = PS;
N1 = N2;
}
}
}
*/

// S : point courant du polygone a decouper
// Pj : les autres points du polygone a decouper
// Fi : un point de la fenetre
int coupe(Point S, Point Pj, Point Fi, Point Fii) {
	//retournant un booléen suivant l'intersection possible entre le côté [SPj ] du
	//	polygone et le bord prolongé(une droite) (FiFi + 1) de la fenêtre
	return 0;
}

Point intersection(Point S, Point Pj, Point Fi, Point Fii) {
	//retournant le point d'intersection [SPj ] \ (FiFi+1)
	Point p = {0, 0};
	return p;
}

int visible(Point S, Point Fi, Point Fii) {
	//retournant un booléen si S est visible par rapport à(FiFi + 1)
	return 0;
}