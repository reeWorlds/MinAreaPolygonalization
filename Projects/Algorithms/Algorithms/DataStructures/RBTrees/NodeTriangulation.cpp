#include "NodeTriangulation.h"

NodeTriangulation::NodeTriangulation(Point _p1, Point _p2)
{
	if (_p1.y < _p2.y)
	{
		swap(_p1, _p2);
	}

	p1 = _p1;
	p2 = _p2;
	pLast = _p1;

	parent = left = right = nullptr;
	color = RBTREE1_COLOR::RED;
}

NodeTriangulation* NodeTriangulation::uncle()
{
	if (parent == nullptr || parent->parent == nullptr)
	{
		return nullptr;
	}

	if (parent->isOnLeft())
	{
		return parent->parent->right;
	}
	else
	{
		return parent->parent->left;
	}
}

bool NodeTriangulation::isOnLeft()
{
	return this == parent->left;
}

NodeTriangulation* NodeTriangulation::sibling()
{
	if (parent == nullptr)
	{
		return nullptr;
	}

	if (isOnLeft())
	{
		return parent->right;
	}

	return parent->left;
}

void NodeTriangulation::moveDown(NodeTriangulation* newParent)
{
	if (parent != nullptr)
	{
		if (isOnLeft())
		{
			parent->left = newParent;
		}
		else
		{
			parent->right = newParent;
		}
	}
	newParent->parent = parent;
	parent = newParent;
}

bool NodeTriangulation::hasRedChild()
{
	return (left != nullptr && left->color == RBTREE1_COLOR::RED) ||
		(right != nullptr && right->color == RBTREE1_COLOR::RED);
}