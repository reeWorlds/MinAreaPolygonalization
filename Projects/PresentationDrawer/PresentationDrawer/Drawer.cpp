#include "Drawer.h"

Drawer::Drawer() : globalWindowWidth(1280), globalWindowHeight(720),
buttonPanelX(0), buttonPanelY(0), buttonPanelWidth(300), buttonPanelHeight(720),
graphicPanelX(330), graphicPanelY(30), graphicPanelWidth(920), graphicPanelHeight(660),
openButtonX(25), openButtonY(20), openButtonWidth(250), openButtonHeight(50),
saveButtonX(25), saveButtonY(80), saveButtonWidth(250), saveButtonHeight(50),
MAP_DAC2ButtonX(25), MAP_DAC2ButtonY(160), MAP_DAC2ButtonWidth(250), MAP_DAC2ButtonHeight(50),
MAP_DACButtonX(25), MAP_DACButtonY(220), MAP_DACButtonWidth(250), MAP_DACButtonHeight(50),
MAP_GreedyButtonX(25), MAP_GreedyButtonY(280), MAP_GreedyButtonWidth(250), MAP_GreedyButtonHeight(50),
MAP_RANDButtonX(25), MAP_RANDButtonY(340), MAP_RANDButtonWidth(250), MAP_RANDButtonHeight(50),
MAP_RSButtonX(25), MAP_RSButtonY(400), MAP_RSButtonWidth(250), MAP_RSButtonHeight(50),
MAP_PostButtonX(25), MAP_PostButtonY(480), MAP_PostButtonWidth(250), MAP_PostButtonHeight(50),
triangButtonX(25), triangButtonY(560), triangButtonWidth(250), triangButtonHeight(50),
addButtonX(25), addButtonY(640), addButtonWidth(100), addButtonHeight(50),
deleteButtonX(175), deleteButtonY(640), deleteButtonWidth(100), deleteButtonHeight(50),
pointsR(3.0)
{
	timer = 0.0;

	isAddState = 0;
	isDeleteState = 0;

	updateScale();

	font.loadFromFile("Roboto-Regular.ttf");

	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(35);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);
}

pair<double, double> Drawer::getMousePosition(sf::RenderWindow& window)
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

	pair<double, double> scaledPosition = { double(mousePosition.x) / window.getSize().x * globalWindowWidth,
		 double(mousePosition.y) / window.getSize().y * globalWindowHeight };

	return scaledPosition;
}

int Drawer::isInide(int x, int y, int width, int height, double px, double py)
{
	if (px >= x && px <= x + width && py >= y && py <= y + height)
	{
		return 1;
	}

	return 0;
}

void Drawer::updateScale()
{
	if (points.size() == 0)
	{
		minXScale = 0.0;
		minYScale = 0.0;

		scale = 1.0;
	}
	else
	{
		double maxX = -1.0e9, maxY = -1.0e9;
		minXScale = 1.0e9;
		minYScale = 1.0e9;

		for (auto p : points)
		{
			minXScale = min(minXScale, p.x);
			minYScale = min(minYScale, p.y);

			maxX = max(maxX, p.x);
			maxY = max(maxY, p.y);
		}

		double width = maxX - minXScale, height = maxY - minYScale;

		scale = min(graphicPanelWidth / width, graphicPanelHeight / height);
	}
}

void Drawer::processOpenButton(sf::RenderWindow& window)
{
	const char* filter = "All Files (*.*)\0*.*\0";
	HWND owner = NULL;

	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	string fileNameStr;

	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;

	if (fileNameStr != "")
	{
		vector <Point> newPoints;
		Point p;

		ifstream in(fileNameStr);
		while (in >> p.x >> p.y)
		{
			newPoints.push_back(p);
		}

		polygon.clear();
		insideArea.clear();
		points = newPoints;

		updateScale();

		lastMethod = "";
		window.setTitle("Min Area Polygomalization");
	}
}

void Drawer::processSaveButton(sf::RenderWindow& window)
{
	if (polygon.size() == 0)
	{
		return;
	}

	const char* filter = "All Files (*.*)\0*.*\0";
	HWND owner = NULL;

	OPENFILENAME ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";

	string fileNameStr;

	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;

	try
	{
		ofstream out(fileNameStr);

		for (auto p : polygon)
		{
			out << p.x << " " << p.y << "\n";
		}

		out.close();
	}
	catch (...)
	{
		return;
	}
}

