#pragma once

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

// Represents a 2D point (x, y)
typedef struct Point {
	int x;
	int y;
} Point;

// Represents a Polygon
typedef struct CustomPolygon {
	Point vertices[256];
	int nbVertices;
} CustomPolygon;