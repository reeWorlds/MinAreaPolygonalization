#include "RBTreeTriangulation.h"

void RBTreeTriangulation::leftRotate(NodeTriangulation* x)
{
	NodeTriangulation* newParent = x->right;

	if (x == root)
	{
		root = newParent;
	}

	x->moveDown(newParent);
	x->right = newParent->left;

	if (newParent->left != nullptr)
	{
		newParent->left->parent = x;
	}

	newParent->left = x;
}

void RBTreeTriangulation::rightRotate(NodeTriangulation* x)
{
	NodeTriangulation* newParent = x->left;

	if (x == root)
	{
		root = newParent;
	}

	x->moveDown(newParent);
	x->left = newParent->right;

	if (newParent->right != nullptr)
	{
		newParent->right->parent = x;
	}

	newParent->right = x;
}

void RBTreeTriangulation::swapColors(NodeTriangulation* x1, NodeTriangulation* x2)
{
	swap(x1->color, x2->color);
}

void RBTreeTriangulation::swapValues(NodeTriangulation* u, NodeTriangulation* v)
{
	swap(u->p1, v->p1);
	swap(u->p2, v->p2);
	swap(u->pLast, v->pLast);
}

void RBTreeTriangulation::fixRedRed(NodeTriangulation* x)
{
	if (x == root)
	{
		x->color = RBTREE1_COLOR::BLACK;
		return;
	}

	NodeTriangulation* parent = x->parent;
	NodeTriangulation* grandparent = parent->parent;
	NodeTriangulation* uncle = x->uncle();

	if (parent->color != RBTREE1_COLOR::BLACK)
	{
		if (uncle != nullptr && uncle->color == RBTREE1_COLOR::RED)
		{
			parent->color = RBTREE1_COLOR::BLACK;
			uncle->color = RBTREE1_COLOR::BLACK;
			grandparent->color = RBTREE1_COLOR::RED;
			fixRedRed(grandparent);
		}
		else
		{
			if (parent->isOnLeft())
			{
				if (x->isOnLeft())
				{
					swapColors(parent, grandparent);
				}
				else
				{
					leftRotate(parent);
					swapColors(x, grandparent);
				}

				rightRotate(grandparent);
			}
			else
			{
				if (x->isOnLeft())
				{
					rightRotate(parent);
					swapColors(x, grandparent);
				}
				else
				{
					swapColors(parent, grandparent);
				}

				leftRotate(grandparent);
			}
		}
	}
}

NodeTriangulation* RBTreeTriangulation::successor(NodeTriangulation* x)
{
	NodeTriangulation* temp = x;

	while (temp->left != nullptr)
	{
		temp = temp->left;
	}

	return temp;
}

NodeTriangulation* RBTreeTriangulation::BSTreplace(NodeTriangulation* x)
{
	if (x->left != nullptr && x->right != nullptr)
	{
		return successor(x->right);
	}

	if (x->left == nullptr && x->right == nullptr)
	{
		return nullptr;
	}

	if (x->left != nullptr)
	{
		return x->left;
	}
	else
	{
		return x->right;
	}
}

void RBTreeTriangulation::deleteNode(NodeTriangulation* v)
{
	NodeTriangulation* u = BSTreplace(v);

	bool uvBlack = ((u == nullptr || u->color == RBTREE1_COLOR::BLACK) && (v->color == RBTREE1_COLOR::BLACK));
	NodeTriangulation* parent = v->parent;

	if (u == nullptr)
	{
		if (v == root)
		{
			root = nullptr;
		}
		else
		{
			if (uvBlack)
			{
				fixDoubleBlack(v);
			}
			else
			{
				if (v->sibling() != nullptr)
				{
					v->sibling()->color = RBTREE1_COLOR::RED;
				}
			}

			if (v->isOnLeft())
			{
				parent->left = nullptr;
			}
			else
			{
				parent->right = nullptr;
			}
		}
		delete v;

		return;
	}

	if (v->left == nullptr || v->right == nullptr)
	{
		if (v == root)
		{
			v->p1 = u->p1;
			v->p2 = u->p2;
			v->pLast = u->pLast;

			v->left = v->right = nullptr;

			delete u;
		}
		else
		{
			if (v->isOnLeft())
			{
				parent->left = u;
			}
			else
			{
				parent->right = u;
			}

			delete v;

			u->parent = parent;
			if (uvBlack)
			{
				fixDoubleBlack(u);
			}
			else
			{
				u->color = RBTREE1_COLOR::BLACK;
			}
		}
		return;
	}

	swapValues(u, v);
	deleteNode(u);
}

