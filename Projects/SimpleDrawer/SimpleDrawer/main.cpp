#include <iostream>

#include "Drawer.h"

using namespace std;



int main()
{
	Drawer drawer;

	drawer.readPoints("../../../Data/Generated/0_circle.txt", sf::Color::Black);

	drawer.show();

	return 0;
}