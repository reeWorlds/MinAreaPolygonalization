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

int Algorithms::rotationType(Point v1, Point v2)
{
	return (v1.x * v2.y - v1.y * v2.x > 0) ? 1 : -1;
}

bool Algorithms::intersect(Segment s1, Segment s2)
{
	int rotation1 = rotationType(s1.p2 - s1.p1, s2.p1 - s1.p2);
	int rotation2 = rotationType(s1.p2 - s1.p1, s2.p2 - s1.p2);

	int rotation3 = rotationType(s2.p2 - s2.p1, s1.p1 - s2.p2);
	int rotation4 = rotationType(s2.p2 - s2.p1, s1.p2 - s2.p2);

	if (rotation1 != rotation2 && rotation3 != rotation4)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Algorithms::dist(Point p1, Point p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double Algorithms::distanceToSegment(Segment s, Point p)
{
	double dist = min(sqrt((s.p1.x - p.x) * (s.p1.x - p.x) + (s.p1.y - p.y) * (s.p1.y - p.y)),
		sqrt((s.p2.x - p.x) * (s.p2.x - p.x) + (s.p2.y - p.y) * (s.p2.y - p.y)));

	if (abs(angle(Point(s.p2.x - s.p1.x, s.p2.y - s.p1.y), Point(p.x - s.p2.x, p.y - s.p2.y))) > M_PI / 2.0 &&
		abs(angle(Point(s.p1.x - s.p2.x, s.p1.y - s.p2.y), Point(p.x - s.p1.x, p.y - s.p1.y))) > M_PI / 2.0)
	{
		double len = sqrt((s.p1.x - s.p2.x) * (s.p1.x - s.p2.x) + (s.p1.y - s.p2.y) * (s.p1.y - s.p2.y));
		double altDist = abs((s.p2.x - s.p1.x) * (s.p1.y - p.y) - (s.p1.x - p.x) * (s.p2.y - s.p1.y)) / len;

		dist = min(dist, altDist);
	}

	return dist;
}

double Algorithms::area(Point p1, Point p2, Point p3)
{
	return 0.5 * abs((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));
}

double Algorithms::area(vector <Point> points)
{
	if (points.size() == 0)
	{
		return 0.0;
	}

	points.push_back(points[0]);

	double res = 0.0;

	for (int i = 1; i < points.size(); i++)
	{
		res += (points[i].x - points[i - 1].x) * (points[i].y + points[i - 1].y) / 2.0;
	}

	return res;
}

vector <vector <int> > Algorithms::precalcPointsUnderStripe(vector <Point> points)
{
	vector <vector <int> > res(points.size(), vector<int>(points.size(), 0));

	vector <pair<Point, int> > inf; // {point, index}
	for (int i = 0; i < points.size(); i++)
	{
		inf.push_back({ points[i], i });
	}
	sort(inf.begin(), inf.end());

	for (int i = 1; i < points.size(); i++)
	{
		int myI = inf[i].second;

		vector <pair<pdd, int> > infAngle; // {angle, index}
		for (int j = 0; j < i; j++)
		{
			int myJ = inf[j].second;

			if (inf[j].first.x < inf[i].first.x)
			{
				double a = angle(points[myJ] - points[myI]);
				a = a < 0 ? -M_PI - a : M_PI - a;

				infAngle.push_back({ {a, points[myI].dist2(points[myJ])}, myJ });
			}
			else
			{
				res[myI][myI]++;
			}
		}

		sort(infAngle.begin(), infAngle.end());

		if (infAngle.size() == 0)
		{
			continue;
		}

		res[myI][infAngle[0].second] = res[infAngle[0].second][myI] = res[myI][myI];
		for (int j = 1; j < infAngle.size(); j++)
		{
			int myJ = infAngle[j].second, myJP = infAngle[j - 1].second;

			if (points[myJ].x == points[myJP].x)
			{
				res[myI][myJ] = res[myJ][myI] = res[myI][myJP] + 1;
			}
			else if (points[myJ].x < points[myJP].x)
			{
				res[myI][myJ] = res[myJ][myI] = res[myI][myJP] + res[myJP][myJ] - res[myJP][myJP] + 1;
			}
			else
			{
				res[myI][myJ] = res[myJ][myI] = res[myI][myJP] - res[myJP][myJ] + res[myJ][myJ];
			}
		}
	}

	return res;
}

int Algorithms::pointsInsideTriangle(vector <vector <int> >& stripes, vector<Point>& points, int i, int j, int k)
{
	if (points[i].x < points[j].x)
	{
		swap(i, j);
	}
	if (points[i].x < points[k].x)
	{
		swap(i, k);
	}
	if (points[j].x < points[k].x)
	{
		swap(j, k);
	}

	double midY = points[k].y + (points[j].x - points[k].x) / (points[i].x - points[k].x) * (points[i].y - points[k].y);

	if (midY >= points[j].y)
	{
		return stripes[i][k] - stripes[i][j] - stripes[j][k] - 1 + stripes[j][j];
	}
	else
	{
		return  stripes[i][j] + stripes[j][k] - stripes[i][k] - stripes[j][j];
	}
}

int Algorithms::debug_pointsInsideTriangle(vector <Point>& points, int i, int j, int k)
{
	int res = 0;

	double s = area(points[i], points[j], points[k]);

	for (int l = 0; l < points.size(); l++)
	{
		if (l == i || l == j || l == k)
		{
			continue;
		}

		double ts = area(points[i], points[j], points[l]) + area(points[i], points[k], points[l])
			+ area(points[j], points[k], points[l]);

		if (abs(ts - s) < 1.0e-8)
		{
			res++;
		}
	}

	return res;
}

bool Algorithms::debug_isSimplePolygon(vector <Point> points)
{
	set <Point> s;

	for (auto it : points)
	{
		s.insert(it);
	}

	if (s.size() != points.size())
	{
		//cout << "Points repeat------------------------\n";

		return false;
	}

	points.push_back(points[0]);

	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = i + 2; j < points.size() - 1; j++)
		{
			if (points[i] != points[j] && points[i] != points[j + 1]
				&& points[i + 1] != points[j] && points[i] != points[j + 1])
			{
				if (intersect(Segment(points[i], points[i + 1]), Segment(points[j], points[j + 1])))
				{
					//cout << "Segments intersect------------------------\n";

					return false;
				}
			}
		}
	}

	return true;
}

