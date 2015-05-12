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

// Represents a Polygon
typedef struct CustomPolygon {
	Point vertices[256];
	int nbVertices;
} CustomPolygon;

typedef struct color_rgb{
	float r;
	float g;
	float b;
} color_rgb;