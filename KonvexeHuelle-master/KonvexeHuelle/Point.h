#pragma once
struct Point {
	double xPos, yPos;

	bool operator <(const Point& p) const {
		return xPos < p.xPos || (xPos == p.xPos && yPos < p.yPos);
	}
};