vector <vector <Point> > Algorithms::splitBySplitVertex(vector <Point> points)
{
	int n = points.size();
	points.push_back(points[0]);
	points.push_back(points[1]);

	{
		// check for the counterclockwise order
		int maxYI = 1;
		for (int i = 2; i <= n; i++)
		{
			if (points[i].y > points[maxYI].y)
			{
				maxYI = i;
			}
		}

		if (angle(points[maxYI + 1] - points[maxYI]) < angle(points[maxYI - 1] - points[maxYI]))
		{

		}
		else
		{
			reverse(points.begin(), points.end());
		}
	}

	// find all bad nodes
	vector <int> isGood(n + 2, 1);

	for (int i = 1; i <= n; i++)
	{
		if (points[i - 1].y < points[i].y && points[i + 1].y < points[i].y)
		{
			if (angle(points[i - 1] - points[i]) < angle(points[i + 1] - points[i]))
			{
				isGood[i] = 0;
			}
		}
	}

	// sort points in scanline order
	vector <pair<double, pair<Point, int> > > inf; // {y, {Point, index} }
	for (int i = 1; i <= n; i++)
	{
		inf.push_back({ points[i].y , {points[i], i} });
	}
	sort(inf.begin(), inf.end());
	reverse(inf.begin(), inf.end());

	// add aditional edges to remove bad points
	vector <Segment> edges;
	for (int i = 1; i <= n; i++)
	{
		edges.push_back(Segment(points[i - 1], points[i]));
	}
	RBTreeTriangulation tree;

	for (auto it : inf)
	{
		Point p = it.second.first;
		int i = it.second.second;

		if (isGood[i] == 1)
		{
			if (points[i - 1].y > points[i].y)
			{
				tree.deleteSegment(points[i - 1], points[i]);
			}
			if (points[i + 1].y > points[i].y)
			{
				tree.deleteSegment(points[i + 1], points[i]);
			}

			NodeTriangulation* leftSegment = tree.findFirstLeft(tree.root, p);
			if (leftSegment != nullptr)
			{
				leftSegment->pLast = p;
			}

			if (points[i - 1].y < points[i].y)
			{
				tree.insert(points[i - 1], points[i]);
			}
			if (points[i + 1].y < points[i].y)
			{
				tree.insert(points[i + 1], points[i]);
			}
		}
		else
		{
			NodeTriangulation* leftSegment = tree.findFirstLeft(tree.root, p);
			edges.push_back({ p, leftSegment->pLast });
			leftSegment->pLast = p;

			if (points[i - 1].y < points[i].y)
			{
				tree.insert(points[i - 1], points[i]);
			}
			if (points[i + 1].y < points[i].y)
			{
				tree.insert(points[i + 1], points[i]);
			}
		}
	}

	vector <vector <Point> > faces;

	map<Point, int> pointNumber; // number of a point
	vector <Point> points2; // points
	vector <set<pair<double, int> > > grA; // for each point: {angle with edge-neibor, index of neibor in gr}
	vector <vector <pair<Point, pair<double, int> > > > gr; // for each point: {neibor point,{ angle, 0-1 if used} }

	// numerate all points
	int pointIndex = 0;
	for (auto& p : points)
	{
		if (pointNumber.count(p) == 0)
		{
			points2.push_back(p);
			pointNumber[p] = pointIndex;

			pointIndex++;
		}
	}

	// build grA and gr
	gr.resize(pointIndex);
	grA.resize(pointIndex);

	for (auto s : edges)
	{
		int i1 = pointNumber[s.p1], i2 = pointNumber[s.p2];

		{// s.p1
			double angle = atan2(s.p2.y - s.p1.y, s.p2.x - s.p1.x);

			grA[i1].insert({ angle, int(gr[i1].size()) });
			gr[i1].push_back({ s.p2, {angle, 0 } });
		}

		{// s.p2
			double angle = atan2(s.p1.y - s.p2.y, s.p1.x - s.p2.x);

			grA[i2].insert({ angle, int(gr[i2].size()) });
			gr[i2].push_back({ s.p1, {angle, 0 } });
		}
	}

	// construct all faces
	for (int i = 0; i < points2.size(); i++)
	{
		for (int j = 0; j < gr[i].size(); j++)
		{
			if (gr[i][j].second.second == 1)
			{
				continue;
			}

			vector <Point> face;
			face.push_back(points2[i]);

			Point p = gr[i][j].first;
			double angle = gr[i][j].second.first;

			gr[i][j].second.second = 1;
			while (p != face[0])
			{
				face.push_back(p);

				Point prevP = face[face.size() - 2];
				angle = atan2(prevP.y - p.y, prevP.x - p.x);

				int index = pointNumber[p];

				auto it = grA[index].upper_bound({ angle, -1 });
				if (it == grA[index].begin())
				{
					it = grA[index].end();
					it--;
				}
				else
				{
					it--;
				}

				int grI = it->second;
				angle = it->first;
				gr[index][grI].second.second = 1;
				p = gr[index][grI].first;
			}

			faces.push_back(face);
		}
	}

	// find outer face
	double maxArea = 0.0;
	int maxAreaI = 0;
	for (int i = 0; i < faces.size(); i++)
	{
		double myArea = faces[i].size();

		if (myArea > maxArea)
		{
			maxArea = myArea;
			maxAreaI = i;
		}
	}
	faces.erase(faces.begin() + maxAreaI);

	return faces;
}

vector <tuple<Point, Point, Point> > Algorithms::triangulateYMonotone(vector <Point> points)
{
	{
		// check for the counterclockwise order
		int n = points.size();

		points.push_back(points[0]);
		points.push_back(points[1]);

		int maxYI = 1;
		for (int i = 2; i <= n; i++)
		{
			if (points[i].y > points[maxYI].y)
			{
				maxYI = i;
			}
		}

		if (angle(points[maxYI + 1] - points[maxYI]) < angle(points[maxYI - 1] - points[maxYI]))
		{

		}
		else
		{
			reverse(points.begin(), points.end());
		}

		points.pop_back();
		points.pop_back();
	}

	int minI = 0, maxI = 0;

	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].y > points[maxI].y)
		{
			maxI = i;
		}

		if (points[i].y < points[minI].y)
		{
			minI = i;
		}
	}

	vector <Point> leftP, rightP;
	for (int i = maxI + 1; ; i++)
	{
		if (i == points.size())
		{
			i = 0;
		}
		if (i == minI)
		{
			break;
		}

		leftP.push_back(points[i]);
	}
	for (int i = maxI - 1; ; i--)
	{
		if (i == -1)
		{
			i = points.size() - 1;
		}
		if (i == minI)
		{
			break;
		}

		rightP.push_back(points[i]);
	}

	vector <tuple<Point, Point, Point> > triangles;

	int leftI = 0, rightI = 0;

	Point mainPoint = points[maxI];
	vector <Point> cacheL, cacheR;
	while (leftI != leftP.size() || rightI != rightP.size())
	{
		if (leftI != leftP.size() && (rightI == rightP.size() || leftP[leftI].y > rightP[rightI].y))
		{
			//add left point
			Point p = leftP[leftI];
			leftI++;

			// delete left triangles
			while (cacheL.size() != 0)
			{
				Point prev = cacheL.back();
				Point prev2;
				if (cacheL.size() > 1)
				{
					prev2 = cacheL[cacheL.size() - 2];
				}
				else
				{
					prev2 = mainPoint;
				}

				Point reference = Point(prev.x - prev2.x, prev.y - prev2.y);
				Point direction = Point(p.x - prev.x, p.y - prev.y);
				double a = angle(reference, direction);

				if (a > 0)
				{
					triangles.push_back(tuple<Point, Point, Point>(p, prev, prev2));
					cacheL.pop_back();
				}
				else
				{
					break;
				}
			}

			// delete right triangles
			if (cacheR.size() != 0)
			{
				Point newMain = cacheR.back();

				while (cacheR.size() > 0)
				{
					Point p1 = cacheR.back(), p2;
					cacheR.pop_back();
					if (cacheR.size() == 0)
					{
						p2 = mainPoint;
					}
					else
					{
						p2 = cacheR.back();
					}

					triangles.push_back(tuple<Point, Point, Point>(p, p1, p2));
				}

				mainPoint = newMain;
			}

			cacheL.push_back(p);
		}
		else
		{
			// add right point
			Point p = rightP[rightI];
			rightI++;

			// delete right triangles
			while (cacheR.size() != 0)
			{
				Point prev = cacheR.back();
				Point prev2;
				if (cacheR.size() > 1)
				{
					prev2 = cacheR[cacheR.size() - 2];
				}
				else
				{
					prev2 = mainPoint;
				}

				Point reference = Point(prev.x - prev2.x, prev.y - prev2.y);
				Point direction = Point(p.x - prev.x, p.y - prev.y);
				double a = angle(reference, direction);

				if (a < 0)
				{
					triangles.push_back(tuple<Point, Point, Point>(p, prev, prev2));
					cacheR.pop_back();
				}
				else
				{
					break;
				}
			}

			// delete left triangles
			if (cacheL.size() != 0)
			{
				Point newMain = cacheL.back();

				while (cacheL.size() > 0)
				{
					Point p1 = cacheL.back(), p2;
					cacheL.pop_back();
					if (cacheL.size() == 0)
					{
						p2 = mainPoint;
					}
					else
					{
						p2 = cacheL.back();
					}

					triangles.push_back(tuple<Point, Point, Point>(p, p1, p2));
				}

				mainPoint = newMain;
			}

			cacheR.push_back(p);
		}
	}

	Point minPoint = points[minI];
	if (cacheL.size() != 0)
	{
		cacheL.insert(cacheL.begin(), mainPoint);

		for (int i = 1; i < cacheL.size(); i++)
		{
			triangles.push_back(tuple<Point, Point, Point>(cacheL[i - 1], cacheL[i], minPoint));
		}
	}
	else
	{
		cacheR.insert(cacheR.begin(), mainPoint);

		for (int i = 1; i < cacheR.size(); i++)
		{
			triangles.push_back(tuple<Point, Point, Point>(cacheR[i - 1], cacheR[i], minPoint));
		}
	}

	return triangles;
}

