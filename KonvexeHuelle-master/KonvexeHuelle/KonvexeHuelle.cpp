#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "RenderingWindow.h"
#include "Point.h"
#include "DataGenerator.h"
#include <cstring>
#include "Timing.h"

bool random = true;
bool graphic = false;
int seed = 1234;
int pointCount = 10;
std::string inFile;

// cross product for vectors OA and OB
double crossProduct(const Point &O, const Point &A, const Point &B) {
	return (A.xPos - O.xPos) * (B.yPos - O.yPos) - (A.yPos - O.yPos) * (B.xPos - O.xPos);
}

std::vector<std::vector<Point>> convexHull(std::vector<Point> points) {
	int listSize = points.size(), k = 0;

	std::vector<std::vector<Point>> record;
	std::vector<Point> ans;

	for (int i = 0; i < 2 * listSize; i++) {
		ans.push_back({ -1, -1 });
	}

	if (listSize <= 3) {
		record.push_back(points);
		return record;
	}

	// sort points
	sort(points.begin(), points.end());

	for (int i = 0; i < listSize; ++i) {
		while (k >= 2 && crossProduct(ans[k - 2], ans[k - 1], points[i]) <= 0) {
			k--;
		}
		ans[k++] = points[i];
		record.push_back(ans);
	}

	for (int i = listSize - 1, t = k + 1; i > 0; --i) {
		while (k >= t && crossProduct(ans[k - 2], ans[k - 1], points[i - 1]) <= 0)
		{
			k--;
		}
		ans[k++] = points[i - 1];
		record.push_back(ans);
	}

	// Resize the array to desired size 
	ans.resize(k - 1);
	return record;
}

void print_usage() {
	std::cerr << "Usage: --graphic (optional) --load inFile (optional) --seed seedValue --pointCount pointCountValue" << std::endl;
}

int main(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "--load") {
			if (i + 1 < argc) {
				random = false;
				inFile = argv[i + 1];
			}
			else {
				print_usage();
				return 1;
			}
		}else if (std::string(argv[i]) == "--graphic") {
			graphic = true;
		}else if (std::string(argv[i]) == "--seed") {
			if (i + 1 < argc) {
				seed = std::stoi(argv[i + 1]);
			}
			else {
				print_usage();
				return 1;
			}
		}else if (std::string(argv[i]) == "--pointCount") {
			if (i + 1 < argc) {
				pointCount = std::stoi(argv[i + 1]);
			}
			else {
				print_usage();
				return 1;
			}
		}
	}

	std::vector<Point> points;
	Timing* timer = Timing::getInstance();

	if(random)
		points = DataGenerator::getData(pointCount, seed);
	else
		points = DataGenerator::createPointListFile(inFile);

	timer->startRecord("timing 1");
	std::vector<std::vector<Point>> result = convexHull(points);
	timer->stopRecord("timing 1");
	timer->print();


	if (!graphic)
		return 1;

	int size = result.size();

	sf::RenderWindow window(sf::VideoMode(500, 500), "Huelle");
	RenderingWindow myWindow(window);

	myWindow.setHullPoints(points);
	int step = 0;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// get the next step from hull algorithm

		window.clear();
		myWindow.bindPoints(result[step], sf::Color::Red);
		myWindow.drawPoints();
		window.display();
		if (step + 1 <= size - 1)
			step++;

		sf::sleep(sf::seconds(1));
	}

	return 0;
}


