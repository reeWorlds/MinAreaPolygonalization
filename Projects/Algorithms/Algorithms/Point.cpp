#include "Point.h"

Point::Point(): x(0.0), y(0.0)
{

}

Point::Point(double x, double y) : x(x), y(y)
{

}

Point::Point(const Point& other) : x(other.x), y(other.y)
{

}

bool Point::operator ==(const Point& other)
{
	return x == other.x && y == other.y;
}

bool Point::operator !=(const Point& other)
{
	return !(this->operator==(other));
}

bool Point::operator <(const Point& other)
{
	return (x < other.x) || (x == other.x && y < other.y);
}

bool Point::operator <=(const Point& other)
{
	return (x < other.x) || (x == other.x && y <= other.y);
}

bool Point::operator >(const Point& other)
{
	return !this->operator<=(other);
}

bool Point::operator >=(const Point& other)
{
	return !this->operator<(other);
}

double Point::dist(const Point& other)
{
	return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
}

double Point::dist(const Point& a, const Point& b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

double Point::dist2(const Point& other)
{
	return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
}

double Point::dist2(const Point& a, const Point& b)
{
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

Point Point::operator+(const Point other)
{
	return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point other)
{
	return Point(x - other.x, y - other.y);
}