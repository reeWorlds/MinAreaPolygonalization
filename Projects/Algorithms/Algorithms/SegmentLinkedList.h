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

#include "Point.h"
#include "Segment.h"

using namespace std;

class SegmentLinkedList
{
public:

	int last;
	vector <pair<Segment, pair<int, int> > > m; // {segment, {prev, next}}

	SegmentLinkedList();

	void add(Segment s);
	void erase(int i);
	void clearInvalidSegments();
};