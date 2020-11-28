#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "Timing.h"

// global variables
bool measure = false;
bool graphic = false;
std::string inFile;

struct Point {
    double xPos, yPos;

    bool operator <(const Point& p) const {
        return xPos < p.xPos || (xPos == p.xPos && yPos < p.yPos);
    }
};

class DataGenerator {
public:
    static std::vector<Point> getData(int length, int seed) {
        // initialize engine with a seed
        std::default_random_engine e1(seed);
        std::default_random_engine e2(seed + seed);
        std::uniform_int_distribution<int> uniform_dist(0, 1000000);

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
};

// cross product for vectors OA and OB
double crossProduct(const Point &O, const Point &A, const Point &B) {
    return (A.xPos - O.xPos) * (B.yPos - O.yPos) - (A.yPos - O.yPos) * (B.xPos - O.xPos);
}

std::vector<Point> convexHull(std::vector<Point> points) {
    int listSize = points.size(), k = 0;

    if (listSize <= 3) {
        return points;
    }

    std::vector<Point> ans(2 * listSize);

    // sort points
    sort(points.begin(), points.end());

    for (int i = 0; i < listSize; ++i) {
        while (k >= 2 && crossProduct(ans[k - 2], ans[k - 1], points[i]) <= 0) {
            k--;
        } 
        ans[k++] = points[i];        
    }

    for (int i = listSize - 1, t = k + 1; i > 0; --i) {
        while (k >= t && crossProduct(ans[k - 2], ans[k - 1], points[i - 1]) <= 0)
        {
            k--;
        }
        ans[k++] = points[i - 1];
    }

    // Resize the array to desired size 
    ans.resize(k - 1);

    return ans;
}

std::vector<Point> createPointListFile(std::string fileName)
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

void print_usage() {
    std::cerr << "Usage: --graphic (optional) --load inFile (optional) --measure" << std::endl;
}

int main(int argc, char* argv[])
{
    /*if (argc < 3) {
        print_usage();
        return 1;
    }*/
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--load") {
            if (i + 1 < argc) {
                inFile = argv[i + 1];
            }
            else {
                print_usage();
                return 1;
            }
        }
        else if (std::string(argv[i]) == "--graphic") {
            graphic = true;
        }
        else if (std::string(argv[i]) == "--performance") {
            graphic = false;
        }
        else if (std::string(argv[i]) == "--measure") {
            measure = true;
        }
    }

    // start the Timing class
    Timing* timer = Timing::getInstance();

    timer->startSetup();

    std::vector<Point> points;

    if (inFile != "") {
        createPointListFile(inFile);
    }
    else {
        points = DataGenerator::getData(10, 223);
    }
    
    // Add points 
    /*points.push_back({ 0, 3 });
    points.push_back({ 2, 2 });
    points.push_back({ 1, 1 });
    points.push_back({ 2, 1 });
    points.push_back({ 3, 0 });
    points.push_back({ 0, 0 });
    points.push_back({ 3, 3 });
    points.push_back({ 3, 4 });
    points.push_back({ 4, 3 });*/

    timer->stopSetup();

    timer->startComputation();
    // Find the convex hull 
    std::vector<Point> ans = convexHull(points);
    timer->stopComputation();

    timer->startFinalization();
    // Print the convex hull 
    for (int i = 0; i < ans.size(); i++)
        std::cout << "(" << ans[i].xPos << ", "
        << ans[i].yPos << ")" << std::endl;
    timer->stopFinalization();
    
    // Print timing results
    if (measure) {
        std::cout << timer->getResults() << std::endl;
    }
    return 0;
}