bool Algorithms::isInside(vector <Point> poly, Point p)
{
	double windmill = 0.0;

	for (int i = 1; i < poly.size(); i++)
	{
		windmill += angle(Point(poly[i - 1].x - p.x, poly[i - 1].y - p.y), Point(poly[i].x - p.x, poly[i].y - p.y));
	}
	windmill += angle(Point(poly.back().x - p.x, poly.back().y - p.y), Point(poly[0].x - p.x, poly[0].y - p.y));

	// windmill ~= -2Pi/0/2Pi

	if (abs(windmill) < 0.5)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Algorithms::isInside(Point p1, Point p2, Point p3, Point p)
{
	double s = area(p1, p2, p3);

	double ts = area(p1, p2, p) + area(p2, p3, p) + area(p3, p1, p);

	if (abs(s - ts) < 1.0e-8)
	{
		return true;
	}
	else
	{
		return false;
	}
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

vector <Point> Algorithms::MAPGreedy(vector <Point> points)
{
	vector <pair<Point, int> > input; // {point, index}
	vector <vector <int> > stripes = precalcPointsUnderStripe(points);
	for (int i = 0; i < points.size(); i++)
	{
		input.push_back({ points[i], i });
	}

	vector <pair<Point, int> > res;

	{// erase points from CH
		vector <Point> CH = convexHull(points);

		for (auto p : CH)
		{
			for (int i = 0; i < input.size(); i++)
			{
				if (p == input[i].first)
				{
					res.push_back(input[i]);
					input.erase(input.begin() + i);

					break;
				}
			}
		}
	}
	res.push_back(res[0]);

	// start building inside area
	vector <SegmentLinkedList> info(input.size());
	for (int i = 0; i < input.size(); i++)
	{
		for (int j = 1; j < res.size(); j++)
		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[j - 1].second, res[j].second);

			if (pointsInside == 0)
			{
				info[i].add(Segment(res[j - 1].first, res[j].first));
			}
		}
	}

	while (input.size() > 0)
	{
		double bestArea = -1.0;
		int bestPointI = 0;
		Point bestPoint;
		Segment bestSegment;

		// find best point
		for (int i = 0; i < input.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				double tArea = area(points[input[i].second], info[i].m[pointer].first.p1, info[i].m[pointer].first.p2);

				if (tArea > bestArea)
				{
					bestArea = tArea;
					bestPointI = i;
					bestPoint = points[input[i].second];
					bestSegment = info[i].m[pointer].first;
				}

				pointer = info[i].m[pointer].second.first;
			}
		}

		if (bestArea < -0.5)
		{
			//cout << "Unable to connect all poits\n";

			break;
		}

		Point p1 = bestSegment.p1, p2 = bestPoint, p3 = bestSegment.p2;
		int p1i, p2i = input[bestPointI].second, p3i;

		// add new point to res
		for (int i = 0; i < res.size() - 1; i++)
		{
			if (res[i].first == p1 && res[i + 1].first == p3)
			{
				p1i = res[i].second;
				p3i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}

			if (res[i].first == p3 && res[i + 1].first == p1)
			{
				p3i = res[i].second;
				p1i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}
		}

		// erase triangles with deleted segment
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if ((info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
					(info[i].m[pointer].first.p1 == p3 && info[i].m[pointer].first.p2 == p1))
				{
					info[i].erase(pointer);
				}

				pointer = nxt;
			}
		}

		// delete point
		input.erase(input.begin() + bestPointI);
		info.erase(info.begin() + bestPointI);

		// delete unvalid triangles (intersecting new segments)
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if (p1 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p1 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}

				pointer = nxt;
			}
		}

		// clear deleted triangles
		for (int i = 0; i < info.size(); i++)
		{
			info[i].clearInvalidSegments();
		}

		// check if new triangles are possible
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p1, np2 = input[i].first, np3 = p2;
			int np1i = p1i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p3, np2 = input[i].first, np3 = p2;
			int np1i = p3i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
	}

	vector <Point> _res;
	res.pop_back();
	for (auto it : res)
	{
		_res.push_back(it.first);
	}

	return _res;
}

vector <tuple<Point, Point, Point> > Algorithms::triangulatePolygon(vector <Point> points)
{
	vector <vector <Point> > faces1 = splitBySplitVertex(points), faces2;

	for (auto face : faces1)
	{
		for (auto& p : face)
		{
			p.y = -p.y;
		}

		vector <vector <Point> > subFaces = splitBySplitVertex(face);

		for (auto& subFace : subFaces)
		{
			for (auto& p : subFace)
			{
				p.y = -p.y;
			}

			faces2.push_back(subFace);
		}
	}

	vector < tuple<Point, Point, Point> > triangles;

	for (auto face : faces2)
	{
		vector <tuple<Point, Point, Point> > subTriangles = triangulateYMonotone(face);

		for (auto triangle : subTriangles)
		{
			triangles.push_back(triangle);
		}
	}

	return triangles;
}

vector <Point> Algorithms::MAPPermuteReject(vector <Point> points)
{
	points.push_back(points[0]);
	vector <Point> bestPolygon = points;
	double bestArea = 1.0e9;

	long long factorial = 1;
	for (int i = 2; i < points.size() - 1; i++)
	{
		factorial *= (long long)i;
	}

	for (long long f = 0; f < factorial; f++)
	{
		int isOk = 1;

		for (int i = 1; i < points.size(); i++)
		{
			for (int j = i + 2; j < points.size(); j++)
			{
				if (intersect(Segment(points[i - 1], points[i]), Segment(points[j - 1], points[j])))
				{
					isOk = 0;

					break;
				}
			}

			if (isOk == 0)
			{
				break;
			}
		}

		if (isOk == 1)
		{
			double currentArea = abs(area(points));

			if (currentArea < bestArea)
			{
				bestArea = currentArea;
				bestPolygon = points;
			}
		}

		next_permutation(points.begin() + 1, --points.end());
	}

	bestPolygon.pop_back();
	return bestPolygon;
}

