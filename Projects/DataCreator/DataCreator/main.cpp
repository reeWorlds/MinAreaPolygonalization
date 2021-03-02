#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <time.h>
#include <random>
using namespace std;

default_random_engine generator(time(NULL));
uniform_real_distribution<double> distribution(0.0, 1.0);

double getUniform()
{
	return distribution(generator);
}

void createGrid(string path, string fileName, string recordsName, int pointsN, int pointsM, double dist, double R)
{
	set <string> allRecords;
	string ts;

	ifstream recordsIn(path + recordsName);
	while (recordsIn >> ts)
	{
		allRecords.insert(ts);
	}
	allRecords.insert(fileName);
	recordsIn.close();

	set <double> usedX, usedY;

	ofstream out(path + fileName);
	for (int i = 0; i < pointsN; i++)
	{
		for (int j = 0; j < pointsM; j++)
		{
			while (true)
			{
				double x = i * dist + (0.5 - getUniform()) * 2.0 * R;
				double y = j * dist + (0.5 - getUniform()) * 2.0 * R;

				if (usedX.find(x) != usedX.end() || usedY.find(y) != usedY.end())
				{
					continue;
				}

				usedX.insert(x);
				usedY.insert(y);

				out << x << " " << y << "\n";
				break;
			}
		}
	}
	out.close();

	ofstream recordsOut(path + recordsName);
	for (auto s : allRecords)
	{
		recordsOut << s << "\n";
	}
	recordsOut.close();
}

void createUniformSquare(string path, string fileName, string recordsName, double maxX, double maxY, double pointsN)
{
	set <string> allRecords;
	string ts;

	ifstream recordsIn(path + recordsName);
	while (recordsIn >> ts)
	{
		allRecords.insert(ts);
	}
	allRecords.insert(fileName);
	recordsIn.close();

	set <double> usedX, usedY;

	ofstream out(path + fileName);
	for (int i = 0; i < pointsN; i++)
	{
		while (true)
		{
			double x = getUniform() * maxX;
			double y = getUniform() * maxY;

			if (usedX.find(x) != usedX.end() || usedY.find(y) != usedY.end())
			{
				continue;
			}

			usedX.insert(x);
			usedY.insert(y);

			out << x << " " << y << "\n";

			break;
		}
	}
	out.close();

	ofstream recordsOut(path + recordsName);
	for (auto s : allRecords)
	{
		recordsOut << s << "\n";
	}
	recordsOut.close();
}

void createUniformCircle(string path, string fileName, string recordsName, double R, double pointsN)
{
	set <string> allRecords;
	string ts;

	ifstream recordsIn(path + recordsName);
	while (recordsIn >> ts)
	{
		allRecords.insert(ts);
	}
	allRecords.insert(fileName);
	recordsIn.close();

	set <double> usedX, usedY;

	ofstream out(path + fileName);
	for (int i = 0; i < pointsN; i++)
	{
		double x = (getUniform() - 0.5) * 2.0 * R;
		double y = (getUniform() - 0.5) * 2.0 * R;

		if (usedX.find(x) != usedX.end() || usedY.find(y) != usedY.end())
		{
			i--;

			continue;
		}

		if (x * x + y * y <= R * R)
		{
			out << x << " " << y << "\n";

			usedX.insert(x);
			usedY.insert(y);
		}
		else
		{
			i--;
		}
	}
	out.close();

	ofstream recordsOut(path + recordsName);
	for (auto s : allRecords)
	{
		recordsOut << s << "\n";
	}
	recordsOut.close();
}


void makeGrids()
{
	for (int i = 0; i < 6; i++)
	{
		createGrid("../../../Data/Generated/", to_string(i) + "_grid.txt", "RECORDS.txt", 15 + 3 * i, 15 + 3 * i, 10.0, 4.0);
	}

	for (int i = 6; i < 10; i++)
	{
		createGrid("../../../Data/Generated/", to_string(i) + "_grid.txt", "RECORDS.txt", 15 + 3 * i, 10, 10.0, 4.0);
	}
}

void makeUniformSquare()
{
	for (int i = 0; i < 5; i++)
	{
		createUniformSquare("../../../Data/Generated/", to_string(i) + "_square.txt", "RECORDS.txt",
			100.0, 100.0, 20 + 50 * i * i);
	}

	for (int i = 5; i < 10; i++)
	{
		createUniformSquare("../../../Data/Generated/", to_string(i) + "_square.txt", "RECORDS.txt",
			100.0, 30.0, 20 + 30 * (i - 4) * (i - 4));
	}
}

void makeUniformCircle()
{
	for (int i = 0; i < 10; i++)
	{
		createUniformCircle("../../../Data/Generated/", to_string(i) + "_circle.txt", "RECORDS.txt", 50.0, 50 + 50 * i);
	}
}

void tmp()
{
	set <double> usedX, usedY;
	double R = 25.0;
	int pointsN = 20;

	ofstream out("../../../Data/Temp/test4Points.txt");
	for (int i = 0; i < pointsN; i++)
	{
		double x = (getUniform() - 0.5) * 2.0 * R;
		double y = (getUniform() - 0.5) * 2.0 * R;

		if (usedX.find(x) != usedX.end() || usedY.find(y) != usedY.end())
		{
			i--;

			continue;
		}

		if (x * x + y * y <= R * R)
		{
			out << x << " " << y << "\n";

			usedX.insert(x);
			usedY.insert(y);
		}
		else
		{
			i--;
		}
	}
	out.close();
}

int main()
{
	//makeGrids();

	//makeUniformSquare();

	//makeUniformCircle();

	tmp();


	return 0;
}