#include <iostream>

#include "Drawer.h"

using namespace std;



int main()
{
	Drawer drawer;

	/*
	string name = "5_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/Greedy/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPGreedy/" + name, sf::Color::Blue);
	//*/


	/*
	drawer.readTriangles("../../../Data/Temp/Test7Triangulation.txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Temp/test7Points.txt", sf::Color::Blue);
	drawer.readShape("../../../Data/Temp/test7Polygon.txt", sf::Color::Black);
	//*/


	/*
	string name = "5_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/DAC/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPDAC/" + name, sf::Color::Blue);
	//*/


	/*
	string name = "5_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/DAC2/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPDAC2/" + name, sf::Color::Blue);
	//*/


	/*
	string name = "5_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/RAND/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAP_RAND/" + name, sf::Color::Blue);
	//*/

	/*
	string name = "5_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/RS/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAP_RS/" + name, sf::Color::Blue);
	//*/

	/*
	string name = "5_square.txt";
	drawer.readTriangles("../../../Data/Temp/Triangulation_5_square.txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/Postprocess/MAPDAC2/" + name, sf::Color::Blue);
	//*/

	/*
	string name = "7";
	drawer.readTriangles("Data/Fig1/trigA" + name + ".txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readTriangles("Data/Fig1/trigB" + name + ".txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readTriangles("Data/Fig1/trigQ" + name + ".txt", sf::Color::Green, sf::Color::Red);
	drawer.readPoints("Data/Fig1/polyA" + name + ".txt", sf::Color::Black);
	drawer.readPoints("Data/Fig1/polyB" + name + ".txt", sf::Color::Black);
	drawer.readShape("Data/Fig1/polyA" + name + ".txt", sf::Color::Blue);
	drawer.readShape("Data/Fig1/polyB" + name + ".txt", sf::Color::Blue);
	//*/

	//*
	drawer.readTriangles("../../../Data/TempFig2/3triag.txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/TempFig2/points.txt", sf::Color::Black);
	drawer.readShape("../../../Data/TempFig2/3.txt", sf::Color::Blue);
	//*/

	drawer.show();

	return 0;
}