/*******************************************************/
/*					didac.c							   */
/*******************************************************/
/*													   */
/*	Préambule OpenGL sous Glut			               */
/*  ESGI : 2I année 						           */
/*													   */
/*******************************************************/
/*													   */
/*  Fenêtre graphique 2D vierge                        */
/*  Evènement souris actif, q pour quitter             */
/*													   */
/*******************************************************/



#include<windows.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<stdio.h>

int x0, y0;  // clic souris
char presse;
int anglex, angley, x, y, xold, yold;

int numLines;
typedef enum state {
	waitingforclick,
	clickedonce,
};

typedef struct point {
	int x;
	int y;
}point;


point lines[256][2];


int gState = waitingforclick;
int lineisvalid = 0;
int gHeight;
float gColor[3] = {0, 1, 0};


/* prototypes de fonctions */
void display();										//displaying
void clavier(unsigned char touche, int x, int y);   // fonction clavier
void mouse(int bouton, int etat, int x, int y);      // fonction souris
void drawLines();
void handleMenu();
void menu(int opt);


/* Programme principal */
int main(int argc,       // argc: nombre d'arguments, argc vaut au moins 1
		 char **argv) {  // argv: tableau de chaines de caractères, argv[0] contient le nom du programme lancé (plus un éventuel chemin)


	/* Initialisation de glut et creation de la fenetre */
	glutInit(&argc, argv);                       // Initialisation
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // mode d'affichage RGB, et test prafondeur
	glutInitWindowSize(500, 500);                // dimension fenêtre
	glutInitWindowPosition(100, 100);           // position coin haut gauche
	glutCreateWindow("A vous de jouer!!!");  // nom

	/* Repère 2D délimitant les abscisses et les ordonnées*/
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);

	/* Initialisation d'OpenGL */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);       // couleur: blanc
	glPointSize(4.0);               // taille d'un point: 2px

	/* Enregistrement des fonctions de rappel
	=> initialisation des fonctions callback appelées par glut */
	//glutDisplayFunc(affichage);
	glutDisplayFunc(display);
	glutKeyboardFunc(clavier);
	glutMouseFunc(mouse);

	/* rq: le callback de fonction (fonction de rappel) est une fonction qui est passée en argument à une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en même temps)
	alors qu'il ne les connaît pas par avance.*/

	handleMenu();
	numLines = -1;
	glMatrixMode(GL_MODELVIEW);


	/* Entrée dans la boucle principale de glut, traitement des évènements */
	glutMainLoop();         // lancement de la boucle de réception des évènements
	return 0;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	glClear(GL_COLOR_BUFFER_BIT);
	drawLines();
	glutSwapBuffers();

	glFlush();
}

/*
void mouse(int button, int state, int x, int y) {
// Si on appuie sur le bouton de gauche
if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
x0 = x - 250; //on sauvegarde la position de la souris
y0 = -y + 250;
affichage();
}

}*/

void mouse(int button, int state, int x, int y) {
	/*
	// Si on appuie sur le bouton de gauche
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	presse = 1; //le booleen presse passe à 1 (vrai)
	xold = x; //on sauvegardela position de la souris
	yold = y;
	}

	// Si on relache le bouton gauche


	presse = 0; //le booleen presse passe à 0 (faux)
	drawLines();
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
	*/
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		printf("(%d, %d)\n", x, y);

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		switch(gState) {

		case waitingforclick:
			printf("one clidk");
			++numLines;
			lines[numLines][0].x = x;
			lines[numLines][0].y = y;
			lines[numLines][1].x = x;
			lines[numLines][1].y = y;
			gState++;
			break;
		case clickedonce:
			printf("2 clidk");
			lines[numLines][1].x = x;
			lines[numLines][1].y = y;
			gState = waitingforclick;
			break;
		}
	}
	glutPostRedisplay();
}



/* Evènement du clavier */
void clavier(unsigned char touche, int x, int y) {
	switch(touche) {

	case 'q':/* Quitter le programme */
		exit(0);
	}
}

void handleMenu() {
	int submenu1, submenu2;
	submenu1 = glutCreateMenu(menu);
	glutAddMenuEntry("abc", 1);
	glutAddMenuEntry("ABC", 2);
	submenu2 = glutCreateMenu(menu);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Red", 2);
	glutAddMenuEntry("White", 3);
	glutCreateMenu(menu);
	glutAddMenuEntry("9 by 15", 0);
	glutAddMenuEntry("Times Roman 10", 1);
	glutAddMenuEntry("Times Roman 24", 2);
	glutAddSubMenu("Messages", submenu1);
	glutAddSubMenu("Color", submenu2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int opt) {

}
void drawLines() {
	glColor3fv(gColor);
	glBegin(GL_LINES);
	for(int i = 0; i <= numLines; i++) {
		glVertex2i((lines[i][0].x) - 250, (gHeight - lines[i][0].y) + 250);
		glVertex2i((lines[i][1].x) - 250, (gHeight - lines[i][1].y) + 250);
	}
	glEnd();
}
/*
void drawPolygons() {
glColor3fv(gColor);
glBegin(GL_POLYGON);
for(int i = 0; i <= numPolygons; i++) {
glVertex2i(polygons[i][0].x, gHeight - lines[i][0].y);
glVertex2i(polygons[i][1].x, gHeight - lines[i][1].y);
}
glEnd();
}*/
