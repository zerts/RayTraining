#pragma once

#include "Point.h"
#include "Printer.h"

class MyScreen {
private:
	MyPoint corner, xDirection, yDirection;
	size_t heigth, width, currX, currY;
	Printer printer;
public:
	MyScreen() {};
	MyScreen(MyPoint ccorner, size_t h, size_t w, long double step) {
		corner = ccorner;
		heigth = h;
		width = w;
		xDirection = MyPoint(step, 0., 0.);
		yDirection = MyPoint(0., step, 0.);
		currX = 0;
		currY = 0;
	}
	size_t getHeight() { return heigth; }
	size_t getWidth() { return width; }

	void clearNext() {
		currX = 0;
		currY = 0;
	}

	bool hasNext() {
		return currX + 1 < width;
	}

	MyPoint next() {
		MyPoint curr = corner
			+ xDirection * currX
			+ yDirection * currY;
		currX++;

		//printer.print(xDirection);
		//printer.print(currX);
		//printer.print(curr);
		return curr;
	}

	void nextString() {
		currY++;
		currX = 0;
	}
};