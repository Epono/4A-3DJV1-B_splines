#pragma once

#include "Point.h"

/**
* Contains the definition of the differents data structures we're gonna need.
*/

// Represents the state of the creation action
typedef enum creationState {
	waitingForFirstClick,
	waitingForNextClick,
	pending,
	selectPoint
};

//// Represents a Polygon
//typedef struct CustomPolygon {
//	Point vertices[256];
//	int nbVertices;
//} CustomPolygon;

typedef struct color_rgb{
	color_rgb(float r, float g, float b) :_r(r), _g(g), _b(b){}
	float _r;
	float _g;
	float _b;
} color_rgb;