void Drawer::processMAP_DAC2Button(sf::RenderWindow& window)
{
	if (points.size() < 3)
	{
		return;
	}
	insideArea.clear();

	polygon = Algorithms::MAPDAC2(points);
	
	double area = abs(Algorithms::area(polygon));
	window.setTitle("Min Area Polygonalization  DAC2  S = " + to_string(area));

	lastMethod = "DAC2";
}

void Drawer::processMAP_DACButton(sf::RenderWindow& window)
{
	if (points.size() < 3)
	{
		return;
	}
	insideArea.clear();

	polygon = Algorithms::MAPDAC(points);

	double area = abs(Algorithms::area(polygon));
	window.setTitle("Min Area Polygonalization  DAC  S = " + to_string(area));

	lastMethod = "DAC";
}

void Drawer::processMAP_GreedyButton(sf::RenderWindow& window)
{
	if (points.size() < 3)
	{
		return;
	}
	insideArea.clear();

	polygon = Algorithms::MAPGreedy(points);

	double area = abs(Algorithms::area(polygon));
	window.setTitle("Min Area Polygonalization  Greedy  S = " + to_string(area));

	lastMethod = "Greedy";
}

void Drawer::processMAP_RANDButton(sf::RenderWindow& window)
{
	if (points.size() < 3)
	{
		return;
	}
	insideArea.clear();

	polygon = Algorithms::MAP_RAND(points, 10);

	double area = abs(Algorithms::area(polygon));
	window.setTitle("Min Area Polygonalization  RAND  S = " + to_string(area));

	lastMethod = "RAND";
}

void Drawer::processMAP_RSButton(sf::RenderWindow& window)
{
	if (points.size() < 3)
	{
		return;
	}
	insideArea.clear();

	polygon = Algorithms::MAP_RS(points, 10);

	double area = abs(Algorithms::area(polygon));
	window.setTitle("Min Area Polygonalization  RS  S = " + to_string(area));

	lastMethod = "RS";
}

void Drawer::processMAP_PostrocessButton(sf::RenderWindow& window)
{
	if (polygon.size() == 0)
	{
		return;
	}

	insideArea.clear();

	polygon = Algorithms::MAP_Postprocess(polygon);

	double area = abs(Algorithms::area(polygon));
	window.setTitle("Min Area Polygonalization  " + lastMethod + "+Postprocess  S = " + to_string(area));
}

void Drawer::processDrawInnerAreaButton(sf::RenderWindow& window)
{
	if (polygon.size() == 0)
	{
		return;
	}

	auto triangulation = Algorithms::triangulatePolygon(polygon);

	for (auto triangle : triangulation)
	{
		Point p1, p2, p3;

		tie(p1, p2, p3) = triangle;

		vector <Point> subPoly;
		subPoly.push_back(p1);
		subPoly.push_back(p2);
		subPoly.push_back(p3);

		insideArea.push_back(subPoly);
	}
}

void Drawer::processAddPointButton(sf::RenderWindow& window)
{
	if (isAddState == 0)
	{
		isAddState = 1;
		isDeleteState = 0;
	}
	else
	{
		isAddState = 0;
	}
}

void Drawer::processDeletePointButton(sf::RenderWindow& window)
{
	if (isDeleteState == 0)
	{
		isDeleteState = 1;
		isAddState = 0;
	}
	else
	{
		isDeleteState = 0;
	}
}

