#pragma once

/**
 * Contains the definition of the differents data structures we're gonna need.
 */

// Represents the state of the creation action
typedef enum creationToolState {
	polygonCreationState
};

// Represents the state of the creation action
typedef enum creationState {
	waitingForFirstClick,
	waitingForNextClick,
	pending
};

// Represents a 2D point (x, y)
typedef struct Point {
	int x;
	int y;
} Point;

// Represents a Line
typedef struct Line {
	Point startPoint;
	Point endPoint;
} Line;

// Represents a Polygon
typedef struct CustomPolygon {
	Point* vertices;
	int verticesCount;
} CustomPolygon;

// Reprensents the Window
typedef struct QuadrangleWindow {
	Point vertices[4];
} QuadrangleWindow;