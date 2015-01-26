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

/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse inputs

void drawPolygons();								// draws the polygons
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
	glClear(GL_COLOR_BUFFER_BIT);	//Clears the display
	glColor3f(1.0, 0.0, 0.0);		//Sets the drawing color

	drawPolygons();					//Draws polygons
	glutSwapBuffers();				//Double buffer ?

	glFlush();						//Forces refresh ?
}

/*
 * Function in charge of handling mouse events (clicking only, not dragging)
 */
void mouse(int button, int state, int x, int y) {
	Point point;
	CustomPolygon customPolygon;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
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
			customPolygon.vertexes = points, customPolygon.vertexesCount = 1;
			polygons[numPolygons] = customPolygon;
			creationState++;
			break;
		case waitingForNextClick:
			printf("New point selected : (%d, %d)\n", x, y);
			point.x = x, point.y = y;
			customPolygon = polygons[numPolygons];
			customPolygon.vertexes[customPolygon.vertexesCount] = point;
			customPolygon.vertexesCount++;
			polygons[numPolygons] = customPolygon;
			break;
		}
	}

	glutPostRedisplay();		// Refresh display
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
	case 'q':
		exit(0);
	}
}

//TODO
/*
 * Creates the menu available via right-click
 */
void createMenu() {
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

//TODO
/*
 * Function to handle menu
 */
void menu(int opt) {

}

void drawPolygons() {
	for(int i = 0; i <= numPolygons; i++) {
		CustomPolygon polygon = polygons[i];

		// Draws vertexes of the polygon
		glBegin(GL_POINTS);
		for(int j = 0; j < polygon.vertexesCount; j++) {
			glVertex2i(polygon.vertexes[j].x, polygon.vertexes[j].y);
		}
		glEnd();

		// Draws the polygon
		glColor3fv(polygonsColor);
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < polygon.vertexesCount; j++) {
			glVertex2i(polygon.vertexes[j].x, polygon.vertexes[j].y);
		}
		glVertex2i(polygon.vertexes[0].x, polygon.vertexes[0].y);
		glEnd();
	}
}

void printPolygon(int polygonCount) {
	CustomPolygon cp = polygons[polygonCount];

	printf("**************** Polygon **************\n");
	for(int i = 0; i < cp.vertexesCount; i++) {
		printf("Point %d : (%d, %d)\n", i, cp.vertexes[i].x, cp.vertexes[i].y);
	}
	printf("**************** End Polygon **************\n");
}