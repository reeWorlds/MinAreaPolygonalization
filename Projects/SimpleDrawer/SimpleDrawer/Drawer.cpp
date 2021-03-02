#include "Drawer.h"

Drawer::Drawer() : windowX(1600), windowY(900), windowShift(50), pointR(4.0)
{
	timer = 0;
}

void Drawer::readPoints(string path, sf::Color color)
{
	Point p;

	ifstream in(path);
	while (in >> p.x >> p.y)
	{
		p.y *= -1.0;
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
		p.y *= -1.0;
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
		s.p1.y *= -1.0;
		s.p2.y *= -1.0;

		segments.push_back(s);
		segmentsColor.push_back(color);
	}
	in.close();
}

void Drawer::readTriangles(string path, sf::Color colorEdges, sf::Color colorIn)
{
	double x1, y1, x2, y2, x3, y3;
	
	ifstream in(path);
	while (in >> x1 >> y1 >> x2 >> y2 >> x3 >> y3)
	{
		y1 *= -1;
		y2 *= -1;
		y3 *= -1;

		segments.push_back(Segment(x1, y1, x2, y2));
		segmentsColor.push_back(colorEdges);
		segments.push_back(Segment(x2, y2, x3, y3));
		segmentsColor.push_back(colorEdges);
		segments.push_back(Segment(x3, y3, x1, y1));
		segmentsColor.push_back(colorEdges);

		vector <Point> shape;
		shape.push_back(Point(x1, y1));
		shape.push_back(Point(x2, y2));
		shape.push_back(Point(x3, y3));
		shapes.push_back(shape);
		shapesColor.push_back(colorIn);
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
	if (event.type == sf::Event::Resized)
	{
		auto size = window.getSize();
		double scale = max(double(size.x) / windowX, double(size.y) / windowY);

		window.setSize(sf::Vector2u(windowX * scale, min(windowY * scale, 1080 - 80.0)));
	}

	return true;
}

void Drawer::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	for (int i = 0; i < shapesD.size(); i++)
	{
		sf::ConvexShape shape;

		shape.setPointCount(shapesD[i].size());

		for (int j = 0; j < shapesD[i].size(); j++)
		{
			shape.setPoint(j, sf::Vector2f(shapesD[i][j].x, shapesD[i][j].y));
		}

		shape.setFillColor(shapesColor[i]);

		window.draw(shape);
	}

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
	for (auto shape : shapes)
	{
		for (auto p : shape)
		{
			minX = min(minX, p.x);
			maxX = max(maxX, p.x);

			minY = min(minY, p.y);
			maxY = max(maxY, p.y);
		}
	}

	scaleX = sizeX / (maxX - minX);
	scaleY = sizeY / (maxY - minY);
	scale = min(scaleX, scaleY);

	pointsD.clear();
	for (auto p : points)
	{
		pointsD.push_back(Point((p.x - minX) * scale + windowShift, (p.y - minY) * scale + windowShift));
	}
	segmentsD.clear();
	for (auto s: segments)
	{
		Point p1 = Point((s.p1.x - minX) * scale + windowShift, (s.p1.y - minY) * scale + windowShift);
		Point p2 = Point((s.p2.x - minX) * scale + windowShift, (s.p2.y - minY) * scale + windowShift);

		segmentsD.push_back(Segment(p1, p2));
	}
	shapesD.clear();
	for (auto shape : shapes)
	{
		shapesD.push_back(vector <Point>());
		for (auto p : shape)
		{
			shapesD.back().push_back(Point((p.x - minX) * scale + windowShift, (p.y - minY) * scale + windowShift));
		}
	}
}

void Drawer::show()
{
	rescaleCoordinates();

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(windowX, windowY), "Drawer", sf::Style::Default, settings);

	window.setKeyRepeatEnabled(0);

	process(window);
}