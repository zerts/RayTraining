#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "IObject.h"
#include "Triangle.h"
#include "Printer.h"
#include "Camera.h"
#include "Screen.h"

using namespace std;

class Reader {
private:
	const string
		vertex = "vertex",
		endOfFile = "endsolid",
		triangle = "loop";

	vector<IObject*> objects;
	long double x, y, z;
	string s;
	Printer printer;
	Camera camera;
	MyScreen screen;

	Triangle* getNewTriangle() {
		MyPoint first, second, third;
		while (s != vertex) {
			in >> s;
		}
		in >> x >> y >> z;
		first = MyPoint(x, y, z);
		in >> s >> x >> y >> z;
		second = MyPoint(x, y, z);
		in >> s >> x >> y >> z;
		third = MyPoint(x, y, z);
		Triangle tr(first, second, third);
		if (tr.getNormal(first) * (camera.getPlace() - first) < 0.) {
			return new Triangle(first, second, third);
		}
		return new Triangle(first, third, second);
	}
public:
	void init(Camera cam, MyScreen scr) {
		camera = cam;
		screen = scr;
	}

	vector<IObject*> getObjects() {
		while (in >> s) {
			if (s == triangle) {
				objects.push_back(getNewTriangle());
				objects.back()->setColor(250, 200, 100);
			}
			if (s == endOfFile) {
				break;
			}
		}
		return objects;
	}
};