#include "Drawer.h"

Drawer::Drawer() : windowX(1500), windowY(800), windowShift(50), pointR(3.0)
{
	timer = 0;
}

void Drawer::readPoints(string path, sf::Color color)
{
	Point p;

	ifstream in(path);
	while (in >> p.x >> p.y)
	{
		points.push_back(p);
		pointsColor.push_back(color);
	}
	in.close();
}

void Drawer::readShape(string path, sf::Color color)
{
	vector <Point> readPoints;
	Point p;

	ifstream in(path);
	while (in >> p.x >> p.y)
	{
		readPoints.push_back(p);
	}
	in.close();

	for (int i = 1; i < readPoints.size(); i++)
	{
		segments.push_back(Segment(readPoints[i - 1], readPoints[i]));
		segmentsColor.push_back(color);
	}
	segments.push_back(Segment(readPoints.back(), readPoints[0]));
	segmentsColor.push_back(color);
}

void Drawer::readSegments(string path, sf::Color color)
{
	Segment s;

	ifstream in(path);
	while (in >> s.p1.x >> s.p1.y >> s.p2.x >> s.p2.y)
	{
		segments.push_back(s);
		segmentsColor.push_back(color);
	}
	in.close();
}

bool Drawer::switchEvent(sf::Event event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::Closed)
	{
		return false;
	}
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Enter)
		{
			return false;
		}
	}

	return true;
}

void Drawer::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	for (int i = 0; i < segments.size(); i++)
	{
		Segment s = segmentsD[i];

		sf::Vertex line[2] = { sf::Vertex(sf::Vector2f(s.p1.x, s.p1.y)), sf::Vertex(sf::Vector2f(s.p2.x, s.p2.y)) };
		line[0].color = segmentsColor[i];
		line[1].color = segmentsColor[i];

		window.draw(line, 2, sf::Lines);
	}

	for (int i = 0; i < points.size(); i++)
	{
		sf::CircleShape point;
		point.setPosition(pointsD[i].x - pointR, pointsD[i].y - pointR);
		point.setRadius(pointR);
		point.setFillColor(pointsColor[i]);

		window.draw(point);
	}

	window.display();
}

void Drawer::process(sf::RenderWindow& window)
{
	while (window.isOpen())
	{
		timer += clock.getElapsedTime().asMicroseconds() / 1000.0;
		clock.restart();

		if (window.pollEvent(event))
		{
			if (!switchEvent(event, window))
			{
				window.close();
			}
		}

		if (timer > 30.0)
		{
			draw(window);

			timer = 0.0;
		}
	}
}

void Drawer::rescaleCoordinates()
{
	double sizeX = windowX - 2.0 * windowShift;
	double sizeY = windowY - 2.0 * windowShift;

	double minX = points[0].x, minY = points[0].y, maxX = points[0].x, maxY = points[0].y;
	double scaleX, scaleY, scale;

	for (auto p : points)
	{
		minX = min(minX, p.x);
		maxX = max(maxX, p.x);

		minY = min(minY, p.y);
		maxY = max(maxY, p.y);
	}
	for (auto s : segments)
	{
		minX = min(minX, s.p1.x);
		minY = min(minY, s.p1.y);
		maxX = max(maxX, s.p1.x);
		maxY = max(maxY, s.p1.y);

		minX = min(minX, s.p2.x);
		minY = min(minY, s.p2.y);
		maxX = max(maxX, s.p2.x);
		maxY = max(maxY, s.p2.y);
	}

	scaleX = sizeX / (maxX - minX);
	scaleY = sizeY / (maxY - minY);
	scale = min(scaleX, scaleY);

	pointsD.clear();
	for (auto p : points)
	{
		pointsD.push_back(Point((p.x - minX) * scale + windowShift, (p.y - minX) * scale + windowShift));
	}
	segmentsD.clear();
	for (auto s: segments)
	{
		Point p1 = Point((s.p1.x - minX) * scale + windowShift, (s.p1.y - minX) * scale + windowShift);
		Point p2 = Point((s.p2.x - minX) * scale + windowShift, (s.p2.y - minX) * scale + windowShift);

		segmentsD.push_back(Segment(p1, p2));
	}
}

void Drawer::show()
{
	rescaleCoordinates();

	sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Drawer");

	window.setKeyRepeatEnabled(0);

	process(window);
}