vector <Point> Algorithms::DACMergePolygons(vector <Point> poly1, vector <Point> poly2)
{
	int n1 = poly1.size(), n2 = poly2.size();
	poly1.push_back(poly1[0]);
	poly2.push_back(poly2[0]);

	int bestI1 = -1, bestI2 = -1, bestJ1 = -1, bestJ2 = -1; // I1 -> I2 -> J2 -> J1 -> I1
	double minQArea = 1.0e9; // minQuadrilateralArea

	vector <int> prevVis, curVis;
	// check intersections for 0
	for (int j = 0; j < poly2.size(); j++)
	{
		int isOk = 1;
		Segment s = Segment(poly1[0], poly2[j]);

		for (int ii = 1; ii < poly1.size(); ii++)
		{
			if (poly1[ii] == poly1[0] || poly1[ii - 1] == poly1[0])
			{
				continue;
			}

			if (intersect(s, Segment(poly1[ii - 1], poly1[ii])))
			{
				isOk = 0;

				break;
			}
		}

		if (isOk == 0)
		{
			prevVis.push_back(0);

			continue;
		}

		for (int jj = 1; jj < poly2.size(); jj++)
		{
			if (poly2[jj] == poly2[j] || poly2[jj - 1] == poly2[j])
			{
				continue;
			}

			if (intersect(s, Segment(poly2[jj - 1], poly2[jj])))
			{
				isOk = 0;

				break;
			}
		}

		prevVis.push_back(isOk);
	}

	for (int i = 1; i < poly1.size(); i++)
	{
		// calc visibility
		for (int j = 0; j < poly2.size(); j++)
		{
			int isOk = 1;
			Segment s = Segment(poly1[i], poly2[j]);

			for (int ii = 1; ii < poly1.size(); ii++)
			{
				if (poly1[ii] == poly1[i] || poly1[ii - 1] == poly1[i])
				{
					continue;
				}

				if (intersect(s, Segment(poly1[ii - 1], poly1[ii])))
				{
					isOk = 0;

					break;
				}
			}

			if (isOk == 0)
			{
				curVis.push_back(0);

				continue;
			}

			for (int jj = 1; jj < poly2.size(); jj++)
			{
				if (poly2[jj] == poly2[j] || poly2[jj - 1] == poly2[j])
				{
					continue;
				}

				if (intersect(s, Segment(poly2[jj - 1], poly2[jj])))
				{
					isOk = 0;

					break;
				}
			}

			curVis.push_back(isOk);
		}

		// calc possible quads
		for (int k = 1; k < prevVis.size(); k++)
		{
			if (prevVis[k - 1] == 1 && curVis[k] == 1)
			{
				if (!intersect(Segment(poly1[i - 1], poly2[k - 1]), Segment(poly1[i], poly2[k])))
				{
					vector <Point> quad;
					quad.push_back(poly1[i - 1]);
					quad.push_back(poly1[i]);
					quad.push_back(poly2[k]);
					quad.push_back(poly2[k - 1]);
					double QArea = abs(area(quad));

					if (QArea < minQArea)
					{
						minQArea = QArea;
						bestI1 = i - 1;
						bestI2 = i;
						bestJ2 = k;
						bestJ1 = k - 1;
					}
				}
			}

			if (prevVis[k] == 1 && curVis[k - 1] == 1)
			{
				if (!intersect(Segment(poly1[i - 1], poly2[k]), Segment(poly1[i], poly2[k - 1])))
				{
					vector <Point> quad;
					quad.push_back(poly1[i - 1]);
					quad.push_back(poly1[i]);
					quad.push_back(poly2[k - 1]);
					quad.push_back(poly2[k]);
					double QArea = abs(area(quad));

					if (QArea < minQArea)
					{
						minQArea = QArea;
						bestI1 = i - 1;
						bestI2 = i;
						bestJ2 = k - 1;
						bestJ1 = k;
					}
				}
			}
		}

		prevVis = curVis;
		curVis.clear();
	}

	if (bestI1 == -1)
	{
		return vector <Point>();
	}

	// merge polygons based on best quad
	vector <Point> result;
	for (int i = 0; i < n1; i++)
	{
		result.push_back(poly1[(bestI1 - i + n1) % n1]);
	}
	if (bestJ2 < bestJ1)
	{
		for (int i = 0; i < n2; i++)
		{
			result.push_back(poly2[(bestJ2 - i + n2) % n2]);
		}
	}
	else
	{
		for (int i = 0; i < n2; i++)
		{
			result.push_back(poly2[(bestJ2 + i) % n2]);
		}
	}

	return result;
}

vector <Point> Algorithms::MAPDACPrivate(vector <Point> points)
{
	if (points.size() < 6)
	{
		return MAPPermuteReject(points);
	}
	int n = points.size();

	vector <Point> points1, points2;
	for (int i = 0; i < points.size(); i++)
	{
		if (i < (points.size() + 1) / 2)
		{
			points1.push_back(points[i]);
		}
		else
		{
			points2.push_back(points[i]);
		}
	}

	vector <Point> poly1 = MAPDACPrivate(points1), poly2 = MAPDACPrivate(points2);

	// combine 2 polygons
	vector <Point> result = DACMergePolygons(poly1, poly2);

	return result;
}

vector <Point> Algorithms::MAPDAC(vector <Point> points)
{
	sort(points.begin(), points.end());

	return MAPDACPrivate(points);
}

pair<vector <Point>, vector <Point> > Algorithms::MAPDAC2Private(vector <Point> points)
{
	if (points.size() < 6)
	{
		vector <Point> result = MAPPermuteReject(points);

		return { result, result };
	}

	pair<double, double> areas = { 1.0e9, 1.0e9 };
	pair<vector <Point>, vector <Point> > result; // {vertical, horizontal}
	int n = points.size();

	{// vertical
		sort(points.begin(), points.end());

		vector <Point> points1, points2;
		for (int i = 0; i < points.size(); i++)
		{
			if (i < (points.size() + 1) / 2)
			{
				points1.push_back(points[i]);
			}
			else
			{
				points2.push_back(points[i]);
			}
		}

		auto solLeft = MAPDAC2Private(points1), solRight = MAPDAC2Private(points2);

		vector <vector <Point> > results;
		results.push_back(DACMergePolygons(solLeft.first, solRight.first));
		results.push_back(DACMergePolygons(solLeft.first, solRight.second));
		results.push_back(DACMergePolygons(solLeft.second, solRight.first));
		results.push_back(DACMergePolygons(solLeft.second, solRight.second));

		for (auto& poly : results)
		{
			if (poly.size() == 0)
			{
				continue;
			}

			double polyArea = abs(area(poly));

			if (polyArea < areas.first)
			{
				areas.first = polyArea;
				result.first = poly;
			}
		}
	}

	{// horizontal
		for (auto& it : points)
		{
			swap(it.x, it.y);
		}
		sort(points.begin(), points.end());
		for (auto& it : points)
		{
			swap(it.x, it.y);
		}

		vector <Point> points1, points2;
		for (int i = 0; i < points.size(); i++)
		{
			if (i < (points.size() + 1) / 2)
			{
				points1.push_back(points[i]);
			}
			else
			{
				points2.push_back(points[i]);
			}
		}

		auto solDown = MAPDAC2Private(points1), solUp = MAPDAC2Private(points2);

		vector <vector <Point> > results;
		results.push_back(DACMergePolygons(solDown.first, solUp.first));
		results.push_back(DACMergePolygons(solDown.first, solUp.second));
		results.push_back(DACMergePolygons(solDown.second, solUp.first));
		results.push_back(DACMergePolygons(solDown.second, solUp.second));

		for (auto& poly : results)
		{
			if (poly.size() == 0)
			{
				continue;
			}

			double polyArea = abs(area(poly));

			if (polyArea < areas.second)
			{
				areas.second = polyArea;
				result.second = poly;
			}
		}
	}

	return result;
}

vector <Point> Algorithms::MAPDAC2(vector <Point> points)
{
	pair<vector <Point>, vector <Point> > result = MAPDAC2Private(points);

	double area1 = abs(area(result.first)), area2 = abs(area(result.second));

	if (area1 < area2)
	{
		return result.first;
	}
	else
	{
		return result.second;
	}
}

vector <Point> Algorithms::MAP_RANDPrivate(vector <Point> points, mt19937& rng)
{
	vector <Point> pointsCopy = points;

	vector <Point> poly;

	{// initial triangle
		int i;

		i = rng() % points.size();
		poly.push_back(points[i]);
		points.erase(points.begin() + i);

		i = rng() % points.size();
		poly.push_back(points[i]);
		points.erase(points.begin() + i);

		i = rng() % points.size();
		poly.push_back(points[i]);
		points.erase(points.begin() + i);
	}

	while (points.size() > 0)
	{
		int index = rng() % points.size();
		Point p = points[index];
		points.erase(points.begin() + index);

		vector <int> visible;
		int polyN = poly.size();

		poly.push_back(poly[0]);
		for (int i = 0; i < polyN; i++)
		{
			Segment s(p, poly[i]);

			int isOk = 1;

			for (int j = 1; j <= polyN; j++)
			{
				if (poly[j - 1] != s.p2 && poly[j] != s.p2)
				{
					if (intersect(s, Segment(poly[j - 1], poly[j])))
					{
						isOk = 0;

						break;
					}
				}
			}

			visible.push_back(isOk);
		}
		visible.push_back(visible[0]);

		if (isInside(poly, p))
		{
			int bestTriangleI = -1;
			double maxArea = 0.0;

			for (int i = 1; i < poly.size(); i++)
			{
				if (visible[i - 1] == 1 && visible[i] == 1)
				{
					int okTriangle = 1;
					for (int j = 0; j < polyN; j++)
					{
						if (poly[j] != poly[i - 1] && poly[j] != poly[i])
						{
							if (isInside(poly[i - 1], poly[i], p, poly[j]))
							{
								okTriangle = 0;

								break;
							}
						}
					}

					if (okTriangle == 1)
					{
						double myArea = area(p, poly[i - 1], poly[i]);

						if (myArea > maxArea)
						{
							bestTriangleI = i;
							maxArea = myArea;
						}
					}
				}
			}

			if (bestTriangleI == -1)
			{
				return MAP_RANDPrivate(pointsCopy, rng);
			}
			else
			{
				poly.insert(poly.begin() + bestTriangleI, p);
			}
		}
		else
		{
			int bestTriangleI = -1;
			double minArea = 1.0e9;

			for (int i = 1; i < poly.size(); i++)
			{
				if (visible[i - 1] == 1 && visible[i] == 1)
				{
					int okTriangle = 1;
					for (int j = 0; j < polyN; j++)
					{
						if (poly[j] != poly[i - 1] && poly[j] != poly[i])
						{
							if (isInside(poly[i - 1], poly[i], p, poly[j]))
							{
								okTriangle = 0;

								break;
							}
						}
					}

					if (okTriangle == 1)
					{
						double myArea = area(p, poly[i - 1], poly[i]);

						if (myArea < minArea)
						{
							bestTriangleI = i;
							minArea = myArea;
						}
					}
				}
			}

			if (bestTriangleI == -1)
			{
				return MAP_RANDPrivate(pointsCopy, rng);
			}
			else
			{
				poly.insert(poly.begin() + bestTriangleI, p);
			}
		}

		poly.pop_back();
	}

	return poly;
}

