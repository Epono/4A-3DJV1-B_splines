#include <cstdlib>
#include <GL/glut.h>
#include <windows.h>

#include <vector>
#include <iostream>

#include <math.h>
#include "utils.h"
#include "Point.h"
#include "LineStrip.h"

int creationState = waitingForFirstClick;

std::vector<LineStrip*> lines;
LineStrip *currentLine = nullptr;

CustomPolygon window;						// Window used to cut another polygon
CustomPolygon windows[256];
int windowsCount;
float windowColor[3] = {0, 0.5f, 0.5f};		// Window color
int windowVerticeToMove = -1;
bool hideControlPoints = false;
float step = 0.01;
float pas = 20;
color_rgb dessinColor = color_rgb(1.f, 0.f, 0.f);

int presse = 0;								// Stores if the mouse is dragging
/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void keyboardSpecial(int key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse clicks
void motion(int x, int y);							// manages mouse motions

void drawPolygon(CustomPolygon cp, float color[], int lineSize);								// draws the polygons
void drawWindow();									// draws the window (algorithm of my bite)
void createMenu();
void menu(int opt);
void colorPicking(int option);
void setPolygonColor(float colors[3], float r, float g, float b);
void write();										// Writes on the top left what's happening

//void decasteljau(CustomPolygon cp);
Point* DC(const std::vector<Point>& p, float t);
void drawBezier(int p, LineStrip& line);
Point drawBezier(Point A, Point B, Point C, Point D, double t);
void drawLine(Point& p1, Point& p2);
void drawCurve(LineStrip& line, int lineSize);

void translate(int x, int y);
void scale(float scaleX, float scaleY);
void rotate(float angle);

// Debug
void printPolygon(int polygonCount);

int main(int argc, char **argv) {
	//Glut and Window Initialization
	glutInit(&argc, argv);										// Initializes Glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	// RGB display mode, with depth
	glutInitWindowSize(1280, 720);								// Sets window's dimensions
	glutInitWindowPosition(100, 100);							// Positions the window
	glutCreateWindow("FunStuffWithOpenGL");						// Title of the window

	gluOrtho2D(0, 1280, 720, 0);									// 2D orthogonal frame with xMin, xMax, yMin, yMax

	// OpenGL initialization
	glClearColor(0.0, 0.0, 0.0, 1.0);		// Background color : black ?
	glColor3f(0.0, 0.0, 0.0);				// Color : white
	glPointSize(4.0);						// Point size : 4px

	// Registering of callback functions called by glut
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	//glOrtho(-1, 1.0, -1, 1.0, -1.0, 1.0); // il faut le mettre ?
	createMenu();							// Creates the menu available via right-click
	glMatrixMode(GL_MODELVIEW);

	currentLine = new LineStrip();

	glutMainLoop();         // Launches main OpenGL loop, events handlers
	return 0;				// Should not get here (unless we exit with 'q' ?)
}

Point* DC(const std::vector<Point>& p, float t) {
	int n = p.size();
	std::vector<Point> q(n);
	for(int i = 0; i < n; ++i)
		q.at(i) = p.at(i);
	for(int k = 1; k < n; ++k)
		for(int i = 0; i < n - k; ++i) {
			q.at(i).setX((1 - t)*q.at(i).getX() + t*q.at(i + 1).getX());
			q.at(i).setY((1 - t)*q.at(i).getY() + t*q.at(i + 1).getY());
		}
	return new Point(q.at(0));
}

void drawBezier(float pas, LineStrip& line) {
	Point A = line.getPoints().at(0), B;
	for(float k = 1.f; k <= pas; ++k) {
		B = *DC(line.getPoints(), k / pas);
		drawLine(A, B);
		A = B;
	}
}

/*void decasteljau(CustomPolygon cp) {
	Point POld = cp.vertices[0];
	for(int i = 0; i < cp.nbVertices - 3; ++i) {
	for(double t = 0.0; t <= 1.0; t += step) {
	Point P = drawBezier(cp.vertices[i], cp.vertices[i + 1], cp.vertices[i + 2], cp.vertices[i + 3], t);
	drawLine(POld, P);
	POld = P;
	}
	}
	}*/
/*
Point drawBezier(Point A, Point B, Point C, Point D, double t) {
float x = pow((1 - t), 3) * A.getX() + 3 * t * pow((1 - t), 2) * B.getX() + 3 * (1 - t) * pow(t, 2)* C.getX() + pow(t, 3)* D.getX(),
y = pow((1 - t), 3) * A.getY() + 3 * t * pow((1 - t), 2) * B.getY() + 3 * (1 - t) * pow(t, 2)* C.getY() + pow(t, 3)* D.getY();

return Point(x,y);
}*/

void drawLine(Point& p1, Point& p2) {
	glBegin(GL_LINES);
	glVertex2f(p1.getX(), p1.getY());
	glVertex2f(p2.getX(), p2.getY());
	glEnd();
}

/*
* Function in charge of refreshing the display of the window
*/
void display() {
	glClear(GL_COLOR_BUFFER_BIT);	// Clears the display

	write();
	for(auto &l : lines)
		drawCurve(*l, 2);
	if(currentLine != nullptr)
		drawCurve(*currentLine, 2);
	//drawPolygon(window, windowColor, 2); // Draw the window

	/*for(int i = 0; i < windowsCount; ++i) {
		drawPolygon(windows[i], windowColor, 1);
		}*/
	glutSwapBuffers();				// Double buffer ?

	glFlush();						// Forces refresh ?
}

/*
* Function in charge of handling mouse events (clicking only, not dragging)
*/
void mouse(int button, int state, int x, int y) {
	Point point(x, y);
	if(currentLine != nullptr) {
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			presse = 1;
			switch(creationState) {
			case pending:
				printf("Coords clicked (pending state) : (%d, %d)\n", x, y);
				break;
			case waitingForFirstClick:
				//window.vertices[0] = point;
				//window.nbVertices = 1;
				currentLine->addPoint(point);
				creationState++;
				break;
			case waitingForNextClick:
				//window.vertices[window.nbVertices++] = point;
				currentLine->addPoint(point);
				break;
			case selectPoint:
				break;
			}
		}
		if(currentLine->getPoints().size() > 0) {
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
				presse = 0;
				windowVerticeToMove = -1;
				std::vector<Point> points = currentLine->getPoints();
				if(creationState == selectPoint) {
					for(int i = 0; i < points.size(); i++) {
						//int tempX = window.vertices[i].getX();
						float tempX = points.at(i).getX();
						//int tempY = window.vertices[i].getY();
						float tempY = points.at(i).getY();
						int distance = 10;
						if(abs(tempX - x) < distance && abs(tempY - y) < distance) {
							windowVerticeToMove = i;
							break;
						}
					}
				}
			}
		}
	}

	glutPostRedisplay();		// Refresh display
}

