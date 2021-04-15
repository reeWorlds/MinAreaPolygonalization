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
	
	//string path = "../../../Data/MAPDAC/";
	//string pathOut = "../../../Data/Triangulation/DAC/";

	//string path = "../../../Data/MAPDAC2/";
	//string pathOut = "../../../Data/Triangulation/DAC2/";

	//string path = "../../../Data/MAP_RAND/";
	//string pathOut = "../../../Data/Triangulation/RAND/";
	
	string path = "../../../Data/MAP_RS/";
	string pathOut = "../../../Data/Triangulation/RS/";
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

void Executer::runMAPDAC2()
{
	string path = "../../../Data/Generated/";
	string pathOut = "../../../Data/MAPDAC2/";
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

		vector <Point> polygon = Algorithms::MAPDAC2(data);

		ofstream outData(pathOut + name);
		for (auto it : polygon)
		{
			outData << it.x << " " << it.y << "\n";
		}
		outData.close();
	}
}

void Executer::runMAP_RAND()
{
	string path = "../../../Data/Generated/";
	string pathOut = "../../../Data/MAP_RAND/";
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

		vector <Point> polygon = Algorithms::MAP_RAND(data);

		ofstream outData(pathOut + name);
		for (auto it : polygon)
		{
			outData << it.x << " " << it.y << "\n";
		}
		outData.close();
	}
}

void Executer::testMAP_RS()
{
	mt19937 rng(47);

	while (true)
	{
		vector <Point> poly;
		for (int i = 0; i < 40; i++)
		{
			Point p;
			p.x = rng() % 1000000 / 100000.0;
			p.y = rng() % 1000000 / 100000.0;

			poly.push_back(p);
		}

		auto pp = Algorithms::MAP_RS(poly);
	}
}

void Executer::runMAP_RS()
{
	string path = "../../../Data/Generated/";
	string pathOut = "../../../Data/MAP_RS/";
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

		vector <Point> polygon = Algorithms::MAP_RS(data);

		ofstream outData(pathOut + name);
		for (auto it : polygon)
		{
			outData << it.x << " " << it.y << "\n";
		}
		outData.close();
	}
}

void Executer::runMAP_Postprocess()
{
	vector <pair<string, string> > paths;
	paths.push_back({ "../../../Data/MAPDAC2/", "../../../Data/Postprocess/MAPDAC2/" });
	paths.push_back({ "../../../Data/MAPDAC/", "../../../Data/Postprocess/MAPDAC/" });
	paths.push_back({ "../../../Data/MAPGreedy/", "../../../Data/Postprocess/MAPGreedy/" });
	paths.push_back({ "../../../Data/MAP_RAND/", "../../../Data/Postprocess/MAP_RAND/" });
	paths.push_back({ "../../../Data/MAP_RS/", "../../../Data/Postprocess/MAP_RS/" });

	ifstream in("../../../Data/Generated/RECORDS.txt");
	vector <string> names;
	string _name;

	while (in >> _name)
	{
		names.push_back(_name);
	}

	for (auto path : paths)
	{
		cout << "start path = " << path.first << "\n";

		for (auto name : names)
		{
			ifstream dataIn(path.first + name);
			ofstream dataOut(path.second + name);

			vector <Point> poly;
			Point p;

			while (dataIn >> p.x >> p.y)
			{
				poly.push_back(p);
			}

			vector <Point> newPoly = Algorithms::MAP_Postprocess(poly);

			for (auto it : newPoly)
			{
				dataOut << it.x << " " << it.y << "\n";
			}

			double oldArea = abs(Algorithms::area(poly)), newArea = abs(Algorithms::area(newPoly));

			cout << name << " " << oldArea << " -> " << newArea << "   " << (newArea / oldArea) * 100.0 << "%\n";
		}

		cout << "\n\n";
	}
}