vector <Point> Algorithms::MAP_RAND(vector <Point> points, int q)
{
	//mt19937 rng(time(0));
	mt19937 rng(47);

	vector <Point> bestPolygon;
	double bestArea = 1.0e9;

	//cout << "iter i =";
	for (int i = 0; i < q; i++)
	{
		vector <Point> poly = MAP_RANDPrivate(points, rng);
		double polyArea = abs(area(poly));

		if (polyArea < bestArea)
		{
			bestArea = polyArea;
			bestPolygon = poly;
		}

		//cout << " " << i;
	}
	//cout << "\n\n";

	return bestPolygon;
}

vector <Point> Algorithms::MAP_RSA1(vector <Point> points, mt19937& rng)
{
	vector <pair<Point, int> > input; // {point, index}
	vector <vector <int> > stripes = precalcPointsUnderStripe(points);
	for (int i = 0; i < points.size(); i++)
	{
		input.push_back({ points[i], i });
	}

	vector <pair<Point, int> > res;

	{// build initial triangle
		int pos1;

		pos1 = rng() % input.size();
		res.push_back(input[pos1]);
		input.erase(input.begin() + pos1);

		Point p = res[0].first;

		vector <pair<double, int> > inf;
		for (int i = 0; i < input.size(); i++)
		{
			double _dist = dist(p, input[i].first);
			inf.push_back({ _dist, i });
		}

		sort(inf.begin(), inf.end());

		res.push_back(input[inf[0].second]);
		res.push_back(input[inf[1].second]);

		input.erase(input.begin() + max(inf[0].second, inf[1].second));
		input.erase(input.begin() + min(inf[0].second, inf[1].second));
	}
	res.push_back(res[0]);

	// start building possible triangles
	vector <SegmentLinkedList> info(input.size());
	for (int i = 0; i < input.size(); i++)
	{
		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[0].second, res[1].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[1].first, res[2].first)) &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[0].first, res[2].first)))
			{
				info[i].add(Segment(res[0].first, res[1].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[1].second, res[2].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[2].first, res[0].first)) &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[1].first, res[0].first)))
			{
				info[i].add(Segment(res[1].first, res[2].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[2].second, res[0].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[0].first, res[1].first)) &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[2].first, res[1].first)))
			{
				info[i].add(Segment(res[2].first, res[0].first));
			}
		}
	}

	while (input.size() > 0)
	{
		double bestArea = 1.0e9;
		double minDist = 1.0e9;
		int bestPointI = -1;
		Point bestPoint;
		Segment bestSegment;

		// find best edge
		vector <int> feasiblePoints;
		for (int i = 0; i < input.size(); i++)
		{
			if (info[i].last != -1)
			{
				double myDist = 1.0e9;

				for (int j = 1; j < res.size(); j++)
				{
					myDist = min(myDist, distanceToSegment(Segment(res[j - 1].first, res[j].first), input[i].first));
				}

				if (myDist < minDist)
				{
					bestPointI = i;
				}
			}
		}

		if (bestPointI == -1)
		{
			return MAP_RSA1(points, rng);
		}

		{
			int i = bestPointI;
			int pointer = info[i].last;
			while (pointer != -1)
			{
				double tArea = area(points[input[i].second], info[i].m[pointer].first.p1, info[i].m[pointer].first.p2);

				if (tArea < bestArea)
				{
					bestArea = tArea;
					bestPointI = i;
					bestPoint = points[input[i].second];
					bestSegment = info[i].m[pointer].first;
				}

				pointer = info[i].m[pointer].second.first;
			}
		}

		if (bestArea > 1.0e8)
		{
			return MAP_RSA1(points, rng);
		}

		Point p1 = bestSegment.p1, p2 = bestPoint, p3 = bestSegment.p2;
		int p1i, p2i = input[bestPointI].second, p3i;

		// add new point to res
		for (int i = 0; i < res.size() - 1; i++)
		{
			if (res[i].first == p1 && res[i + 1].first == p3)
			{
				p1i = res[i].second;
				p3i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}

			if (res[i].first == p3 && res[i + 1].first == p1)
			{
				p3i = res[i].second;
				p1i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}
		}

		// erase triangles with deleted segment
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if ((info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
					(info[i].m[pointer].first.p1 == p3 && info[i].m[pointer].first.p2 == p1))
				{
					info[i].erase(pointer);
				}

				pointer = nxt;
			}
		}

		// delete point
		input.erase(input.begin() + bestPointI);
		info.erase(info.begin() + bestPointI);

		// delete unvalid triangles (intersecting new segments)
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if (p1 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p1 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}

				pointer = nxt;
			}
		}

		// clear deleted triangles
		for (int i = 0; i < info.size(); i++)
		{
			info[i].clearInvalidSegments();
		}

		// check if new triangles are possible
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p1, np2 = input[i].first, np3 = p2;
			int np1i = p1i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p3, np2 = input[i].first, np3 = p2;
			int np1i = p3i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
	}

	vector <Point> _res;
	res.pop_back();
	for (auto it : res)
	{
		_res.push_back(it.first);
	}

	return _res;
}

vector <Point> Algorithms::MAP_RSA2(vector <Point> points, mt19937& rng)
{
	vector <pair<Point, int> > input; // {point, index}
	vector <vector <int> > stripes = precalcPointsUnderStripe(points);
	for (int i = 0; i < points.size(); i++)
	{
		input.push_back({ points[i], i });
	}

	vector <pair<Point, int> > res;

	{// build initial triangle
		int pos1;

		pos1 = rng() % input.size();
		res.push_back(input[pos1]);
		input.erase(input.begin() + pos1);

		Point p = res[0].first;

		vector <pair<double, int> > inf;
		for (int i = 0; i < input.size(); i++)
		{
			double _dist = dist(p, input[i].first);
			inf.push_back({ _dist, i });
		}

		sort(inf.begin(), inf.end());

		res.push_back(input[inf[0].second]);
		res.push_back(input[inf[1].second]);

		input.erase(input.begin() + max(inf[0].second, inf[1].second));
		input.erase(input.begin() + min(inf[0].second, inf[1].second));
	}
	res.push_back(res[0]);

	// start building possible triangles
	vector <SegmentLinkedList> info(input.size());
	for (int i = 0; i < input.size(); i++)
	{
		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[0].second, res[1].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[1].first, res[2].first)) &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[0].first, res[2].first)))
			{
				info[i].add(Segment(res[0].first, res[1].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[1].second, res[2].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[2].first, res[0].first)) &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[1].first, res[0].first)))
			{
				info[i].add(Segment(res[1].first, res[2].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[2].second, res[0].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[0].first, res[1].first)) &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[2].first, res[1].first)))
			{
				info[i].add(Segment(res[2].first, res[0].first));
			}
		}
	}

	while (input.size() > 0)
	{
		double bestArea = 1.0e9;
		int bestPointI = 0;
		Point bestPoint;
		Segment bestSegment;

		// find best edge
		vector <int> feasiblePoints;
		for (int i = 0; i < input.size(); i++)
		{
			if (info[i].last != -1)
			{
				feasiblePoints.push_back(i);
			}
		}

		if (feasiblePoints.size() == 0)
		{
			return MAP_RSA2(points, rng);
		}

		{
			int i = rng() % feasiblePoints.size();
			i = feasiblePoints[i];
			int pointer = info[i].last;
			while (pointer != -1)
			{
				double tArea = area(points[input[i].second], info[i].m[pointer].first.p1, info[i].m[pointer].first.p2);

				if (tArea < bestArea)
				{
					bestArea = tArea;
					bestPointI = i;
					bestPoint = points[input[i].second];
					bestSegment = info[i].m[pointer].first;
				}

				pointer = info[i].m[pointer].second.first;
			}
		}

		if (bestArea > 1.0e8)
		{
			return MAP_RSA2(points, rng);
		}

		Point p1 = bestSegment.p1, p2 = bestPoint, p3 = bestSegment.p2;
		int p1i, p2i = input[bestPointI].second, p3i;

		// add new point to res
		for (int i = 0; i < res.size() - 1; i++)
		{
			if (res[i].first == p1 && res[i + 1].first == p3)
			{
				p1i = res[i].second;
				p3i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}

			if (res[i].first == p3 && res[i + 1].first == p1)
			{
				p3i = res[i].second;
				p1i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}
		}

		// erase triangles with deleted segment
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if ((info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
					(info[i].m[pointer].first.p1 == p3 && info[i].m[pointer].first.p2 == p1))
				{
					info[i].erase(pointer);
				}

				pointer = nxt;
			}
		}

		// delete point
		input.erase(input.begin() + bestPointI);
		info.erase(info.begin() + bestPointI);

		// delete unvalid triangles (intersecting new segments)
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if (p1 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p1 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}

				pointer = nxt;
			}
		}

		// clear deleted triangles
		for (int i = 0; i < info.size(); i++)
		{
			info[i].clearInvalidSegments();
		}

		// check if new triangles are possible
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p1, np2 = input[i].first, np3 = p2;
			int np1i = p1i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p3, np2 = input[i].first, np3 = p2;
			int np1i = p3i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
	}

	vector <Point> _res;
	res.pop_back();
	for (auto it : res)
	{
		_res.push_back(it.first);
	}

	return _res;
}

