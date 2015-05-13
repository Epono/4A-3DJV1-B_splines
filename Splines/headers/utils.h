#pragma once

#include "Point.h"

// Represents the state of the creation action
typedef enum creationState {
	waitingForFirstClick,
	waitingForNextClick,
	pending,
	selectPoint
};

typedef struct color_rgb {
	color_rgb(float r, float g, float b) :_r(r), _g(g), _b(b) {
	}
	float _r;
	float _g;
	float _b;
} color_rgb;

/* Functions prototypes */
void display();										// manages displaying
void keyboard(unsigned char key, int x, int y);		// manages keyboard inputs
void keyboardSpecial(int key, int x, int y);		// manages keyboard inputs
void mouse(int bouton, int etat, int x, int y);		// manages mouse clicks
void motion(int x, int y);							// manages mouse motions

void drawWindow();									// draws the window (algorithm of my bite)
void createMenu();
void menu(int opt);
void colorPicking(int option);
void setPolygonColor(float colors[3], float r, float g, float b);
void write();										// Writes on the top left what's happening

Point* DC(const std::vector<Point>& p, float t);
void drawBezier(int p, LineStrip& line);
Point drawBezier(Point A, Point B, Point C, Point D, double t);
void drawLine(Point& p1, Point& p2);
void drawCurve(LineStrip& line, int lineSize);

void translate(int xOffset, int yOffset);
void scale(float scaleX, float scaleY);
void rotate(float angle);