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

	points.push_back(Point(0.0, 0.0));
	points.push_back(Point(9.0, 0.0));
	points.push_back(Point(0.0, 9.0));
	points.push_back(Point(9.0, 9.0));
	points.push_back(Point(2.0, 2.0));
	points.push_back(Point(2.0, 7.0));
	points.push_back(Point(7.0, 2.0));
	points.push_back(Point(7.0, 7.0));
	points.push_back(Point(3.0, 5.0));
	points.push_back(Point(5.0, 3.0));

	vector <Point> MAP = Algorithms::MAPGreedy(points);

	for (auto p : MAP)
	{
		cout << p.x << " " << p.y << "\n";
	}
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

		break;
	}
}