vector <Point> Algorithms::MAP_RSA3(vector <Point> points, mt19937& rng)
{
	vector <pair<Point, int> > input; // {point, index}
	vector <vector <int> > stripes = precalcPointsUnderStripe(points);
	for (int i = 0; i < points.size(); i++)
	{
		input.push_back({ points[i], i });
	}

	vector <pair<Point, int> > res;

	{// build initial triangle
		int pos1;

		pos1 = rng() % input.size();
		res.push_back(input[pos1]);
		input.erase(input.begin() + pos1);

		Point p = res[0].first;

		vector <pair<double, int> > inf;
		for (int i = 0; i < input.size(); i++)
		{
			double _dist = dist(p, input[i].first);
			inf.push_back({ _dist, i });
		}

		sort(inf.begin(), inf.end());

		res.push_back(input[inf[0].second]);
		res.push_back(input[inf[1].second]);

		input.erase(input.begin() + max(inf[0].second, inf[1].second));
		input.erase(input.begin() + min(inf[0].second, inf[1].second));
	}
	res.push_back(res[0]);

	// start building possible triangles
	vector <SegmentLinkedList> info(input.size());
	for (int i = 0; i < input.size(); i++)
	{
		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[0].second, res[1].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[1].first, res[2].first)) &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[0].first, res[2].first)))
			{
				info[i].add(Segment(res[0].first, res[1].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[1].second, res[2].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[2].first, res[0].first)) &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[1].first, res[0].first)))
			{
				info[i].add(Segment(res[1].first, res[2].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[2].second, res[0].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[0].first, res[1].first)) &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[2].first, res[1].first)))
			{
				info[i].add(Segment(res[2].first, res[0].first));
			}
		}
	}

	while (input.size() > 0)
	{
		double bestArea = 1.0e9;
		int bestPointI = 0;
		Point bestPoint;
		Segment bestSegment;

		// find best point
		for (int i = 0; i < input.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				double tArea = area(points[input[i].second], info[i].m[pointer].first.p1, info[i].m[pointer].first.p2);

				if (tArea < bestArea)
				{
					bestArea = tArea;
					bestPointI = i;
					bestPoint = points[input[i].second];
					bestSegment = info[i].m[pointer].first;
				}

				pointer = info[i].m[pointer].second.first;
			}
		}

		if (bestArea > 1.0e8)
		{
			return MAP_RSA3(points, rng);
		}

		Point p1 = bestSegment.p1, p2 = bestPoint, p3 = bestSegment.p2;
		int p1i, p2i = input[bestPointI].second, p3i;

		// add new point to res
		for (int i = 0; i < res.size() - 1; i++)
		{
			if (res[i].first == p1 && res[i + 1].first == p3)
			{
				p1i = res[i].second;
				p3i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}

			if (res[i].first == p3 && res[i + 1].first == p1)
			{
				p3i = res[i].second;
				p1i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}
		}

		// erase triangles with deleted segment
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if ((info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
					(info[i].m[pointer].first.p1 == p3 && info[i].m[pointer].first.p2 == p1))
				{
					info[i].erase(pointer);
				}

				pointer = nxt;
			}
		}

		// delete point
		input.erase(input.begin() + bestPointI);
		info.erase(info.begin() + bestPointI);

		// delete unvalid triangles (intersecting new segments)
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if (p1 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p1 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}

				pointer = nxt;
			}
		}

		// clear deleted triangles
		for (int i = 0; i < info.size(); i++)
		{
			info[i].clearInvalidSegments();
		}

		// check if new triangles are possible
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p1, np2 = input[i].first, np3 = p2;
			int np1i = p1i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p3, np2 = input[i].first, np3 = p2;
			int np1i = p3i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
	}

	vector <Point> _res;
	res.pop_back();
	for (auto it : res)
	{
		_res.push_back(it.first);
	}

	return _res;
}

vector <Point> Algorithms::MAP_RSA4(vector <Point> points, mt19937& rng)
{
	vector <pair<Point, int> > input; // {point, index}
	vector <vector <int> > stripes = precalcPointsUnderStripe(points);
	for (int i = 0; i < points.size(); i++)
	{
		input.push_back({ points[i], i });
	}

	vector <pair<Point, int> > res;

	{// build initial triangle
		int pos;

		pos = rng() % input.size();
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);

		pos = rng() % input.size();
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);

		vector <int> okPoints;

		for (int i = 0; i < input.size(); i++)
		{
			if (pointsInsideTriangle(stripes, points, res[0].second, res[1].second, input[i].second) == 0)
			{
				okPoints.push_back(i);
			}
		}
		pos = rng() % okPoints.size();
		pos = okPoints[pos];
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);
	}
	res.push_back(res[0]);

	// start building possible triangles
	vector <SegmentLinkedList> info(input.size());
	for (int i = 0; i < input.size(); i++)
	{
		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[0].second, res[1].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[1].first, res[2].first)) &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[0].first, res[2].first)))
			{
				info[i].add(Segment(res[0].first, res[1].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[1].second, res[2].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[2].first, res[0].first)) &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[1].first, res[0].first)))
			{
				info[i].add(Segment(res[1].first, res[2].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[2].second, res[0].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[0].first, res[1].first)) &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[2].first, res[1].first)))
			{
				info[i].add(Segment(res[2].first, res[0].first));
			}
		}
	}

	while (input.size() > 0)
	{
		double bestArea = 1.0e9;
		double minDist = 1.0e9;
		int bestPointI = -1;
		Point bestPoint;
		Segment bestSegment;

		// find best edge
		vector <int> feasiblePoints;
		for (int i = 0; i < input.size(); i++)
		{
			if (info[i].last != -1)
			{
				double myDist = 1.0e9;

				for (int j = 1; j < res.size(); j++)
				{
					myDist = min(myDist, distanceToSegment(Segment(res[j - 1].first, res[j].first), input[i].first));
				}

				if (myDist < minDist)
				{
					bestPointI = i;
				}
			}
		}

		if (bestPointI == -1)
		{
			return MAP_RSA4(points, rng);
		}

		{
			int i = bestPointI;
			int pointer = info[i].last;
			while (pointer != -1)
			{
				double tArea = area(points[input[i].second], info[i].m[pointer].first.p1, info[i].m[pointer].first.p2);

				if (tArea < bestArea)
				{
					bestArea = tArea;
					bestPointI = i;
					bestPoint = points[input[i].second];
					bestSegment = info[i].m[pointer].first;
				}

				pointer = info[i].m[pointer].second.first;
			}
		}

		if (bestArea > 1.0e8)
		{
			return MAP_RSA4(points, rng);
		}

		Point p1 = bestSegment.p1, p2 = bestPoint, p3 = bestSegment.p2;
		int p1i, p2i = input[bestPointI].second, p3i;

		// add new point to res
		for (int i = 0; i < res.size() - 1; i++)
		{
			if (res[i].first == p1 && res[i + 1].first == p3)
			{
				p1i = res[i].second;
				p3i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}

			if (res[i].first == p3 && res[i + 1].first == p1)
			{
				p3i = res[i].second;
				p1i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}
		}

		// erase triangles with deleted segment
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if ((info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
					(info[i].m[pointer].first.p1 == p3 && info[i].m[pointer].first.p2 == p1))
				{
					info[i].erase(pointer);
				}

				pointer = nxt;
			}
		}

		// delete point
		input.erase(input.begin() + bestPointI);
		info.erase(info.begin() + bestPointI);

		// delete unvalid triangles (intersecting new segments)
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if (p1 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p1 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}

				pointer = nxt;
			}
		}

		// clear deleted triangles
		for (int i = 0; i < info.size(); i++)
		{
			info[i].clearInvalidSegments();
		}

		// check if new triangles are possible
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p1, np2 = input[i].first, np3 = p2;
			int np1i = p1i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p3, np2 = input[i].first, np3 = p2;
			int np1i = p3i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
	}

	vector <Point> _res;
	res.pop_back();
	for (auto it : res)
	{
		_res.push_back(it.first);
	}

	return _res;
}

