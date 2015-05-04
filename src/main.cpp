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
#include "../headers/utils.h"

#include <iostream>
#include <windows.h>
#include <GL/glut.h>

int creationState = waitingForFirstClick;

CustomPolygon window;						// Window used to cut another polygon
float windowColor[3] = {0, 0.5f, 0.5f};		// Window color
int windowVerticeToMove = -1;

int presse = 0;								// Stores if the mouse is dragging
/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse clicks
void motion(int x, int y);							// manages mouse motions

void drawPolygon(CustomPolygon cp, float color[], int lineSize);								// draws the polygons
void drawWindow();									// draws the window (algorithm of my bite)
void createMenu();
void menu(int opt);
void colorPicking(int option);
void setPolygonColor(float colors[3], float r, float g, float b);
void write();										// Writes on the top left what's happening
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
	glPointSize(4.0);						// Point size : 4px

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

Point drawBezier(Point A, Point B, Point C, Point D, double t) {
	Point P;

	P.x = pow((1 - t), 3) * A.x + 3 * t * pow((1 - t), 2) * B.x + 3 * (1 - t) * pow(t, 2)* C.x + pow(t, 3)* D.x;
	P.y = pow((1 - t), 3) * A.y + 3 * t * pow((1 - t), 2) * B.y + 3 * (1 - t) * pow(t, 2)* C.y + pow(t, 3)* D.y;

	return P;
}

void drawLine(Point p1, Point p2) {
	glBegin(GL_LINES);
	glVertex2i(p1.x, p1.y);
	glVertex2i(p2.x, p2.y);

	glEnd();
}

void decasteljau() {
	Point POld = window.vertices[0];
	for(double t = 0.0; t <= 1.0; t += 0.01) {
		Point P = drawBezier(window.vertices[0], window.vertices[1], window.vertices[2], window.vertices[3], t);
		printf("%d - %d\n", P.x, P.y);
		drawLine(POld, P);
		POld = P;
	}
}

/*
* Function in charge of refreshing the display of the window
*/
void display() {
	glClear(GL_COLOR_BUFFER_BIT);	// Clears the display
	glColor3f(1.0, 0.0, 0.0);		// Sets the drawing color

	write();

	drawPolygon(window, windowColor, 1); // Draw the window
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
			window.vertices[0] = point;
			window.nbVertices = 1;
			creationState++;
			break;
		case waitingForNextClick:
			point.x = x, point.y = y;
			window.vertices[window.nbVertices++] = point;
			break;
		case selectPoint:
			break;
		}
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		presse = 0;
		windowVerticeToMove = -1;
		if(creationState == selectPoint) {
			for(int i = 0; i < window.nbVertices; i++) {
				int tempX = window.vertices[i].x;
				int tempY = window.vertices[i].y;
				int distance = 10;
				if(abs(tempX - x) < distance && abs(tempY - y) < distance) {
					windowVerticeToMove = i;
					break;
				}
			}
		}
	}

	glutPostRedisplay();		// Refresh display
}

void motion(int x, int y) {
	if(creationState == selectPoint) {
		if(windowVerticeToMove != -1) {
			window.vertices[windowVerticeToMove].x = x;
			window.vertices[windowVerticeToMove].y = y;
		}
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
}

/*
* Function in charge of handling keyboard events
* key : key pressed
* x, y : coordinates of the pointer when the key was pressed ?
*/
void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 'd': // Switch to connected strip lines creation	
		if(creationState != waitingForFirstClick) {
			printf("Switching to window creation\n");
			creationState = waitingForFirstClick;
		}
		break;
	case 'v': // Validates
		creationState = pending;
		break;
	case 'c': // Clear the window
		creationState = waitingForFirstClick;
		window.nbVertices = 0;
		break;
	case 's':
		// select point
		if(creationState != selectPoint) {
			printf("Switching to select point\n");
			creationState = selectPoint;
		}
		break;
	case 127:
		// deletes selected point
		if(windowVerticeToMove != -1) {
			window.vertices[windowVerticeToMove].x = x;
			window.vertices[windowVerticeToMove].y = y;
			int i;
			for(i = windowVerticeToMove; i < window.nbVertices - 1; i++) {
				window.vertices[i] = window.vertices[i + 1];
			}
			window.nbVertices--;
		}
		windowVerticeToMove = -1;
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
	int mainMenu;

	mainMenu = glutCreateMenu(menu);
	glutAddMenuEntry("Polygone (tracer)", 1);
	glutAddMenuEntry("Fenetre (tracer)", 2);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//TODO
/*
* Function to handle menu
*/
void menu(int opt) {
	switch(opt) {
	case 1: // 
		printf("Plus utilisé", opt);
		break;
	case 2: // Creation de fenetre
		break;
		printf("What ? %d choisie mais pas d'option\n", opt);
		break;
	}
}

void setPolygonColor(float colors[3], float r, float g, float b) {
	*colors = r;
	*(colors + 1) = g;
	*(colors + 2) = b;
}

void drawPolygon(CustomPolygon cp, float color[], int lineSize) {
	if(window.nbVertices == 4) {
		decasteljau();
	}

	// Draws vertices of the connected lines strip
	glBegin(GL_POINTS);
	for(int j = 0; j < cp.nbVertices; j++) {
		glVertex2i(cp.vertices[j].x, cp.vertices[j].y);
	}
	glEnd();

	// Draw selected point bigger
	if(windowVerticeToMove != -1) {
		glPointSize(6.0);
		glBegin(GL_POINTS);
		glVertex2i(cp.vertices[windowVerticeToMove].x, cp.vertices[windowVerticeToMove].y);
		glEnd();
		glPointSize(4.0);
	}

	glLineWidth(lineSize);
	// Draws the polygon
	glColor3fv(color);
	glBegin(GL_LINE_STRIP);
	for(int j = 0; j < cp.nbVertices; j++)
		glVertex2i(cp.vertices[j].x, cp.vertices[j].y);
	glEnd();
}

void write() {
	char* truc;
	if(creationState == selectPoint) {
		truc = "Selecting point";
	}
	else if(creationState == waitingForFirstClick || creationState == waitingForNextClick) {
		truc = "Drawing lines";
	}
	else {
		truc = "Unknown action";
	}
	glRasterPos2f(5, 20);

	for(int i = 0; truc[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, truc[i]);
}

