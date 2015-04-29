#pragma once
class Point {
private:
	float _x;
	float _y;

public:
	Point();
	Point(float, float);

	float getX();
	float getY();

	~Point();
};