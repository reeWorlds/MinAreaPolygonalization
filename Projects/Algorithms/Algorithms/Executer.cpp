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
		inData.close();

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
		inData.close();

		vector <Point> polygon = Algorithms::MAPGreedy(data);

		ofstream outData(pathOut + name);
		for (auto it : polygon)
		{
			outData << it.x << " " << it.y << "\n";
		}
		outData.close();
	}

	in.close();
}

void Executer::testTriangulation()
{
	vector <Point> data;
	Point p;

	ifstream inData("../../../Data/Temp/test4Points.txt");
	while (inData >> p.x >> p.y)
	{
		data.push_back(p);
	}
	inData.close();

	vector <Point> polygon = Algorithms::MAPGreedy(data);

	ofstream outPoly("../../../Data/Temp/test4Polygon.txt");
	for (auto it : polygon)
	{
		outPoly << it.x << " " << it.y << "\n";
	}
	outPoly.close();

	vector <tuple<Point, Point, Point> > triangulation = Algorithms::triangulatePolygon(polygon);

	///*
	ofstream outData("../../../Data/Temp/test4Triangles.txt");
	for (auto triangle : triangulation)
	{
		Point p1, p2, p3;

		tie(p1, p2, p3) = triangle;

		outData << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " " << p3.x << " " << p3.y << "\n";
	}
	outData.close(); // */
}

void Executer::runTriangulation()
{
	//string path = "../../../Data/MAPGreedy/";
	//string pathOut = "../../../Data/Triangulation/Greedy/";
	
	string path = "../../../Data/MAPDAC/";
	string pathOut = "../../../Data/Triangulation/DAC/";
	ifstream in("../../../Data/Generated/RECORDS.txt");
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
		inData.close();

		vector <tuple<Point, Point, Point> > triangulation = Algorithms::triangulatePolygon(data);

		ofstream outData(pathOut + name);
		for (auto triangle : triangulation)
		{
			Point p1, p2, p3;

			tie(p1, p2, p3) = triangle;

			outData << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " " << p3.x << " " << p3.y << "\n";
		}
		outData.close();
	}

	in.close();
}

void Executer::testDAC_Perm()
{
	vector <Point> data;
	Point p;

	ifstream inData("../../../Data/Temp/test5Points.txt");
	while (inData >> p.x >> p.y)
	{
		data.push_back(p);
	}
	inData.close();

	//vector <Point> polygon = Algorithms::MAPPermuteReject(data);
	vector <Point> polygon = Algorithms::MAPDAC(data);

	ofstream outPoly("../../../Data/Temp/test5Polygon.txt");
	for (auto it : polygon)
	{
		outPoly << it.x << " " << it.y << "\n";
	}
	outPoly.close();

	//vector <tuple<Point, Point, Point> > triangulation = Algorithms::triangulatePolygon(polygon);

	/*
	ofstream outData("../../../Data/Temp/test4Triangles.txt");
	for (auto triangle : triangulation)
	{
		Point p1, p2, p3;

		tie(p1, p2, p3) = triangle;

		outData << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " " << p3.x << " " << p3.y << "\n";
	}
	outData.close(); // */
}

void Executer::runMAPDAC()
{
	string path = "../../../Data/Generated/";
	string pathOut = "../../../Data/MAPDAC/";
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
		inData.close();

		vector <Point> polygon = Algorithms::MAPDAC(data);

		ofstream outData(pathOut + name);
		for (auto it : polygon)
		{
			outData << it.x << " " << it.y << "\n";
		}
		outData.close();
	}
}

void Executer::compareResults()
{
	vector <string> paths;
	paths.push_back("../../../Data/MAPDAC/");
	paths.push_back("../../../Data/MAPGreedy/");
	ifstream in("../../../Data/Generated/RECORDS.txt");
	string name;

	for (int i = 0; i < paths.size(); i++)
	{
		cout << i << ") " << paths[i] << "\n";
	}
	cout << "\n\n";

	while (in >> name)
	{
		cout << name << ":";

		for (string path : paths)
		{
			vector <Point> data;
			Point p;

			ifstream inData(path + name);
			while (inData >> p.x >> p.y)
			{
				data.push_back(p);
			}
			inData.close();

			double area = abs(Algorithms::area(data));

			cout << " " << area;
		}

		cout << "\n";
	}
}