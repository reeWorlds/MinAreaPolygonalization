#pragma once

#include <algorithm>

#include "../../Point.h"

using namespace std;

enum RBTREE1_COLOR
{
	RED,
	BLACK
};

class NodeTriangulation
{

public:

	Point p1, p2;
	Point pLast;

	RBTREE1_COLOR color;
	NodeTriangulation* left;
	NodeTriangulation* right;
	NodeTriangulation* parent;

	NodeTriangulation(Point _p1, Point _p2);

	NodeTriangulation* uncle();

	bool isOnLeft();

	NodeTriangulation* sibling();

	void moveDown(NodeTriangulation* newParent);

	bool hasRedChild();
};