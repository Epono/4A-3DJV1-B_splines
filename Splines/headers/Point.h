#pragma once

class Point {
private:
	float _x;
	float _y;

public:
	Point() : _x(0.f), _y(0.f){}
	Point(float x, float y) : _x(x), _y(y){}

	float getX(){ return _x; }
	float getY(){ return _y; }
};