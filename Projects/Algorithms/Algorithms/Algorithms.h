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
#include "SegmentLinkedList.h"
#include "DataStructures/RBTrees/RBTreeTriangulation.h"

using namespace std;

#define pdd pair<double, double>


class Algorithms
{
private:
	static double angle(Point p);
	static double angle(Point reference, Point direction);

	static int rotationType(Point v1, Point v2);
	static bool intersect(Segment s1, Segment s2);

	static double area(Point p1, Point p2, Point p3);
	static double area(vector <Point> points);

	static vector <vector <int> > precalcPointsUnderStripe(vector <Point> points);
	static int pointsInsideTriangle(vector <vector <int> >& stripes, vector<Point>& points, int i, int j, int k);
	static int debug_pointsInsideTriangle(vector <Point>& points, int i, int j, int k);

	static bool debug_isSimplePolygon(vector <Point> points);

	static vector <vector <Point> > splitBySplitVertex(vector <Point> points);

public:

	static vector <Point> convexHull(vector <Point> points);

	static vector <Point> MAPGreedy(vector <Point> points);

	static vector <tuple<Point, Point, Point> > triangulatePolygon(vector <Point> points);
};