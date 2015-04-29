#pragma once

#include <vector>
#include "Point.h"

class LineStrip {
private:
	std::vector<Point*> _points;

public:
	LineStrip();
	LineStrip(std::vector<Point*>);

	std::vector<Point*> getPoints();

	~LineStrip();
};