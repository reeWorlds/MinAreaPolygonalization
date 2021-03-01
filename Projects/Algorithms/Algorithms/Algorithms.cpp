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

double Algorithms::area(Point p1, Point p2, Point p3)
{
	return 0.5 * abs((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x));
}

double Algorithms::area(vector <Point> points)
{
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
		cout << "Points repeat------------------------\n";

		return false;
	}

	points.push_back(points[0]);

	for (int i = 0; i < points.size() - 1; i++)
	{
		for (int j = i + 2; j < points.size() - 2; j++)
		{
			if (intersect(Segment(points[i], points[i + 1]), Segment(points[j], points[j + 1])))
			{
				cout << "Segments intersect------------------------\n";

				return false;
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

			grA[i1].insert({ angle, gr[i1].size() });
			gr[i1].push_back({ s.p2, {angle, 0 } });
		}

		{// s.p2
			double angle = atan2(s.p1.y - s.p2.y, s.p1.x - s.p2.x);

			grA[i2].insert({ angle, gr[i2].size() });
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
		double myArea = area(faces[i]);

		if (myArea > maxArea)
		{
			myArea = maxArea;
			maxAreaI = i;
		}
	}
	faces.erase(faces.begin() + maxAreaI);

	return faces;
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
			cout << "Unable to connect all poits\n";

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

			if(res[i].first == p3 && res[i + 1].first == p1)
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

				if ( (info[i].m[pointer].first.p1 == p1 && info[i].m[pointer].first.p2 == p3) ||
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

		// chech if new triangles are possible
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
					if (intersect(Segment(res[j-1].first, res[j].first), Segment(np2, np1)) == true)
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

	for (auto f : faces2)
	{
		for (auto it : f)
		{
			cout << "{ " << it.x << "; " << it.y << "} ";
		}
		cout << "\n";
	}

	// todo triangulate y-monotone polygones

	return vector<tuple<Point, Point, Point> >();
}