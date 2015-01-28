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

#include "../headers/utils.h"
#include "../headers/decoupage.h"
#include "../headers/remplissage.h"

int creationState = waitingForFirstClick;

int creationToolState = polygonCreationState;

int numPolygons = -1;						// Number of polygons to display
CustomPolygon polygons[256];				// 2D array of polygons
float polygonsColor[3] = {0.5f, 0.5f, 0};	// Polygons color

CustomPolygon window;						// Window used to cut another polygon
float windowColor[3] = {0, 0.5f, 0.5f};		// Window color

int presse = 0;								// Stores if the mouse is dragging

/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse clicks
void motion(int x, int y);							// manages mouse motions

void drawPolygons();								// draws the polygons
void drawWindow();									// draws the window (algorithm of my bite)
void createMenu();
void menu(int opt);

// Debug
void printPolygon(int polygonCount);

int main(int argc, char **argv) {
	//Glut and Window Initialization
	glutInit(&argc, argv);										// Initializes Glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);	// RGB display mode, with depth
	glutInitWindowSize(500, 500);								// Sets window's dimensions
	glutInitWindowPosition(100, 100);							// Positions the window
	glutCreateWindow("FunStuffWithOpenGL");						// Title of the window

	gluOrtho2D(0, 500, 500, 0);									// 2D orthogonal frame with xMin, xMax, yMin, yMax

	// OpenGL initialization
	glClearColor(1.0, 1.0, 1.0, 1.0);		// Background color : black ?
	glColor3f(0.0, 0.0, 0.0);				// Color : white
	glPointSize(4.0);						// Point size : 2px

	// Registering of callback functions called by glut
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	//glOrtho(-1, 1.0, -1, 1.0, -1.0, 1.0); // il faut le mettre ?
	createMenu();							// Creates the menu available via right-click
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();         // Launches main OpenGL loop, events handlers
	return 0;				// Should not get here (unless we exit with 'q' ?)
}

/*
 * Function in charge of refreshing the display of the window
 */
void display() {
	glClear(GL_COLOR_BUFFER_BIT);	// Clears the display
	glColor3f(1.0, 0.0, 0.0);		// Sets the drawing color

	drawPolygons();					// Draws polygons
	drawWindow();					// Draws the window (for the kniffey algorithm)
	glutSwapBuffers();				// Double buffer ?

	glFlush();						// Forces refresh ?
}

/*
 * Function in charge of handling mouse events (clicking only, not dragging)
 */
void mouse(int button, int state, int x, int y) {
	Point point;
	CustomPolygon customPolygon;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		presse = 1;
		switch(creationState) {
		case pending:
			printf("Coords clicked (pending state) : (%d, %d)\n", x, y);
			break;
		case waitingForFirstClick:
			printf("Start of polygon selected : (%d, %d)\n", x, y);
			++numPolygons;
			point.x = x, point.y = y;
			Point* points = (Point*) malloc(sizeof(Point) * 256);
			points[0] = point;
			customPolygon.vertices = points, customPolygon.verticesCount = 1;
			polygons[numPolygons] = customPolygon;
			creationState++;
			break;
		case waitingForNextClick:
			printf("New point selected : (%d, %d)\n", x, y);
			point.x = x, point.y = y;
			customPolygon = polygons[numPolygons];
			customPolygon.vertices[customPolygon.verticesCount] = point;
			customPolygon.verticesCount++;
			polygons[numPolygons] = customPolygon;
			break;
		}
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		presse = 0;
	}
	glutPostRedisplay();		// Refresh display
}

void motion(int x, int y) {
	if(presse) {

	}
	//xold = x; //sauvegarde des valeurs courantes de la position de la souris
	//yold = y;

	glutPostRedisplay(); // Rafraichissement de l'affichage
}

/*
* Function in charge of handling keyboard events
* key : key pressed
* x, y : coordinates of the pointer when the key was pressed ?
*/
void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'v': // Validate the polygon
		creationState = pending;
		break;
	case 'p': // Switch to polygon creation	
		if(creationToolState != polygonCreationState || creationState != waitingForFirstClick) {
			printf("Switching to polygon creation\n");
			creationToolState = polygonCreationState;
			creationState = waitingForFirstClick;
		}
		break;
	case 'c': // Clear the window
		numPolygons = -1;
		creationState = waitingForFirstClick;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
	case 'q':
		exit(0);
	}
}

//TODO
/*
 * Creates the menu available via right-click
 */
void createMenu() {
	glutCreateMenu(menu);

	glutAddMenuEntry("Couleurs", 0);
	glutAddMenuEntry("Polygone (tracer)", 1);
	glutAddMenuEntry("Fenetre (tracer)", 2);
	glutAddMenuEntry("Fenetrage (algorithme)", 3);
	glutAddMenuEntry("Remplissage (algorithme)", 4);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//TODO
/*
 * Function to handle menu
 */
void menu(int opt) {
	switch(opt) {
	case 0:
		printf("Choix de la couleur\n");
		break;
	case 1:
		printf("Tracage de polygone\n");
		break;
	case 2:
		printf("Tracage de fenetre\n");
		break;
	case 3:
		printf("Algorithme de fenetrage\n");
		decoupageWiki(&polygons[0], window);
		glutPostRedisplay();
		break;
	case 4:
		printf("Alorithme de remplissage\n");
		break;
	default:
		printf("What ? %d choisie mais pas d'option\n", opt);
		break;
	}
}

void drawPolygons() {
	for(int i = 0; i <= numPolygons; i++) {
		CustomPolygon polygon = polygons[i];

		// Draws vertices of the polygon
		glBegin(GL_POINTS);
		for(int j = 0; j < polygon.verticesCount; j++) {
			glVertex2i(polygon.vertices[j].x, polygon.vertices[j].y);
		}
		glEnd();

		// Draws the polygon
		glColor3fv(polygonsColor);
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < polygon.verticesCount; j++) {
			glVertex2i(polygon.vertices[j].x, polygon.vertices[j].y);
		}
		glVertex2i(polygon.vertices[0].x, polygon.vertices[0].y);
		glEnd();
	}
}

void drawWindow() {
	Point A = {100, 100};
	Point B = {300, 100};
	Point C = {300, 300};
	Point D = {100, 300};
	Point points[] = {A, B, C, D};
	window.verticesCount = 4;
	window.vertices = points;


	// Draws vertices of the window
	glBegin(GL_POINTS);
	for(int j = 0; j < 4; j++) {
		glVertex2i(window.vertices[j].x, window.vertices[j].y);
	}
	glEnd();

	// Draws the window
	glColor3fv(windowColor);
	glBegin(GL_LINE_STRIP);
	for(int j = 0; j < 4; j++) {
		glVertex2i(window.vertices[j].x, window.vertices[j].y);
	}
	glVertex2i(window.vertices[0].x, window.vertices[0].y);
	glEnd();
}

void printPolygon(int polygonCount) {
	CustomPolygon cp = polygons[polygonCount];

	printf("**************** Polygon **************\n");
	for(int i = 0; i < cp.verticesCount; i++) {
		printf("Point %d : (%d, %d)\n", i, cp.vertices[i].x, cp.vertices[i].y);
	}
	printf("**************** End Polygon **************\n");
}

