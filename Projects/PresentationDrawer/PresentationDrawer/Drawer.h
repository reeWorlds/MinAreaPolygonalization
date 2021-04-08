#pragma once

#include <SFML/Graphics.hpp>
#include "../../Algorithms/Algorithms/Algorithms.h"

#include <windows.h>

using namespace std;


class Drawer
{
	const int globalWindowWidth, globalWindowHeight;
	const int buttonPanelX, buttonPanelY, buttonPanelWidth, buttonPanelHeight;
	const int graphicPanelX, graphicPanelY, graphicPanelWidth, graphicPanelHeight;

	const int openButtonX, openButtonY, openButtonWidth, openButtonHeight;
	const int saveButtonX, saveButtonY, saveButtonWidth, saveButtonHeight;
	const int MAP_DAC2ButtonX, MAP_DAC2ButtonY, MAP_DAC2ButtonWidth, MAP_DAC2ButtonHeight;
	const int MAP_DACButtonX, MAP_DACButtonY, MAP_DACButtonWidth, MAP_DACButtonHeight;
	const int MAP_GreedyButtonX, MAP_GreedyButtonY, MAP_GreedyButtonWidth, MAP_GreedyButtonHeight;
	const int MAP_RANDButtonX, MAP_RANDButtonY, MAP_RANDButtonWidth, MAP_RANDButtonHeight;
	const int MAP_RSButtonX, MAP_RSButtonY, MAP_RSButtonWidth, MAP_RSButtonHeight;
	const int MAP_PostButtonX, MAP_PostButtonY, MAP_PostButtonWidth, MAP_PostButtonHeight;
	const int triangButtonX, triangButtonY, triangButtonWidth, triangButtonHeight;
	const int addButtonX, addButtonY, addButtonWidth, addButtonHeight;
	const int deleteButtonX, deleteButtonY, deleteButtonWidth, deleteButtonHeight;

	const double pointsR;

	sf::Clock clock;
	double timer;
	sf::Event event;

	sf::Font font;
	sf::Text text;

	int isAddState, isDeleteState;

	double minXScale, minYScale, scale;

	string lastMethod;

	vector <Point> points;
	vector <Point> polygon;
	vector <vector <Point> > insideArea;

	pair<double, double> getMousePosition(sf::RenderWindow& window);
	int isInide(int x, int y, int width, int height, double px, double py);
	void updateScale();

	void processOpenButton(sf::RenderWindow& window);
	void processSaveButton(sf::RenderWindow& window);
	void processMAP_DAC2Button(sf::RenderWindow& window);
	void processMAP_DACButton(sf::RenderWindow& window);
	void processMAP_GreedyButton(sf::RenderWindow& window);
	void processMAP_RANDButton(sf::RenderWindow& window);
	void processMAP_RSButton(sf::RenderWindow& window);
	void processMAP_PostrocessButton(sf::RenderWindow& window);
	void processDrawInnerAreaButton(sf::RenderWindow& window);
	void processAddPointButton(sf::RenderWindow& window);
	void processDeletePointButton(sf::RenderWindow& window);
	void processGraphicAreaClick(sf::RenderWindow& window);

	bool switchEvent(sf::Event event, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);

	void process(sf::RenderWindow& window);

public:

	Drawer();

	void show();
};