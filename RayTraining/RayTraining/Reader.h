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
	Texture *texture;

	Triangle* getNewTriangle() {
		MyPoint first, second, third;
		while (s != vertex) {
			in >> s;
		}
		in >> y >> z >> x;
		long double shiftX = 900., shiftY = 100., shiftZ = 500., scale = 250.;
		first = MyPoint(-x * scale + shiftX, y * scale + shiftY, -z * scale - shiftZ);
		in >> s >> y >> z >> x;
		second = MyPoint(-x * scale + shiftX, y * scale + shiftY, -z * scale - shiftZ);
		in >> s >> y >> z >> x;
		third = MyPoint(-x * scale + shiftX, y * scale + shiftY, -z * scale - shiftZ);
		Triangle tr(first, second, third);
		return new Triangle(first, second, third);
	}
public:
	void init(Camera cam, MyScreen scr) {
		camera = cam;
		screen = scr;
		texture = new Texture();
		texture->init("texture");
	}

	vector<IObject*> getObjects() {
		while (in >> s) {
			if (s == triangle) {
				objects.push_back(getNewTriangle());
				objects.back()->setColor(200, 10, 10);
				//objects.back()->setColor(rand(), rand(), rand());
				//objects.back()->setMirror(100);
				//objects.back()->setTexture(texture);
			}
			if (s == endOfFile) {
				break;
			}
		}
		return objects;
	}
};