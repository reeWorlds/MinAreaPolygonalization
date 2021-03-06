#include "Segment.h"

Segment::Segment()
{

}

Segment::Segment(Point p1, Point p2) : p1(p1), p2(p2)
{

}

Segment::Segment(double x1, double y1, double x2, double y2) : p1(x1, y1), p2(x2, y2)
{

}

Segment::Segment(const Segment& other) : p1(other.p1), p2(other.p2)
{

}

double Segment::getXByY(double y)
{
	return p1.x + (p2.x - p1.x) * abs((y - p1.y) / (p2.y - p1.y));
}