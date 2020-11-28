#include "RenderingWindow.h"
#include <iostream>
#include "Point.h"

void RenderingWindow::setHullPoints(const std::vector<Point> &points){
	for (auto point : points) {
		sf::CircleShape newCircle(POINT_SIZE);
		newCircle.setOrigin(POINT_SIZE, POINT_SIZE);
		newCircle.setPosition(sf::Vector2f(point.xPos, point.yPos));
		newCircle.setFillColor(sf::Color::Green);
		circlesToRender.push_back(newCircle);
	}
}

 void RenderingWindow::drawPoints() const {
	 for (auto point : circlesToRender) {
		 window.draw(point);
	 }
 }

 void RenderingWindow::bindPoints(const std::vector<Point> &points, sf::Color color) const {
	std::vector<sf::Vertex> vertices;
	for (auto point : points) {
		sf::Vertex thisVertex(sf::Vector2f(point.xPos, point.yPos));
		thisVertex.color = color;
		if(point.xPos !=-1 && point.yPos !=-1)
			vertices.push_back(thisVertex);
	}

	window.draw(&vertices[0],vertices.size(), sf::LineStrip);
 }