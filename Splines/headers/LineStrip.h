#pragma once

#include <vector>
#include "Point.h"
#include "utils.h"

class LineStrip {
private:
	std::vector<Point> _points;
	color_rgb _color;
public:
	LineStrip() : 
		_points(std::vector<Point>()), _color(color_rgb(1.f, 1.f, 1.f)){}
	LineStrip(std::vector<Point> p) 
		: _points(p), _color(color_rgb(1.f,1.f,1.f)){}

	std::vector<Point> getPoints(){ return _points; }
	color_rgb getColor(){ return _color; }
	void addPoint(Point& p){ _points.push_back(p); }

	~LineStrip();
};