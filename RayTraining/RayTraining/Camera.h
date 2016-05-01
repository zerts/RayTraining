#pragma once

#include <vector>
#include "Printer.h"
#include "Screen.h"
#include "IObject.h"
#include "Ray.h"
#include "Light.h"
#include "KdTree.h"

using namespace std;

class Camera {
private:
	long double x, y, z;
	Printer printer;
	KdTree* objectTree;
public:
	Camera() {};
	Camera(long double xx, long double yy, long double zz) :
		x(xx), y(yy), z(zz) {};
	Camera(MyPoint p) {
		x = p.getX();
		y = p.getY();
		z = p.getZ();
	}

	MyPoint getPlace() {
		return MyPoint(x, y, z);
	}

	vector<vector<ObjectColor> > createPixelArray(MyScreen screen, vector<IObject*> &objects, vector<Light*> &lights) {
		vector<vector<ObjectColor> > result(screen.getHeight());
		objectTree = new KdTree(objects.begin(), objects.end(), 0);
		for (size_t i = 0; i < screen.getHeight(); i++) {
			while (screen.hasNext()) {
				//printer.print(Ray(MyPoint(x, y, z), screen.next() - MyPoint(x, y, z)).getColor(objects));
				//printer.print(screen.next() - MyPoint(x, y, z));
				result[i].push_back(Ray(MyPoint(x, y, z), screen.next()).getColor(getPlace(), objectTree, lights));
			}
			screen.nextString();
		}

		return result;
	}

	vector<vector<ObjectColor> > _createPixelArray(MyScreen screen, vector<IObject*> &objects, vector<Light*> &lights) {
		vector<vector<ObjectColor> > result(screen.getHeight());
		for (size_t i = 0; i < screen.getHeight(); i++) {
			while (screen.hasNext()) {
				//printer.print(Ray(MyPoint(x, y, z), screen.next() - MyPoint(x, y, z)).getColor(objects));
				//printer.print(screen.next() - MyPoint(x, y, z));
				result[i].push_back(Ray(MyPoint(x, y, z), screen.next())._getColor(getPlace(), objects, lights));
			}
			screen.nextString();
		}

		return result;
	}
};