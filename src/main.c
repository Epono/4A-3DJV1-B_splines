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

#include "utils.h"
#include "decoupage.h"
#include "remplissage.h"

int creationState = waitingForFirstClick;

int creationToolState = polygonCreationState; //State to know wich part are we drawing (window or polygon)

CustomPolygon polygon;				// 2D array of polygons
CustomPolygon polygonFenetre;				// 2D array of polygons
float polygonColor[3] = {0.5f, 0.5f, 0};	// Polygons color

CustomPolygon window;						// Window used to cut another polygon
float windowColor[3] = {0, 0.5f, 0.5f};		// Window color

int presse = 0;								// Stores if the mouse is dragging
int fenetrage = 0;
/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse clicks
void motion(int x, int y);							// manages mouse motions

void drawPolygon(CustomPolygon cp, float color[]);								// draws the polygons
void drawWindow();									// draws the window (algorithm of my bite)
void createMenu();
void menu(int opt);
void colorPicking(int option);
void write();										// Writes on the top left what's happening
void writePointCoordinates(Point p);
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

	write();

	if(fenetrage == 1)
		drawPolygon(polygonFenetre, polygonColor); //Draw the polygon
	else
		drawPolygon(polygon, polygonColor);
	drawPolygon(window, windowColor); // Draw the window
	glutSwapBuffers();				// Double buffer ?

	glFlush();						// Forces refresh ?
}