void Drawer::processGraphicAreaClick(sf::RenderWindow& window)
{
	if (isAddState == 1)
	{
		pair<double, double> pos = getMousePosition(window);

		pos.first = minXScale + (pos.first - graphicPanelX) / scale;
		pos.second = minXScale + (pos.second - graphicPanelY) / scale;

		points.push_back(Point(pos.first, pos.second));

		polygon.clear();
		insideArea.clear();
		window.setTitle("Min Area Polygonalization");

		lastMethod = "";
	}
	else if (isDeleteState == 1)
	{
		if (points.size() == 0)
		{
			return;
		}

		pair<double, double> pos = getMousePosition(window);

		pos.first = minXScale + (pos.first - graphicPanelX) / scale;
		pos.second = minXScale + (pos.second - graphicPanelY) / scale;

		int bestI = 0;
		double bestDist = 1.0e9;

		for (int i = 0; i < points.size(); i++)
		{
			double dx = points[i].x - pos.first, dy = points[i].y - pos.second;
			double dist = sqrt(dx * dx + dy * dy);

			if (dist < bestDist)
			{
				bestDist = dist;
				bestI = i;
			}
		}

		points.erase(points.begin() + bestI);

		polygon.clear();
		insideArea.clear();
		window.setTitle("Min Area Polygonalization");

		lastMethod = "";

		if (points.size() == 0)
		{
			updateScale();
		}
	}
}

bool Drawer::switchEvent(sf::Event event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::Closed)
	{
		return false;
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		pair<double, double> pos = getMousePosition(window);

		if (isInide(openButtonX, openButtonY, openButtonWidth, openButtonHeight, pos.first, pos.second))
		{
			processOpenButton(window);
		}
		else if (isInide(saveButtonX, saveButtonY, saveButtonWidth, saveButtonHeight, pos.first, pos.second))
		{
			processSaveButton(window);
		}
		else if (isInide(MAP_DAC2ButtonX, MAP_DAC2ButtonY, MAP_DAC2ButtonWidth, MAP_DAC2ButtonHeight, pos.first, pos.second))
		{
			processMAP_DAC2Button(window);
		}
		else if (isInide(MAP_DACButtonX, MAP_DACButtonY, MAP_DACButtonWidth, MAP_DACButtonHeight, pos.first, pos.second))
		{
			processMAP_DACButton(window);
		}
		else if (isInide(MAP_GreedyButtonX, MAP_GreedyButtonY, MAP_GreedyButtonWidth, MAP_GreedyButtonHeight, pos.first, pos.second))
		{
			processMAP_GreedyButton(window);
		}
		else if (isInide(MAP_RANDButtonX, MAP_RANDButtonY, MAP_RANDButtonWidth, MAP_RANDButtonHeight, pos.first, pos.second))
		{
			processMAP_RANDButton(window);
		}
		else if (isInide(MAP_RSButtonX, MAP_RSButtonY, MAP_RSButtonWidth, MAP_RSButtonHeight, pos.first, pos.second))
		{
			processMAP_RSButton(window);
		}
		else if (isInide(MAP_PostButtonX, MAP_PostButtonY, MAP_PostButtonWidth, MAP_PostButtonHeight, pos.first, pos.second))
		{
			processMAP_PostrocessButton(window);
		}
		else if (isInide(triangButtonX, triangButtonY, triangButtonWidth, triangButtonHeight, pos.first, pos.second))
		{
			processDrawInnerAreaButton(window);
		}
		else if (isInide(addButtonX, addButtonY, addButtonWidth, addButtonHeight, pos.first, pos.second))
		{
			processAddPointButton(window);
		}
		else if (isInide(deleteButtonX, deleteButtonY, deleteButtonWidth, deleteButtonHeight, pos.first, pos.second))
		{
			processDeletePointButton(window);
		}
		else if (isInide(graphicPanelX, graphicPanelY, graphicPanelWidth, graphicPanelHeight, pos.first, pos.second))
		{
			processGraphicAreaClick(window);
		}
	}

	return true;
}

