#include <iostream>

#include "Drawer.h"

using namespace std;



int main()
{
	Drawer drawer;

	/*
	string name = "9_circle.txt";
	drawer.readTriangles("../../../Data/Triangulation/Greedy/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPGreedy/" + name, sf::Color::Blue);
	//*/


	/*
	//drawer.readTriangles("../../../Data/Temp/Test5Triangles.txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Temp/test8Points.txt", sf::Color::Blue);
	drawer.readShape("../../../Data/Temp/test8Polygon.txt", sf::Color::Black);
	//*/


	/*
	string name = "1_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/DAC/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPDAC/" + name, sf::Color::Blue);
	//*/


	/*
	string name = "1_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/DAC2/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPDAC2/" + name, sf::Color::Blue);
	//*/


	/*
	string name = "0_grid.txt";
	drawer.readTriangles("../../../Data/Triangulation/RAND/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAP_RAND/" + name, sf::Color::Blue);
	//*/

	/*
	string name = "1_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/RS/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAP_RS/" + name, sf::Color::Blue);
	//*/

	//*
	string name = "1_circle.txt";
	//drawer.readTriangles("../../../Data/Triangulation/RS/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	//drawer.readShape("../../../Data/MAPDAC2/" + name, sf::Color::Blue);
	drawer.readShape("../../../Data/Postprocess/MAPDAC2/" + name, sf::Color::Blue);
	//*/

	drawer.show();

	return 0;
}