void Executer::compareResults()
{
	vector <string> paths;
	
	paths.push_back("../../../Data/MAPDAC2/");
	paths.push_back("../../../Data/MAPDAC/");
	paths.push_back("../../../Data/MAPGreedy/");
	paths.push_back("../../../Data/MAP_RAND/");
	paths.push_back("../../../Data/MAP_RS/");

	paths.push_back("../../../Data/Postprocess/MAPDAC2/");
	paths.push_back("../../../Data/Postprocess/MAPDAC/");
	paths.push_back("../../../Data/Postprocess/MAPGreedy/");
	paths.push_back("../../../Data/Postprocess/MAP_RAND/");
	paths.push_back("../../../Data/Postprocess/MAP_RS/");

	ifstream in("../../../Data/Generated/RECORDS.txt");
	string name;

	for (int i = 0; i < paths.size(); i++)
	{
		cout << i << ") " << paths[i] << "\n";
	}
	cout << "\n\n";

	while (in >> name)
	{
		vector <double> areas;

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

			cout << "\t" << area;

			areas.push_back(area);
		}

		double min1 = min(min(areas[0], areas[1]), min(areas[2], areas[3]));
		min1 = min(min1, areas[4]);
		double min2 = min(min(areas[5], areas[6]), min(areas[7], areas[8]));
		min2 = min(min2, areas[9]);

		cout << "\t" << 100.0 - min2 / min1 * 100.0;

		cout << "\n";
	}
}

void Executer::showPostprocessingPersentage()
{
	vector <pair<string, string> > paths;
	paths.push_back({ "../../../Data/MAPDAC2/", "../../../Data/Postprocess/MAPDAC2/" });
	paths.push_back({ "../../../Data/MAPDAC/", "../../../Data/Postprocess/MAPDAC/" });
	paths.push_back({ "../../../Data/MAPGreedy/", "../../../Data/Postprocess/MAPGreedy/" });
	paths.push_back({ "../../../Data/MAP_RAND/", "../../../Data/Postprocess/MAP_RAND/" });
	paths.push_back({ "../../../Data/MAP_RS/", "../../../Data/Postprocess/MAP_RS/" });

	ifstream in("../../../Data/Generated/RECORDS.txt");
	vector <string> names;
	string _name;

	while (in >> _name)
	{
		names.push_back(_name);
	}

	for (auto name : names)
	{
		cout << name << "\t";

		for (auto path : paths)
		{
			ifstream in1(path.first + name);
			ifstream in2(path.second + name);

			vector <Point> poly1, poly2;
			Point p;

			while (in1 >> p.x >> p.y)
			{
				poly1.push_back(p);
			}
			while (in2 >> p.x >> p.y)
			{
				poly2.push_back(p);
			}

			double oldArea = abs(Algorithms::area(poly1)), newArea = abs(Algorithms::area(poly2));

			cout << (newArea / oldArea) * 100.0 << "\t";
		}

		cout << "\n";
	}
}

void Executer::forFigures1()
{
	vector <Point> poly;
	Point p;

	ifstream in("../../../Data/Temp/Test7Polygon.txt");

	while (in >> p.x >> p.y)
	{
		poly.push_back(p);
	}
	in.close();

	auto triag = Algorithms::triangulatePolygon(poly);

	ofstream outData("../../../Data/Temp/Test7Triangulation.txt");
	for (auto triangle : triag)
	{
		Point p1, p2, p3;

		tie(p1, p2, p3) = triangle;

		outData << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " " << p3.x << " " << p3.y << "\n";
	}
	outData.close();
}

void Executer::forFigures2()
{
	vector <Point> poly;
	Point p;

	ifstream in("../../../Data/Postprocess/MAPDAC2/5_square.txt");

	while (in >> p.x >> p.y)
	{
		poly.push_back(p);
	}

	auto triag = Algorithms::triangulatePolygon(poly);

	ofstream outData("../../../Data/Temp/Triangulation_5_square.txt");
	for (auto triangle : triag)
	{
		Point p1, p2, p3;

		tie(p1, p2, p3) = triangle;

		outData << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " " << p3.x << " " << p3.y << "\n";
	}
	outData.close();
}