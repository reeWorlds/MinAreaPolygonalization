#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <unordered_map>
#include <cmath>
#include <math.h>
#include <string>
#include <time.h>
#include <random>

class Point
{

public:

	double x, y;

	Point();
	Point(double x, double y);
	Point(const Point& other);

	bool operator ==(const Point& other) const;
	bool operator !=(const Point& other) const;

	bool operator <(const Point& other) const;
	bool operator <=(const Point& other) const;
	bool operator >(const Point& other) const;
	bool operator >=(const Point& other) const;

	double dist(const Point& other);
	static double dist(const Point& a, const Point& b);
	double dist2(const Point& other);
	static double dist2(const Point& a, const Point& b);

	Point operator+(const Point other);
	Point operator-(const Point other);
};