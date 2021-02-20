#pragma once

#include "Point.h"

class Segment
{
public:

	Point p1, p2;

	Segment();
	Segment(Point p1, Point p2);
	Segment(double x1, double y1, double x2, double y2);
	Segment(const Segment& other);

};