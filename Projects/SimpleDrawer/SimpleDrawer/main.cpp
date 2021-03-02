#include <iostream>

#include "Drawer.h"

using namespace std;



int main()
{
	Drawer drawer;

	/*
	string name = "0_grid.txt";
	drawer.readTriangles("../../../Data/Triangulation/Greedy/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPGreedy/" + name, sf::Color::Blue);
	//*/


	///*
	drawer.readTriangles("../../../Data/Temp/Test4Triangles.txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Temp/test4Points.txt", sf::Color::Blue);
	drawer.readShape("../../../Data/Temp/test4Polygon.txt", sf::Color::Black);
	//*/

	drawer.show();

	return 0;
}