#include "Algorithms.h"

double Algorithms::angle(Point p)
{
	return atan2(p.y, p.x);
}

double Algorithms::angle(Point reference, Point direction)
{
	double res = angle(direction) - angle(reference);

	res = res > M_PI ? res - 2.0 * M_PI : res;
	res = res < -M_PI ? res + 2.0 * M_PI : res;

	return res;
}

vector <Point> Algorithms::convexHull(vector <Point> points)
{
	vector <Point> res;

	Point leftMost = points[0];

	for (auto& it : points)
	{
		if (it <= leftMost)
		{
			leftMost = it;
		}
	}

	vector <pair<pdd, int> > inf; // {{angle, -dist}, number}

	for (int i = 0; i < points.size(); i++)
	{
		if (points[i] != leftMost)
		{
			inf.push_back({ {angle(points[i] - leftMost), leftMost.dist2(points[i])}, i });
		}
	}

	sort(inf.begin(), inf.end());

	res.push_back(leftMost);
	res.push_back(points[inf[0].second]);
	for (int i = 1; i < inf.size(); i++)
	{
		while (angle(res.back() - res[res.size() - 2], points[inf[i].second] - res.back()) <= 0)
		{
			res.pop_back();
		}
		res.push_back(points[inf[i].second]);
	}

	return res;
}