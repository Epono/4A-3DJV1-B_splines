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


/* prototypes de fonctions */
void affichage(void);                             // modélisation
void display();
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



	/* Entrée dans la boucle principale de glut, traitement des évènements */
	glutMainLoop();         // lancement de la boucle de réception des évènements
	return 0;
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	//draw two points
	glBegin(GL_POINTS);
	for(int i = 0; i < 10; i++) {
		glVertex2i(10 + 5 * i, 110);
	}
	glEnd();

	//draw a line
	glBegin(GL_LINES);
	glVertex2i(10, 10);
	glVertex2i(100, 100);
	glEnd();

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
	// Si on appuie sur le bouton de gauche
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		presse = 1; //le booleen presse passe à 1 (vrai)
		xold = x; //on sauvegardela position de la souris
		yold = y;
	}

	// Si on relache le bouton gauche
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		printf("(%d, %d)\n", x, y);

		presse = 0; //le booleen presse passe à 0 (faux)
		drawLines();
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
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
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3ub(255, 255, 255);
	glBegin(GL_LINE_STRIP);
	glVertex2f(-4.00, 0.00);
	glVertex2f(-3.00, 2.00);
	glVertex2f(-2.00, 0.00);
	glVertex2f(-1.00, 2.00);
	glVertex2f(0.0, 0.00);
	glVertex2f(1.00, 2.00);
	glVertex2f(2.00, 0.00);
	glVertex2f(3.00, 2.00);
	glVertex2f(4.00, 0.00);
	glEnd();
}

