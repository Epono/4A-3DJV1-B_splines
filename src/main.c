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

// Represents a 2D point (x, y)
typedef struct Point {
	int x;
	int y;
} Point;

// Represents the state of the creation action
typedef enum creationState {
	waitingForFirstClick,
	waitingForNextClick
};
int creationState = waitingForFirstClick;

// Represents the state of the creation action
typedef enum creationToolState {
	lineCreationState,
	polygonCreationState
};
int creationToolState = lineCreationState;

int numLines = -1;							// Number of lines stored
Point lines[256][2];						// 2D array of points (store tips)
float linesColor[3] = {0, 0, 0};			// Lines color

int numPolygons = -1;						// Number of polygons to display
Point polygons[256][256];					// 2D array of polygons (store vertexes)
int polygonSize[256];						// Number of vertexes
float polygonsColor[3] = {0.5f, 0.5f, 0};	// Polygons color

/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse inputs

void drawLines();									// draws the lines
void drawPolygons();								// draws the polygons
void createMenu();
void menu(int opt);

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

	drawLines();					//Draws lines
	drawPolygons();				//Draws polygons
	glutSwapBuffers();				//Double buffer ?

	glFlush();						//Forces refresh ?
}

/*
 * Function in charge of handling mouse events (clicking only, not dragging)
 */
void mouse(int button, int state, int x, int y) {
	//if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) 
	//	printf("coords clicked : (%d, %d)\n", x, y);

	/*
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
	switch(creationState) {
	case waitingForFirstClick:
	printf("one clidk");
	++numLines;
	lines[numLines][0].x = x;
	lines[numLines][0].y = y;
	lines[numLines][1].x = x;
	lines[numLines][1].y = y;
	creationState++;
	break;
	case waitingForNextClick:
	printf("2 clidk");
	lines[numLines][1].x = x;
	lines[numLines][1].y = y;
	creationState = waitingForFirstClick;
	break;
	}
	}
	*/

	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		switch(creationState) {
		case waitingForFirstClick:
			switch(creationToolState) {
			case lineCreationState:
				printf("Start of line selected : (%d, %d)\n", x, y);
				++numLines;
				lines[numLines][0].x = x;
				lines[numLines][0].y = y;
				lines[numLines][1].x = x;
				lines[numLines][1].y = y;
				creationState++;
				break;
			case polygonCreationState:
				printf("Start of polygon selected : (%d, %d)\n", x, y);
				++numPolygons;
				polygonSize[numPolygons] = 1;
				polygons[numPolygons][0].x = x;
				polygons[numPolygons][0].y = y;
				polygons[numPolygons][1].x = x;
				polygons[numPolygons][1].y = y;
				creationState++;
				break;
			}
			break;
		case waitingForNextClick:
			switch(creationToolState) {
			case lineCreationState:
				printf("End of line selected : (%d, %d)\n", x, y);
				lines[numLines][1].x = x;
				lines[numLines][1].y = y;
				creationState = waitingForFirstClick;
				break;
			case polygonCreationState:
				printf("New point selected : (%d, %d)\n", x, y);
				polygons[numPolygons][polygonSize[numPolygons]].x = x;
				polygons[numPolygons][polygonSize[numPolygons]].y = y;
				polygonSize[numPolygons]++;
				printf("%d\n", polygonSize[numPolygons]);
				break;
			}
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
		//TODO
		break;
	case 'p': // Switch to polygon creation	
		if(creationToolState != polygonCreationState) {
			printf("Switching to polygon creation\n");
			creationToolState = polygonCreationState;
		}
		break;
	case 'l': // Switch to line creation	
		if(creationToolState != lineCreationState) {
			printf("Switching to line creation\n");
			creationToolState = lineCreationState;
		}
		break;
	case 'c': // Clear the window
		numLines = -1;
		numPolygons = -1;
		for(int i = 0; i < 256; i++) {
			polygonSize[i] = 0;
		}
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

void drawLines() {
	glColor3fv(linesColor);
	glBegin(GL_LINES);
	for(int i = 0; i <= numLines; i++) {
		glVertex2i((lines[i][0].x), lines[i][0].y);
		glVertex2i((lines[i][1].x), lines[i][1].y);
	}
	glEnd();
}

void drawPolygons() {
	glColor3fv(polygonsColor);
	glBegin(GL_POLYGON);
	for(int i = 0; i <= numPolygons; i++) {
		for(int j = 0; j < polygonSize[i]; j++) {
			glVertex2i(polygons[i][j].x, polygons[i][j].y);
		}
	}
	glEnd();
}