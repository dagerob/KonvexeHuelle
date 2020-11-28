#pragma once
#include "Point.h"
#include <vector>
#include <random>
#include <iostream>
#include <sstream>
#include <fstream>

class DataGenerator {
public:
	static std::vector<Point> getData(int length, int seed) {
		// initialize engine with a seed
		std::default_random_engine e1(seed);
		std::default_random_engine e2(seed + seed);
		std::uniform_int_distribution<int> uniform_dist(10, 490);

		std::vector<Point> dataToReturn;
		for (int i = 0; i < length; i++) {
			dataToReturn.push_back({ (double)uniform_dist(e1), (double)uniform_dist(e2) });
		}
		return dataToReturn;
	}

	static void print(const std::vector<int>& vector) {
		for (int i = 0; i < vector.size(); i++) {
			std::cout << vector.at(i) << std::endl;
		}
	}

	static std::vector<Point> createPointListFile(std::string fileName)
	{
		std::vector<Point> points;

		std::ifstream input(fileName);
		if (input.is_open() && input.good())
		{
			std::string gridSize;
			input >> gridSize;

			std::stringstream ss(gridSize);
			double x, y;
			char comma;
			while (input >> x >> comma >> y)
			{
				points.push_back({ x, y });
			}
		}
		else
		{
			std::cout << "Input file could not be opened. Invalid filename or destination";
			exit(0);
		}
		input.close();

		return points;
	}
};