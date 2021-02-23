#pragma once

#include "NodeTriangulation.h"

class RBTreeTriangulation
{

public:

	NodeTriangulation* root;

	void leftRotate(NodeTriangulation* x);

	void rightRotate(NodeTriangulation* x);

	void swapColors(NodeTriangulation* x1, NodeTriangulation* x2);

	void swapValues(NodeTriangulation* u, NodeTriangulation* v);

	void fixRedRed(NodeTriangulation* x);

	NodeTriangulation* successor(NodeTriangulation* x);

	NodeTriangulation* BSTreplace(NodeTriangulation* x);

	void deleteNode(NodeTriangulation* v);

	void fixDoubleBlack(NodeTriangulation* x);

	NodeTriangulation* leftMost();

	bool positionedLeft(Point p1, Point p2, Point p);

public:

	RBTreeTriangulation();

	~RBTreeTriangulation();

	void clear(NodeTriangulation* NodeTriangulation);

	NodeTriangulation* getRoot();

	NodeTriangulation* search(Point p1, Point p2);

	void insert(Point p1, Point p2);

	void deleteSegment(Point p1, Point p2);

	NodeTriangulation* findFirstLeft(NodeTriangulation* node, Point p);
};