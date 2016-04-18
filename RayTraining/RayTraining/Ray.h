#pragma once

#include <cmath>
#include <vector>
#include <algorithm>
#include "Plane.h"
#include "Printer.h"
#include "Light.h"
#include "Camera.h"

using namespace std;

class Ray : public IGeom {
private:
	MyPoint start, direction;
	Printer printer;

	long double getCurrBrightness(MyPoint camera, MyPoint point, Light* light, vector<IObject*> &obj, int currObj) {
		for (size_t i = 0; i < obj.size(); i++) {
			if (i != currObj && obj[i]->between(point, light->getPlace())) {
				return 0.;
			}
			if (obj[currObj]->getPlaneInPoint(point).putPoint(camera)
				* obj[currObj]->getPlaneInPoint(point).putPoint(light->getPlace()) < 0) {
				return 0.;
			}
		}
		MyPoint ray = MyPoint(point - light->getPlace());
		return max(0.l, light->getPower() * ray.getAngleCos(obj[currObj]->getNormal(point)) / (point.distanceSqr(light->getPlace()) / 10000));
	}

	long double getBrightness(MyPoint camera, MyPoint point, vector<IObject*> &obj, vector<Light*> &lights, int currObj) {
		long double result = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			result += getCurrBrightness(camera, point, lights[i], obj, currObj) / lights.size();
		}
		return min(1.l, result);
	}

public:
	Ray() {};
	Ray(MyPoint first, MyPoint second) {
		start = first;
		direction = second - first;
		direction.normalize();
	}

	MyPoint getStart() { return start; }
	MyPoint getDirection() { return direction; }

	MyPoint intersection(Plane plane) {
		long double t0 = (start * plane.getN() + plane.getD()) / (direction * plane.getN());
		return start + (direction * (-t0));
	}

	bool isIntersection(IObject *obj) {
		return obj->isIntersectWithRay(this);
	}

	MyPoint intersection(IObject *obj) {
		return obj->getIntersectionWithRay(this);
	}

	ObjectColor getColorIfIntersect(IObject *obj) {
		if (isIntersection(obj)) {
			return obj->getColor();
		}
		else {
			return ObjectColor(0, 0, 0);
		}
	}

	ObjectColor getColor(MyPoint camera, vector<IObject*> &objects, vector<Light*> &lights) {
		ObjectColor returnColor = ObjectColor(0, 0, 0);
		int currObject = -1;
		long double minDist = INF;
		MyPoint resultPoint;
		for (size_t i = 0; i < objects.size(); i++) {
			if (isIntersection(objects[i]) && start.distance(intersection(objects[i])) < minDist) {
				//printer.print("Yes");
				returnColor = objects[i]->getColor();
				minDist = start.distance(intersection(objects[i]));
				resultPoint = intersection(objects[i]);
				currObject = i;
			}
			else {
				//printer.print("No");
			}
		}
		if (currObject == -1) {
			return returnColor;
		}
		return returnColor * getBrightness(camera, resultPoint, objects, lights, currObject);
	}
};