/*******************************************************/
/*					didac.c							   */
/*******************************************************/
/*													   */
/*	Pr�ambule OpenGL sous Glut			               */
/*  ESGI : 2I ann�e 						           */
/*													   */
/*******************************************************/
/*													   */
/*  Fen�tre graphique 2D vierge                        */
/*  Ev�nement souris actif, q pour quitter             */
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
void affichage(void);                             // mod�lisation
void display();
void clavier(unsigned char touche, int x, int y);   // fonction clavier
void mouse(int bouton, int etat, int x, int y);      // fonction souris
void drawLines();
void handleMenu();
void menu(int opt);


/* Programme principal */
int main(int argc,       // argc: nombre d'arguments, argc vaut au moins 1
		 char **argv) {  // argv: tableau de chaines de caract�res, argv[0] contient le nom du programme lanc� (plus un �ventuel chemin)


	/* Initialisation de glut et creation de la fenetre */
	glutInit(&argc, argv);                       // Initialisation
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // mode d'affichage RGB, et test prafondeur
	glutInitWindowSize(500, 500);                // dimension fen�tre
	glutInitWindowPosition(100, 100);           // position coin haut gauche
	glutCreateWindow("A vous de jouer!!!");  // nom

	/* Rep�re 2D d�limitant les abscisses et les ordonn�es*/
	gluOrtho2D(-250.0, 250.0, -250.0, 250.0);

	/* Initialisation d'OpenGL */
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);       // couleur: blanc
	glPointSize(4.0);               // taille d'un point: 2px

	/* Enregistrement des fonctions de rappel
	=> initialisation des fonctions callback appel�es par glut */
	//glutDisplayFunc(affichage);
	glutDisplayFunc(display);
	glutKeyboardFunc(clavier);
	glutMouseFunc(mouse);

	/* rq: le callback de fonction (fonction de rappel) est une fonction qui est pass�e en argument � une
	autre fonction. Ici, le main fait usage des deux fonctions de rappel (qui fonctionnent en m�me temps)
	alors qu'il ne les conna�t pas par avance.*/



	/* Entr�e dans la boucle principale de glut, traitement des �v�nements */
	glutMainLoop();         // lancement de la boucle de r�ception des �v�nements
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
		presse = 1; //le booleen presse passe � 1 (vrai)
		xold = x; //on sauvegardela position de la souris
		yold = y;
	}

	// Si on relache le bouton gauche
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		printf("(%d, %d)\n", x, y);

		presse = 0; //le booleen presse passe � 0 (faux)
		drawLines();
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
}



/* Ev�nement du clavier */
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