vector <Point> Algorithms::MAP_RSA5(vector <Point> points, mt19937& rng)
{
	vector <pair<Point, int> > input; // {point, index}
	vector <vector <int> > stripes = precalcPointsUnderStripe(points);
	for (int i = 0; i < points.size(); i++)
	{
		input.push_back({ points[i], i });
	}

	vector <pair<Point, int> > res;

	{// build initial triangle
		int pos;

		pos = rng() % input.size();
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);

		pos = rng() % input.size();
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);

		vector <int> okPoints;

		for (int i = 0; i < input.size(); i++)
		{
			if (pointsInsideTriangle(stripes, points, res[0].second, res[1].second, input[i].second) == 0)
			{
				okPoints.push_back(i);
			}
		}
		pos = rng() % okPoints.size();
		pos = okPoints[pos];
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);
	}
	res.push_back(res[0]);

	// start building possible triangles
	vector <SegmentLinkedList> info(input.size());
	for (int i = 0; i < input.size(); i++)
	{
		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[0].second, res[1].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[1].first, res[2].first)) &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[0].first, res[2].first)))
			{
				info[i].add(Segment(res[0].first, res[1].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[1].second, res[2].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[2].first, res[0].first)) &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[1].first, res[0].first)))
			{
				info[i].add(Segment(res[1].first, res[2].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[2].second, res[0].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[0].first, res[1].first)) &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[2].first, res[1].first)))
			{
				info[i].add(Segment(res[2].first, res[0].first));
			}
		}
	}

	while (input.size() > 0)
	{
		double bestArea = 1.0e9;
		int bestPointI = 0;
		Point bestPoint;
		Segment bestSegment;

		// find best edge
		vector <int> feasiblePoints;
		for (int i = 0; i < input.size(); i++)
		{
			if (info[i].last != -1)
			{
				feasiblePoints.push_back(i);
			}
		}

		if (feasiblePoints.size() == 0)
		{
			return MAP_RSA5(points, rng);
		}

		{
			int i = rng() % feasiblePoints.size();
			i = feasiblePoints[i];
			int pointer = info[i].last;
			while (pointer != -1)
			{
				double tArea = area(points[input[i].second], info[i].m[pointer].first.p1, info[i].m[pointer].first.p2);

				if (tArea < bestArea)
				{
					bestArea = tArea;
					bestPointI = i;
					bestPoint = points[input[i].second];
					bestSegment = info[i].m[pointer].first;
				}

				pointer = info[i].m[pointer].second.first;
			}
		}

		if (bestArea > 1.0e8)
		{
			return MAP_RSA5(points, rng);
		}

		Point p1 = bestSegment.p1, p2 = bestPoint, p3 = bestSegment.p2;
		int p1i, p2i = input[bestPointI].second, p3i;

		// add new point to res
		for (int i = 0; i < res.size() - 1; i++)
		{
			if (res[i].first == p1 && res[i + 1].first == p3)
			{
				p1i = res[i].second;
				p3i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}

			if (res[i].first == p3 && res[i + 1].first == p1)
			{
				p3i = res[i].second;
				p1i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}
		}

		// erase triangles with deleted segment
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if ((info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
					(info[i].m[pointer].first.p1 == p3 && info[i].m[pointer].first.p2 == p1))
				{
					info[i].erase(pointer);
				}

				pointer = nxt;
			}
		}

		// delete point
		input.erase(input.begin() + bestPointI);
		info.erase(info.begin() + bestPointI);

		// delete unvalid triangles (intersecting new segments)
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if (p1 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p1 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}

				pointer = nxt;
			}
		}

		// clear deleted triangles
		for (int i = 0; i < info.size(); i++)
		{
			info[i].clearInvalidSegments();
		}

		// check if new triangles are possible
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p1, np2 = input[i].first, np3 = p2;
			int np1i = p1i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p3, np2 = input[i].first, np3 = p2;
			int np1i = p3i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
	}

	vector <Point> _res;
	res.pop_back();
	for (auto it : res)
	{
		_res.push_back(it.first);
	}

	return _res;
}