/*
* Function in charge of handling mouse events (clicking only, not dragging)
*/
void mouse(int button, int state, int x, int y) {
	Point point;

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		presse = 1;
		switch(creationState) {
		case pending:
			printf("Coords clicked (pending state) : (%d, %d)\n", x, y);
			break;
		case waitingForFirstClick:
			point.x = x, point.y = y;
			switch(creationToolState) {
			case polygonCreationState:
				polygon.vertices[0] = point;
				polygon.nbVertices = 1;
				break;
			case windowCreationState:
				window.vertices[0] = point;
				window.nbVertices = 1;
				break;
			}
			creationState++;
			break;
		case waitingForNextClick:
			point.x = x, point.y = y;
			switch(creationToolState) {
			case polygonCreationState:
				polygon.vertices[polygon.nbVertices++] = point;
				break;
			case windowCreationState:
				window.vertices[window.nbVertices++] = point;
				break;
			}
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
	case 'w': // Switch to polygon creation	
		if(creationToolState != windowCreationState || creationState != waitingForFirstClick) {
			printf("Switching to window creation\n");
			creationToolState = windowCreationState;
			creationState = waitingForFirstClick;
		}
		break;
	case 'p': // Switch to polygon creation	
		if(creationToolState != polygonCreationState || creationState != waitingForFirstClick) {
			printf("Switching to polygon creation\n");
			creationToolState = polygonCreationState;
			creationState = waitingForFirstClick;
		}
		break;
	case 'v': // Validates
		creationState = pending;
		break;
	case 'c': // Clear the window
		creationToolState = windowCreationState;
		creationState = waitingForFirstClick;
		window.nbVertices = 0;
		polygon.nbVertices = 0;
		polygonFenetre.nbVertices = 0;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
	case 'q':
		exit(0);
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
}

//TODO
/*
* Creates the menu available via right-click
*/
void createMenu() {
	int mainMenu, colorMenu;
	colorMenu = glutCreateMenu(colorPicking);
	glutAddMenuEntry("Vert", 0);
	glutAddMenuEntry("Bleu", 1);
	glutAddMenuEntry("Rouge", 2);

	mainMenu = glutCreateMenu(menu);
	glutAddSubMenu("Couleurs", colorMenu);
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
	case 1: //Creation de polygone
		creationToolState = polygonCreationState;
		break;
	case 2: // Creation de fenetre
		creationToolState = windowCreationState;
		break;
	case 3:
		printf("Algorithme de fenetrage\n");
		fenetrage = 1;
		SutherlandHodgman(polygon, window, &polygonFenetre);
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

void colorPicking(int option) {
	switch(option) {
	case 0:
		printf("Vert\n");
		polygonColor[0] = 0.f;
		polygonColor[1] = 1.f;
		polygonColor[2] = 0.f;
		break;
	case 1:
		printf("Bleu\n");
		polygonColor[0] = 0.f;
		polygonColor[1] = 0.f;
		polygonColor[2] = 1.f;
		break;
	case 2:
		printf("Rouge\n");
		polygonColor[0] = 1.f;
		polygonColor[1] = 0.f;
		polygonColor[2] = 0.f;
		break;
	default:
		break;
	}
	display();
}

void drawPolygon(CustomPolygon cp, float color[]) {
	// Draws vertices of the polygon
	glBegin(GL_POINTS);
	for(int j = 0; j < cp.nbVertices; j++)
		glVertex2i(cp.vertices[j].x, cp.vertices[j].y);
	glEnd();

	// Draws the polygon
	glColor3fv(color);
	glBegin(GL_LINE_STRIP);
	for(int j = 0; j < cp.nbVertices; j++)
		glVertex2i(cp.vertices[j].x, cp.vertices[j].y);
	glVertex2i(cp.vertices[0].x, cp.vertices[0].y);
	glEnd();
}

void printPolygon(int polygonCount) {
	printf("**************** Polygon **************\n");
	for(int i = 0; i < polygon.nbVertices; i++) {
		printf("Point %d : (%d, %d)\n", i, polygon.vertices[i].x, polygon.vertices[i].y);
	}
	printf("**************** End Polygon **************\n");
}

void write() {
	char* truc;
	if(creationToolState == windowCreationState) {
		truc = "Drawing the window";
	}
	else if(creationToolState == polygonCreationState) {
		truc = "Drawing the polygon";
	}
	else {
		truc = "Unknown action";
	}
	glRasterPos2f(5, 20);

	for(int i = 0; truc[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, truc[i]);

	Point p = {50, 50};
	writePointCoordinates(p);
}

/*
void writePointCoordinates(Point p) {
char *str0 = "(";
printf("%s\n", str0);

char str1[3];
sprintf_s(str1, sizeof(char) * 3, "%d", p.x);
printf("%s\n", str1);

char *str2 = ", ";
printf("%s\n", str2);

char str3[3];
sprintf_s(str3, sizeof(char) * 3, "%d", p.y);
printf("%s\n", str3);

char *str4 = ")";
printf("%s\n", str4);

//char *str[1000];
char* str = (char*) malloc(1000 * sizeof(char));
strcat_s(str, 1000 * sizeof(char), str0);
strcat_s(str, 1000 * sizeof(char), str1);
strcat_s(str, 1000 * sizeof(char), str2);
strcat_s(str, 1000 * sizeof(char), str3);
strcat_s(str, 1000 * sizeof(char), str4);
str[999] = '\0';


//int size = strlen(str1);
//size++;
//int nieuwSize = size + 4;
//char* nieuw = (char*) malloc(nieuwSize);
//strcpy_s(nieuw, nieuwSize, str1);
//nieuw[size] = '\0';
//strcat_s(nieuw, nieuwSize, ".cpt"); // <-- crash
//puts(nieuw);


}
*/


/*
glRasterPos2f(polygon.vertices[j].x, polygon.vertices[j].y);
for(int i = 0; i < 9; i++) {
char truc;
if(i == 0) {
truc = '(';
}
else if(i == 4) {
truc = ',';
}
else if(i == 5) {
truc = ' ';
}
else if(i == 9) {
truc = ')';
}
else if(i > 0 && i << 4) {
char c[3];
//sprintf(c, "%d", polygon.vertices[j].x);
truc = c[i - 1];
}
else if(i > 5 && i << 9) {
char c[3];
//sprintf(c, "%d", polygon.vertices[j].x);
truc = c[i - 6];
}
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, truc);
}

Point A = {100, 100};
Point B = {300, 100};
Point C = {300, 300};
Point D = {100, 300};
Point points[] = {A, B, C, D};
window.nbVertices = 4;
window.vertices[0] = points[0];
window.vertices[1] = points[1];
window.vertices[2] = points[2];
window.vertices[3] = points[3];
*/
