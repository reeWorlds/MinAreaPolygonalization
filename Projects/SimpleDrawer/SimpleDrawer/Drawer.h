#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Point.h"
#include "Segment.h"

#include <SFML/Graphics.hpp>

using namespace std;

class Drawer
{
	const int windowX, windowY, windowShift;
	const double pointR;

	vector <Point> points, pointsD;
	vector <sf::Color> pointsColor;

	vector <Segment> segments, segmentsD;
	vector <sf::Color> segmentsColor;

	vector <vector <Point> > shapes, shapesD;
	vector <sf::Color> shapesColor;

	sf::Clock clock;
	double timer;

	sf::Event event;

	bool switchEvent(sf::Event event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	void process(sf::RenderWindow& window);

	void rescaleCoordinates();

public:

	Drawer();

	void readPoints(string path, sf::Color color);
	void readShape(string path, sf::Color color);
	void readSegments(string path, sf::Color color);
	void readPolygons(string path, sf::Color color);
	void readTriangles(string path, sf::Color colorEdges, sf::Color colorIn);

	void show();
};