vector <Point> Algorithms::MAP_RSA6(vector <Point> points, mt19937& rng)
{
	vector <pair<Point, int> > input; // {point, index}
	vector <vector <int> > stripes = precalcPointsUnderStripe(points);
	for (int i = 0; i < points.size(); i++)
	{
		input.push_back({ points[i], i });
	}

	vector <pair<Point, int> > res;

	{// build initial triangle
		int pos;

		pos = rng() % input.size();
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);

		pos = rng() % input.size();
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);

		vector <int> okPoints;

		for (int i = 0; i < input.size(); i++)
		{
			if (pointsInsideTriangle(stripes, points, res[0].second, res[1].second, input[i].second) == 0)
			{
				okPoints.push_back(i);
			}
		}
		pos = rng() % okPoints.size();
		pos = okPoints[pos];
		res.push_back(input[pos]);
		input.erase(input.begin() + pos);
	}
	res.push_back(res[0]);

	// start building possible triangles
	vector <SegmentLinkedList> info(input.size());
	for (int i = 0; i < input.size(); i++)
	{
		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[0].second, res[1].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[1].first, res[2].first)) &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[0].first, res[2].first)))
			{
				info[i].add(Segment(res[0].first, res[1].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[1].second, res[2].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[1].first), Segment(res[2].first, res[0].first)) &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[1].first, res[0].first)))
			{
				info[i].add(Segment(res[1].first, res[2].first));
			}
		}

		{
			int pointsInside = pointsInsideTriangle(stripes, points, input[i].second, res[2].second, res[0].second);

			if (pointsInside == 0 &&
				!intersect(Segment(input[i].first, res[2].first), Segment(res[0].first, res[1].first)) &&
				!intersect(Segment(input[i].first, res[0].first), Segment(res[2].first, res[1].first)))
			{
				info[i].add(Segment(res[2].first, res[0].first));
			}
		}
	}

	while (input.size() > 0)
	{
		double bestArea = 1.0e9;
		int bestPointI = 0;
		Point bestPoint;
		Segment bestSegment;

		// find best point
		for (int i = 0; i < input.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				double tArea = area(points[input[i].second], info[i].m[pointer].first.p1, info[i].m[pointer].first.p2);

				if (tArea < bestArea)
				{
					bestArea = tArea;
					bestPointI = i;
					bestPoint = points[input[i].second];
					bestSegment = info[i].m[pointer].first;
				}

				pointer = info[i].m[pointer].second.first;
			}
		}

		if (bestArea > 1.0e8)
		{
			return MAP_RSA6(points, rng);
		}

		Point p1 = bestSegment.p1, p2 = bestPoint, p3 = bestSegment.p2;
		int p1i, p2i = input[bestPointI].second, p3i;

		// add new point to res
		for (int i = 0; i < res.size() - 1; i++)
		{
			if (res[i].first == p1 && res[i + 1].first == p3)
			{
				p1i = res[i].second;
				p3i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}

			if (res[i].first == p3 && res[i + 1].first == p1)
			{
				p3i = res[i].second;
				p1i = res[i + 1].second;
				res.insert(res.begin() + i + 1, { bestPoint, p2i });

				break;
			}
		}

		// erase triangles with deleted segment
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if ((info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
					(info[i].m[pointer].first.p1 == p3 && info[i].m[pointer].first.p2 == p1))
				{
					info[i].erase(pointer);
				}

				pointer = nxt;
			}
		}

		// delete point
		input.erase(input.begin() + bestPointI);
		info.erase(info.begin() + bestPointI);

		// delete unvalid triangles (intersecting new segments)
		for (int i = 0; i < info.size(); i++)
		{
			int pointer = info[i].last;
			while (pointer != -1)
			{
				int nxt = info[i].m[pointer].second.first;

				if (p1 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p1 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p1, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p1 && p2 != info[i].m[pointer].first.p1 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p1), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}
				if (p3 != info[i].m[pointer].first.p2 && p2 != info[i].m[pointer].first.p2 &&
					intersect(Segment(input[i].first, info[i].m[pointer].first.p2), Segment(p3, p2)))
				{
					info[i].erase(pointer);
					pointer = nxt;

					continue;
				}

				pointer = nxt;
			}
		}

		// clear deleted triangles
		for (int i = 0; i < info.size(); i++)
		{
			info[i].clearInvalidSegments();
		}

		// check if new triangles are possible
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p1, np2 = input[i].first, np3 = p2;
			int np1i = p1i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
		for (int i = 0; i < input.size(); i++)
		{
			Point np1 = p3, np2 = input[i].first, np3 = p2;
			int np1i = p3i, np2i = input[i].second, np3i = p2i;

			int good = 1;

			if (pointsInsideTriangle(stripes, points, np1i, np2i, np3i) != 0)
			{
				good = 0;
			}

			if (good != 1)
			{
				continue;
			}

			for (int j = 1; j < res.size(); j++)
			{
				if (np1 != res[j - 1].first && np1 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np1)) == true)
					{
						good = 0;

						break;
					}
				}
				if (np3 != res[j - 1].first && np3 != res[j].first)
				{
					if (intersect(Segment(res[j - 1].first, res[j].first), Segment(np2, np3)) == true)
					{
						good = 0;

						break;
					}
				}
			}

			if (good == 1)
			{
				info[i].add(Segment(np1, np3));
			}
		}
	}

	vector <Point> _res;
	res.pop_back();
	for (auto it : res)
	{
		_res.push_back(it.first);
	}

	return _res;
}

vector <Point> Algorithms::MAP_RS(vector <Point> points, int q)
{
	//mt19937 rng(time(0));
	mt19937 rng(47);

	vector <Point> bestPolygon;
	double bestArea = 1.0e9;

	//cout << "iter i =";
	for (int i = 0; i < q; i++)
	{
		vector <vector <Point> > polys;
		vector <double> areas;

		polys.push_back(MAP_RSA1(points, rng));
		areas.push_back(abs(area(polys.back())));

		polys.push_back(MAP_RSA2(points, rng));
		areas.push_back(abs(area(polys.back())));

		polys.push_back(MAP_RSA3(points, rng));
		areas.push_back(abs(area(polys.back())));

		polys.push_back(MAP_RSA4(points, rng));
		areas.push_back(abs(area(polys.back())));

		polys.push_back(MAP_RSA5(points, rng));
		areas.push_back(abs(area(polys.back())));

		polys.push_back(MAP_RSA6(points, rng));
		areas.push_back(abs(area(polys.back())));

		for (int j = 0; j < polys.size(); j++)
		{
			if (polys[j].size() != 0)
			{
				if (areas[j] < bestArea)
				{
					bestArea = areas[j];
					bestPolygon = polys[j];
				}
			}
		}

		//cout << " " << i;
	}
	//cout << "\n\n";

	return bestPolygon;
}

vector <Point> Algorithms::MAP_Postprocess(vector <Point> points)
{
	points.push_back(points[0]);

	int iterations = 0;

	while (true)
	{
		iterations++;
		int wasImproved = 0;

		for (int pi = 1; pi + 1 < points.size(); pi++)
		{
			if (wasImproved == 1)// delete
			{
				break;
			}

			Point p = points[pi], p1 = points[pi - 1], p2 = points[pi + 1];

			int canDelete = 1;

			for (int j = 0; j < points.size(); j++)
			{
				if (points[j] != p && points[j] != p1 && points[j] != p2)
				{
					if (isInside(p, p1, p2, points[j]))
					{
						canDelete = 0;

						break;
					}
				}
			}

			if (canDelete == 0)
			{
				continue;
			}

			for (int j = 1; j < points.size(); j++)
			{
				if (points[j - 1] != p1 && points[j] != p1 && points[j - 1] != p2 && points[j] != p2)
				{
					if (intersect(Segment(p1, p2), Segment(points[j - 1], points[j])))
					{
						canDelete = 0;

						break;
					}
				}
			}

			if (canDelete == 0)
			{
				continue;
			}

			points.pop_back();
			double oldArea = abs(area(points));
			points.push_back(points[0]);

			points.erase(points.begin() + pi);

			vector <int> visible;
			int polyN = points.size() - 1;

			for (int i = 0; i < polyN; i++)
			{
				Segment s(p, points[i]);

				int isOk = 1;

				for (int j = 1; j <= polyN; j++)
				{
					if (points[j - 1] != s.p2 && points[j] != s.p2)
					{
						if (intersect(s, Segment(points[j - 1], points[j])))
						{
							isOk = 0;

							break;
						}
					}
				}

				visible.push_back(isOk);
			}
			visible.push_back(visible[0]);

			if (isInside(points, p))
			{
				int bestTriangleI = -1;
				double maxArea = 0.0;

				for (int i = 1; i < points.size(); i++)
				{
					if (visible[i - 1] == 1 && visible[i] == 1)
					{
						int okTriangle = 1;
						for (int j = 0; j < polyN; j++)
						{
							if (points[j] != points[i - 1] && points[j] != points[i])
							{
								if (isInside(points[i - 1], points[i], p, points[j]))
								{
									okTriangle = 0;

									break;
								}
							}
						}

						if (okTriangle == 1)
						{
							double myArea = area(p, points[i - 1], points[i]);

							if (myArea > maxArea)
							{
								bestTriangleI = i;
								maxArea = myArea;
							}
						}
					}
				}

				points.insert(points.begin() + bestTriangleI, p);
			}
			else
			{
				int bestTriangleI = -1;
				double minArea = 1.0e9;

				for (int i = 1; i < points.size(); i++)
				{
					if (visible[i - 1] == 1 && visible[i] == 1)
					{
						int okTriangle = 1;
						for (int j = 0; j < polyN; j++)
						{
							if (points[j] != points[i - 1] && points[j] != points[i])
							{
								if (isInside(points[i - 1], points[i], p, points[j]))
								{
									okTriangle = 0;

									break;
								}
							}
						}

						if (okTriangle == 1)
						{
							double myArea = area(p, points[i - 1], points[i]);

							if (myArea < minArea)
							{
								bestTriangleI = i;
								minArea = myArea;
							}
						}
					}
				}

				points.insert(points.begin() + bestTriangleI, p);
			}

			points.pop_back();
			if (abs(area(points)) + 1.0e-5 < oldArea)
			{
				wasImproved = 1;
			}
			points.push_back(points[0]);
		}

		if (wasImproved == 0)
		{
			break;
		}

		// delete
		break;
	}

	//cout << iterations << " iterations\n";

	points.pop_back();
	return points;
}