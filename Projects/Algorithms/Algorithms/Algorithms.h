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

	static double dist(Point p1, Point p2);
	static double distanceToSegment(Segment s, Point p);

	static vector <vector <int> > precalcPointsUnderStripe(vector <Point> points);
	static int pointsInsideTriangle(vector <vector <int> >& stripes, vector<Point>& points, int i, int j, int k);
	static int debug_pointsInsideTriangle(vector <Point>& points, int i, int j, int k);

	static bool debug_isSimplePolygon(vector <Point> points);

	static vector <vector <Point> > splitBySplitVertex(vector <Point> points);
	static vector <tuple<Point, Point, Point> > triangulateYMonotone(vector <Point> points);

	static vector <Point> DACMergePolygons(vector <Point> poly1, vector <Point> poly2);
	static vector <Point> MAPDACPrivate(vector <Point> points);
	static pair<vector <Point>, vector <Point> > MAPDAC2Private(vector <Point> points);

	static bool isInside(vector <Point> poly, Point p);
	static bool isInside(Point p1, Point p2, Point p3, Point p);

	static vector <Point> MAP_RANDPrivate(vector <Point> points, mt19937& rng);

	static vector <Point> MAP_RSA1(vector <Point> points, mt19937& rng); // RGT + GP + GE
	static vector <Point> MAP_RSA2(vector <Point> points, mt19937& rng); // RGT + RP + GE
	static vector <Point> MAP_RSA3(vector <Point> points, mt19937& rng); // RGT + GAr
	static vector <Point> MAP_RSA4(vector <Point> points, mt19937& rng); // RT + GP + GE
	static vector <Point> MAP_RSA5(vector <Point> points, mt19937& rng); // RT + RP + GE
	static vector <Point> MAP_RSA6(vector <Point> points, mt19937& rng); // RT + GAr

public:

	static double area(Point p1, Point p2, Point p3);
	static double area(vector <Point> points);

	static vector <Point> convexHull(vector <Point> points);

	static vector <Point> MAPGreedy(vector <Point> points);

	static vector <tuple<Point, Point, Point> > triangulatePolygon(vector <Point> points);

	static vector <Point> MAPPermuteReject(vector <Point> points);

	static vector <Point> MAPDAC(vector <Point> points);

	static vector <Point> MAPDAC2(vector <Point> points);

	static vector <Point> MAP_RAND(vector <Point> points, int q = 200);

	static vector <Point> MAP_RS(vector <Point> points, int q = 200);

	static vector <Point> MAP_Postprocess(vector <Point> points);
};