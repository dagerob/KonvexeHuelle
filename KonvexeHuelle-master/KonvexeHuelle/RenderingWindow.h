#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Point.h"

class RenderingWindow {
private:
	const float POINT_SIZE = 5.0f;
	std::vector<sf::CircleShape> circlesToRender;
	sf::RenderWindow &window;
public:
	RenderingWindow(sf::RenderWindow &window) : window(window) {}
	void setHullPoints(const std::vector<Point> &points);
	void drawPoints() const;
	void bindPoints(const std::vector<Point> &pointso, sf::Color color) const;
};