void Drawer::draw(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	{// draw panels
		sf::RectangleShape buttonPanel;
		buttonPanel.setPosition(buttonPanelX, buttonPanelY);
		buttonPanel.setSize(sf::Vector2f(buttonPanelWidth, buttonPanelHeight));
		buttonPanel.setFillColor(sf::Color(120, 255, 255));

		window.draw(buttonPanel);

		sf::RectangleShape graphicPanel;
		graphicPanel.setPosition(graphicPanelX - 2.0 * pointsR, graphicPanelY - pointsR);
		graphicPanel.setSize(sf::Vector2f(graphicPanelWidth + 4.0 * pointsR, graphicPanelHeight + 4.0 * pointsR));
		graphicPanel.setFillColor(sf::Color(240, 240, 240));

		window.draw(graphicPanel);
	}

	{// draw buttons
		{// Open button
			sf::RectangleShape openButtonShape;
			openButtonShape.setPosition(openButtonX, openButtonY);
			openButtonShape.setSize(sf::Vector2f(openButtonWidth, openButtonHeight));
			openButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(openButtonShape);

			text.setString("Open File");
			text.setPosition(openButtonX + 5, openButtonY);

			window.draw(text);
		}

		{// Save button
			sf::RectangleShape saveButtonShape;
			saveButtonShape.setPosition(saveButtonX, saveButtonY);
			saveButtonShape.setSize(sf::Vector2f(saveButtonWidth, saveButtonHeight));
			saveButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(saveButtonShape);

			text.setString("Save File");
			text.setPosition(saveButtonX + 5, saveButtonY);

			window.draw(text);
		}

		{// MAP_DAC2 button
			sf::RectangleShape MAP_DAC2ButtonShape;
			MAP_DAC2ButtonShape.setPosition(MAP_DAC2ButtonX, MAP_DAC2ButtonY);
			MAP_DAC2ButtonShape.setSize(sf::Vector2f(MAP_DAC2ButtonWidth, MAP_DAC2ButtonHeight));
			MAP_DAC2ButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(MAP_DAC2ButtonShape);

			text.setString("MAP_DAC2");
			text.setPosition(MAP_DAC2ButtonX + 5, MAP_DAC2ButtonY);

			window.draw(text);
		}

		{// MAP_DAC button
			sf::RectangleShape MAP_DACButtonShape;
			MAP_DACButtonShape.setPosition(MAP_DACButtonX, MAP_DACButtonY);
			MAP_DACButtonShape.setSize(sf::Vector2f(MAP_DACButtonWidth, MAP_DACButtonHeight));
			MAP_DACButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(MAP_DACButtonShape);

			text.setString("MAP_DAC");
			text.setPosition(MAP_DACButtonX + 5, MAP_DACButtonY);

			window.draw(text);
		}

		{// MAP_Greedy button
			sf::RectangleShape MAP_GreedyButtonShape;
			MAP_GreedyButtonShape.setPosition(MAP_GreedyButtonX, MAP_GreedyButtonY);
			MAP_GreedyButtonShape.setSize(sf::Vector2f(MAP_GreedyButtonWidth, MAP_GreedyButtonHeight));
			MAP_GreedyButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(MAP_GreedyButtonShape);

			text.setString("MAP_Greedy");
			text.setPosition(MAP_GreedyButtonX + 5, MAP_GreedyButtonY);

			window.draw(text);
		}

		{// MAP_RAND button
			sf::RectangleShape MAP_RANDButtonShape;
			MAP_RANDButtonShape.setPosition(MAP_RANDButtonX, MAP_RANDButtonY);
			MAP_RANDButtonShape.setSize(sf::Vector2f(MAP_RANDButtonWidth, MAP_RANDButtonHeight));
			MAP_RANDButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(MAP_RANDButtonShape);

			text.setString("MAP_RAND");
			text.setPosition(MAP_RANDButtonX + 5, MAP_RANDButtonY);

			window.draw(text);
		}

		{// MAP_RS button
			sf::RectangleShape MAP_RSButtonShape;
			MAP_RSButtonShape.setPosition(MAP_RSButtonX, MAP_RSButtonY);
			MAP_RSButtonShape.setSize(sf::Vector2f(MAP_RSButtonWidth, MAP_RSButtonHeight));
			MAP_RSButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(MAP_RSButtonShape);

			text.setString("MAP_RS");
			text.setPosition(MAP_RSButtonX + 5, MAP_RSButtonY);

			window.draw(text);
		}

		{// MAP_Postrocess button
			sf::RectangleShape MAP_PostButtonShape;
			MAP_PostButtonShape.setPosition(MAP_PostButtonX, MAP_PostButtonY);
			MAP_PostButtonShape.setSize(sf::Vector2f(MAP_PostButtonWidth, MAP_PostButtonHeight));
			MAP_PostButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(MAP_PostButtonShape);

			text.setString("Postprocess");
			text.setPosition(MAP_PostButtonX + 5, MAP_PostButtonY);

			window.draw(text);
		}

		{// DrawInnerArea button
			sf::RectangleShape triangButtonShape;
			triangButtonShape.setPosition(triangButtonX, triangButtonY);
			triangButtonShape.setSize(sf::Vector2f(triangButtonWidth, triangButtonHeight));
			triangButtonShape.setFillColor(sf::Color::Color(100, 100, 250));

			window.draw(triangButtonShape);

			text.setString("Add Inner Area");
			text.setPosition(triangButtonX + 5, triangButtonY);

			window.draw(text);
		}

		{// AddPoint button
			sf::RectangleShape addButtonShape;
			addButtonShape.setPosition(addButtonX, addButtonY);
			addButtonShape.setSize(sf::Vector2f(addButtonWidth, addButtonHeight));
			if (isAddState == 1)
			{
				addButtonShape.setFillColor(sf::Color::Color(100, 250, 100));
			}
			else
			{
				addButtonShape.setFillColor(sf::Color::Color(250, 100, 100));
			}

			window.draw(addButtonShape);

			text.setString("+");
			text.setPosition(addButtonX + 40, addButtonY);

			window.draw(text);
		}

		{// DeletePoint button
			sf::RectangleShape deleteButtonShape;
			deleteButtonShape.setPosition(deleteButtonX, deleteButtonY);
			deleteButtonShape.setSize(sf::Vector2f(deleteButtonWidth, deleteButtonHeight));
			if (isDeleteState == 1)
			{
				deleteButtonShape.setFillColor(sf::Color::Color(100, 250, 100));
			}
			else
			{
				deleteButtonShape.setFillColor(sf::Color::Color(250, 100, 100));
			}

			window.draw(deleteButtonShape);

			text.setString("-");
			text.setPosition(deleteButtonX + 45, deleteButtonY);

			window.draw(text);
		}
	}

	if(points.size() > 0)
	{
		if(polygon.size() > 0)
		{
			if (insideArea.size() > 0)
			{
				for (int i = 0; i < insideArea.size(); i++)
				{
					sf::ConvexShape shape;

					shape.setPointCount(insideArea[i].size());

					for (int j = 0; j < insideArea[i].size(); j++)
					{
						Point p = insideArea[i][j];

						p.x = graphicPanelX + (p.x - minXScale) * scale;
						p.y = graphicPanelY + (p.y - minYScale) * scale;

						shape.setPoint(j, sf::Vector2f(p.x, p.y));
					}

					shape.setFillColor(sf::Color::Cyan);

					window.draw(shape);
				}
			}

			polygon.push_back(polygon[0]);

			for (int i = 1; i < polygon.size(); i++)
			{
				Point p1 = polygon[i - 1];
				Point p2 = polygon[i];

				p1.x = graphicPanelX + (p1.x - minXScale) * scale;
				p1.y = graphicPanelY + (p1.y - minYScale) * scale;
				p2.x = graphicPanelX + (p2.x - minXScale) * scale;
				p2.y = graphicPanelY + (p2.y - minYScale) * scale;

				sf::Vertex line[2] = { sf::Vertex(sf::Vector2f(p1.x, p1.y)), sf::Vertex(sf::Vector2f(p2.x, p2.y)) };
				line[0].color = sf::Color::Blue;
				line[1].color = sf::Color::Blue;

				window.draw(line, 2, sf::Lines);
			}

			polygon.pop_back();
		}

		for (auto point : points)
		{
			sf::CircleShape pointShape;
			pointShape.setPosition(graphicPanelX + (point.x - minXScale) * scale - pointsR,
				graphicPanelY + (point.y - minYScale) * scale - pointsR);
			pointShape.setRadius(pointsR);
			pointShape.setFillColor(sf::Color::Black);

			window.draw(pointShape);
		}
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

		if (timer > 25.0)
		{
			draw(window);

			timer = 0.0;
		}
	}
}

void Drawer::show()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(globalWindowWidth, globalWindowHeight),
		"Min Area Polygomalization", sf::Style::Default, settings);

	window.setKeyRepeatEnabled(0);

	process(window);
}