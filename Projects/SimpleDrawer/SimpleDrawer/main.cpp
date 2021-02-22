#include <iostream>

#include "Drawer.h"

using namespace std;



int main()
{
	Drawer drawer;

	string name = "0_circle.txt";
	drawer.readPoints("../../../Data/Generated/" + name, sf::Color::Black);
	drawer.readShape("../../../Data/MAPGreedy/" + name, sf::Color::Blue);

	drawer.show();

	return 0;
}