void RBTreeTriangulation::fixDoubleBlack(NodeTriangulation* x)
{
	if (x == root)
	{
		return;
	}

	NodeTriangulation* sibling = x->sibling();
	NodeTriangulation* parent = x->parent;

	if (sibling == nullptr)
	{
		fixDoubleBlack(parent);
	}
	else
	{
		if (sibling->color == RBTREE1_COLOR::RED)
		{
			parent->color = RBTREE1_COLOR::RED;
			sibling->color = RBTREE1_COLOR::BLACK;
			if (sibling->isOnLeft())
			{
				rightRotate(parent);
			}
			else
			{
				leftRotate(parent);
			}

			fixDoubleBlack(x);
		}
		else
		{
			if (sibling->hasRedChild())
			{
				if (sibling->left != nullptr && sibling->left->color == RBTREE1_COLOR::RED)
				{
					if (sibling->isOnLeft())
					{
						sibling->left->color = sibling->color;
						sibling->color = parent->color;
						rightRotate(parent);
					}
					else
					{
						sibling->left->color = parent->color;
						rightRotate(sibling);
						leftRotate(parent);
					}
				}
				else
				{
					if (sibling->isOnLeft())
					{
						sibling->right->color = parent->color;
						leftRotate(sibling);
						rightRotate(parent);
					}
					else
					{
						sibling->right->color = sibling->color;
						sibling->color = parent->color;
						leftRotate(parent);
					}
				}

				parent->color = RBTREE1_COLOR::BLACK;
			}
			else
			{
				sibling->color = RBTREE1_COLOR::RED;
				if (parent->color == RBTREE1_COLOR::BLACK)
				{
					fixDoubleBlack(parent);
				}
				else
				{
					parent->color = RBTREE1_COLOR::BLACK;
				}
			}
		}
	}
}

NodeTriangulation* RBTreeTriangulation::leftMost()
{
	if (root == nullptr)
	{
		return nullptr;
	}

	NodeTriangulation* left = root;
	while (left->left != nullptr)
	{
		left = left->left;
	}

	return left;
}

bool RBTreeTriangulation::positionedLeft(Point p1, Point p2, Point p)
{
	if (p1.y < p2.y)
	{
		swap(p1, p2);
	}

	double angle = atan2(p.y - p1.y, p.x - p1.x) - atan2(p2.y - p1.y, p2.x - p1.x);

	if (angle > M_PI)
	{
		angle -= M_PI;
	}
	if (angle < -M_PI)
	{
		angle += M_PI;
	}

	if (angle < 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

RBTreeTriangulation::RBTreeTriangulation()
{
	root = nullptr;
}

RBTreeTriangulation::~RBTreeTriangulation()
{
	clear(root);
}

void RBTreeTriangulation::clear(NodeTriangulation* NodeTriangulation)
{
	if (NodeTriangulation != nullptr)
	{
		clear(NodeTriangulation->left);
		clear(NodeTriangulation->right);

		delete NodeTriangulation;

		root = nullptr;
	}
}

NodeTriangulation* RBTreeTriangulation::getRoot()
{
	return root;
}

NodeTriangulation* RBTreeTriangulation::search(Point p1, Point p2)
{
	NodeTriangulation* temp = root;

	if (p1.y < p2.y)
	{
		swap(p1, p2);
	}

	while (temp != nullptr)
	{
		if (p1 < temp->p1)
		{
			if (temp->left == nullptr)
			{
				break;
			}
			else
			{
				temp = temp->left;
			}
		}
		else if (p1 > temp->p1)
		{
			if (temp->right == nullptr)
			{
				break;
			}
			else
			{
				temp = temp->right;
			}
		}
		else // p1 == temp->p1
		{
			if (p2 == temp->p2)
			{
				break;
			}
			else
			{
				if (positionedLeft(temp->p1, temp->p2, p2))
				{
					if (temp->left == nullptr)
					{
						break;
					}
					else
					{
						temp = temp->left;
					}
				}
				else
				{
					if (temp->right == nullptr)
					{
						break;
					}
					else
					{
						temp = temp->right;
					}
				}
			}
		}
	}

	return temp;
}

void RBTreeTriangulation::insert(Point p1, Point p2)
{
	NodeTriangulation* newNode = new NodeTriangulation(p1, p2);

	if (root == nullptr)
	{
		newNode->color = RBTREE1_COLOR::BLACK;
		root = newNode;
	}
	else
	{
		NodeTriangulation* temp = search(p1, p2);

		if (temp->p1 == p1 && temp->p2 == p2)
		{
			return;
		}

		newNode->parent = temp;

		if (positionedLeft(temp->p1, temp->p2, p1))
		{
			temp->left = newNode;
		}
		else
		{
			temp->right = newNode;
		}

		fixRedRed(newNode);
	}
}

void RBTreeTriangulation::deleteSegment(Point p1, Point p2)
{
	if (root == nullptr)
	{
		return;
	}

	NodeTriangulation* v = search(p1, p2);

	if (v->p1 != p1 || v->p2 != p2)
	{
		return;
	}

	deleteNode(v);
}

NodeTriangulation* RBTreeTriangulation::findFirstLeft(NodeTriangulation* node, Point p)
{
	if (node == nullptr)
	{
		return nullptr;
	}

	if (positionedLeft(node->p1, node->p2, p))
	{
		return findFirstLeft(node->left, p);
	}
	else
	{
		NodeTriangulation* firstLeft = findFirstLeft(node->right, p);
		if (firstLeft != nullptr)
		{
			return firstLeft;
		}
		else
		{
			return node;
		}
	}
}