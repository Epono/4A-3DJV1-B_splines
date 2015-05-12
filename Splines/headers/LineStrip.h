#pragma once

#include <vector>
#include "Point.h"
#include "utils.h"

class LineStrip {
private:
	std::vector<Point*> _points;
	color_rgb _color;
public:
	LineStrip();
	LineStrip(std::vector<Point*>);

	std::vector<Point*> getPoints();
	void addPoint(Point& p){ _points.push_back(&p); }

	~LineStrip();
};