void motion(int x, int y) {
	if(creationState == selectPoint) {
		if(windowVerticeToMove != -1) {
			std::vector<Point> points = currentLine->getPoints();
			//window.vertices[windowVerticeToMove].setX(x);
			//window.vertices[windowVerticeToMove].setY(y);
			points.at(windowVerticeToMove).setX(x);
			points.at(windowVerticeToMove).setY(y);
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
		if(creationState == selectPoint) {
			creationState = waitingForNextClick;
		}
		else if(creationState != waitingForFirstClick) {
			printf("Switching to window creation\n");
			creationState = waitingForFirstClick;
		}
		break;
	case 'v': // Validates
		creationState = waitingForFirstClick;
		if(currentLine != nullptr) {
			lines.push_back(currentLine);
			currentLine = nullptr;
		}
		break;
	case 'c': // Clear the window
		creationState = waitingForFirstClick;
		currentLine = nullptr;
		break;
	case 's':
		// select point
		if(creationState != selectPoint) {
			printf("Switching to select point\n");
			creationState = selectPoint;
		}
		break;
	case 'h':
		// hide control points
		hideControlPoints = !hideControlPoints;
		break;
	case '-':
		if(pas > 0) --pas;
		break;
	case '+':
		// decrease step
		++pas;
		break;
	case 't':
		translate(20, 20);
		break;
	case 'r':
		rotate(0.1f);
		break;
	case 'o':
		scale(2.0f, 2.0f);
		break;
	case 127:
		// deletes selected point
		if(windowVerticeToMove != -1) {
			window.vertices[windowVerticeToMove].setX(x);
			window.vertices[windowVerticeToMove].setY(y);
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

void keyboardSpecial(int key, int x, int y) {
	int modifier = glutGetModifiers();

	switch(modifier) {
	case 0: // NONE - Translation
		switch(key) {
		case 100: // LEFT
			translate(-10, 0);
			break;
		case 101: // UP
			translate(0, -10);
			break;
		case 102: // RIGHT
			translate(10, 0);
			break;
		case 103: // DOWN
			translate(0, 10);
			break;
		}
		break;
	case 1: // SHIFT - Scaling
		switch(key) {
		case 100: // LEFT
			scale(0.9f, 1.0f);
			break;
		case 101: // UP
			scale(1.0f, 1.1f);
			break;
		case 102: // RIGHT
			scale(1.1f, 1.0f);
			break;
		case 103: // DOWN
			scale(1.0f, 0.9f);
			break;
		}
		break;
	case 2: // CTRL - Rotation
		switch(key) {
		case 100: // LEFT
			rotate(0.05f);
			break;
		case 101: // UP
			rotate(0.05f);
			break;
		case 102: // RIGHT
			rotate(-0.05f);
			break;
		case 103: // DOWN
			rotate(-0.05f);
			break;
		}
		break;
	case 3: // ALT
		break;
	}

	glutPostRedisplay(); // Rafraichissement de l'affichage
}

void colorPicking(int option) {
	switch(option) {
	case 0:
		std::cout << "Vert" << std::endl;
		dessinColor._r = 0.f; dessinColor._g = 1.f; dessinColor._b = 0.f;
		break;
	case 1:
		std::cout << "Bleu" << std::endl;
		dessinColor._r = 0.f; dessinColor._g = 0.f; dessinColor._b = 1.f;
		break;
	case 2:
		std::cout << "Rouge" << std::endl;
		dessinColor._r = 1.f; dessinColor._g = 0.f; dessinColor._b = 0.f;
		break;
	default:
		break;
	}
	display();
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
	glutAddMenuEntry("Nouvelle courbe", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

//TODO
/*
* Function to handle menu
*/
void menu(int opt) {
	switch(opt) {
	case 1: // 
		std::cout << "Nouvelle courbe" << std::endl;
		if(currentLine != nullptr)
			lines.push_back(currentLine);
		currentLine = new LineStrip();
		creationState = waitingForFirstClick;
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

void drawCurve(LineStrip& line, int lineSize) {
	glLineWidth(lineSize);
	glColor3f(1.0f, 0.0f, 0.0f);		// Sets the drawing color
	glBegin(GL_LINE_STRIP);
	for(auto &p : line.getPoints())
		glVertex2f(p.getX(), p.getY());
	glEnd();
	if(!hideControlPoints) {
		// Draws vertices of the connected lines strip
		glBegin(GL_POINTS);
		for(auto &p : line.getPoints())
			glVertex2f(p.getX(), p.getY());
		glEnd();
	}
	if(line.getPoints().size() > 3) {
		color_rgb c = line.getColor();
		glColor3f(c._r, c._g, c._b);		// Sets the drawing color
		drawBezier(pas, line);
	}
}

void drawPolygon(CustomPolygon cp, float color[], int lineSize) {
	glLineWidth(lineSize);
	glColor3f(1.0, 0.0, 0.0);		// Sets the drawing color of bezier
	drawBezier(pas, *currentLine);
	/*if(cp.nbVertices >= 4) {
		decasteljau(cp);
		}*/
	glColor3f(windowColor[0], windowColor[1], windowColor[2]);		// Sets the drawing color

	if(!hideControlPoints) {
		// Draws vertices of the connected lines strip
		glBegin(GL_POINTS);
		for(int j = 0; j < cp.nbVertices; j++) {
			glVertex2i(cp.vertices[j].getX(), cp.vertices[j].getY());
		}
		glEnd();

		// Draw selected point bigger
		// TODO: seulement le dernier
		if(windowVerticeToMove != -1) {
			glPointSize(6.0);
			glBegin(GL_POINTS);
			glVertex2i(cp.vertices[windowVerticeToMove].getX(), cp.vertices[windowVerticeToMove].getY());
			glEnd();
			glPointSize(4.0);
		}

		// Draws the polygon
		glColor3fv(color);
		glBegin(GL_LINE_STRIP);
		for(int j = 0; j < cp.nbVertices; j++)
			glVertex2i(cp.vertices[j].getX(), cp.vertices[j].getY());
		glEnd();
	}
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

// Faire en mode matrice
void translate(int x, int y) {
	for(int i = 0; i < window.nbVertices; i++) {
		window.vertices[i].setX(window.vertices[i].getX() + x);
		window.vertices[i].setY(window.vertices[i].getY() + y);
	}
}

// Faire en mode matrice
// Essayer de le faire par rapport au barycentre
void scale(float scaleX, float scaleY) {

	float sumX = 0;
	float sumY = 0;

	//Calcul du barycentre pour décaler
	for(int i = 0; i < window.nbVertices; i++) {
		sumX += window.vertices[i].getX();
		sumY += window.vertices[i].getY();
	}

	Point barycenter = {sumX / window.nbVertices, sumY / window.nbVertices};

	for(int i = 0; i < window.nbVertices; i++) {

		// Translate barycenter to origin
		window.vertices[i].setX(window.vertices[i].getX() - barycenter.getX());
		window.vertices[i].setY(window.vertices[i].getY() - barycenter.getY());

		// Scale
		window.vertices[i].setX(window.vertices[i].getX() * scaleX);
		window.vertices[i].setY(window.vertices[i].getY() * scaleY);

		// Translation back
		window.vertices[i].setX(window.vertices[i].getX() + barycenter.getX());
		window.vertices[i].setY(window.vertices[i].getY() + barycenter.getY());
	}
}

// Perte d'infos (int - float)
// Global plutot que local !
void rotate(float angle) {
	float x, y;
	float cos_angle = cos(angle);
	float sin_angle = sin(angle);

	float sumX = 0;
	float sumY = 0;

	//Calcul du barycentre pour décaler
	for(int i = 0; i < window.nbVertices; i++) {
		sumX += window.vertices[i].getX();
		sumY += window.vertices[i].getY();
	}

	Point barycenter = {sumX / window.nbVertices, sumY / window.nbVertices};

	for(int i = 0; i < window.nbVertices; i++) {

		// Translate barycenter to origin
		window.vertices[i].setX(window.vertices[i].getX() - barycenter.getX());
		window.vertices[i].setY(window.vertices[i].getY() - barycenter.getY());

		x = window.vertices[i].getX();
		y = window.vertices[i].getY();

		// Rotation around origin
		window.vertices[i].setX((x * cos_angle) + (y * -sin_angle));
		window.vertices[i].setY((x * sin_angle) + (y * cos_angle));

		// Translation back
		window.vertices[i].setX(window.vertices[i].getX() + barycenter.getX());
		window.vertices[i].setY(window.vertices[i].getY() + barycenter.getY());
	}
}