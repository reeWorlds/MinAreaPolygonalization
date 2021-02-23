#include "Executer.h"

void Executer::buildConvexHullForGeneratedData()
{
	string path = "../../../Data/Generated/";
	string pathOut = "../../../Data/ConvexHull/";
	ifstream in(path + "RECORDS.txt");
	string name;

	while (in >> name)
	{
		vector <Point> data;
		Point p;

		ifstream inData(path + name);
		while (inData >> p.x >> p.y)
		{
			data.push_back(p);
		}
		inData.clear();

		vector <Point> CH = Algorithms::convexHull(data);

		ofstream outData(pathOut + name);
		for (auto it : CH)
		{
			outData << it.x << " " << it.y << "\n";
		}
		outData.close();
	}
}

void Executer::testMAPGreedy()
{
	vector <Point> points;

	points.push_back(Point(2.93499, 2.46947));
	points.push_back(Point(5.48902, 4.41105));
	points.push_back(Point(1.21308, 0.04975));
	points.push_back(Point(7.94494, 2.92786));
	points.push_back(Point(0.34659, 9.55576));
	points.push_back(Point(4.44005, 5.01851));
	points.push_back(Point(1.54961, 6.82873));

	vector <Point> MAP = Algorithms::MAPGreedy(points);

	for (auto p : MAP)
	{
		cout << p.x << " " << p.y << "\n";
	}

	/*
	int N = 7;

	mt19937 rng(time(NULL));

	while (true)
	{
		vector <Point> points;

		set<double> usedX, usedY;

		for (int i = 0; i < N; i++)
		{
			while (true)
			{
				double x = rng() % 1000000 / 100000.0;
				double y = rng() % 1000000 / 100000.0;

				if (usedX.find(x) == usedX.end() && usedY.find(y) == usedY.end())
				{
					usedX.insert(x);
					usedY.insert(y);

					points.push_back(Point(x, y));

					break;
				}
			}
		}

		vector <Point> MAP = Algorithms::MAPGreedy(points);

		if (MAP.size() != points.size())
		{
			for (auto p : points)
			{
				cout << p.x << " " << p.y << "\n";
			}
			
			cout << "\n\n";

			for (auto p : MAP)
			{
				cout << p.x << " " << p.y << "\n";
			}

			break;
		}
	}
	*/
}

void Executer::runMapGreedy()
{
	string path = "../../../Data/Generated/";
	string pathOut = "../../../Data/MAPGreedy/";
	ifstream in(path + "RECORDS.txt");
	string name;

	while (in >> name)
	{
		cout << "start " << name << "\n";		

		vector <Point> data;
		Point p;

		ifstream inData(path + name);
		while (inData >> p.x >> p.y)
		{
			data.push_back(p);
		}
		inData.clear();

		vector <Point> polygon = Algorithms::MAPGreedy(data);

		ofstream outData(pathOut + name);
		for (auto it : polygon)
		{
			outData << it.x << " " << it.y << "\n";
		}
		outData.close();
	}
}

void Executer::testTriangulation()
{
	vector <Point> v1, v2;

	v1.push_back(Point(0.0, 3.0));
	v1.push_back(Point(3.0, 0.0));
	v1.push_back(Point(4.0, 2.0));
	v1.push_back(Point(7.0, 1.0));
	v1.push_back(Point(6.0, 5.0));
	v1.push_back(Point(5.0, 4.0));
	v1.push_back(Point(2.0, 6.0));

	auto it = Algorithms::triangulatePolygon(v1);
}