#pragma once

#define _USE_MATH_DEFINES

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

#include "Point.h"
#include "Segment.h"

using namespace std;

#define pdd pair<double, double>


class Algorithms
{
public:
	static double angle(Point p);
	static double angle(Point reference, Point direction);

public:

	static vector<Point> convexHull(vector <Point> points);


};