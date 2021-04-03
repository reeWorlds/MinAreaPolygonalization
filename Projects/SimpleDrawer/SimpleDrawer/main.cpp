#include <iostream>

#include "Drawer.h"

using namespace std;



int main()
{
	Drawer drawer;

	/*
	string name = "0_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/Greedy/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPGreedy/" + name, sf::Color::Blue);
	//*/


	/*
	//drawer.readTriangles("../../../Data/Temp/Test5Triangles.txt", sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Temp/test5Points.txt", sf::Color::Blue);
	drawer.readShape("../../../Data/Temp/tesT5Polygon.txt", sf::Color::Black);
	//*/


	//*
	string name = "9_square.txt";
	drawer.readTriangles("../../../Data/Triangulation/DAC/" + name, sf::Color::Green, sf::Color::Cyan);
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPDAC/" + name, sf::Color::Blue);
	//*/

	drawer.show();

	return 0;
}