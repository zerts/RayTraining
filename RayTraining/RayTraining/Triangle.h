#pragma once

#include <cmath>
#include "Ray.h"
#include "IObject.h"
#include "Printer.h"


using namespace std;

class Triangle : public IObject
{
private:
	MyPoint a, b, c;
	Plane plane;
public:
	Triangle() {};
	Triangle(MyPoint aa, MyPoint bb, MyPoint cc) {
		a = aa;
		b = bb;
		c = cc;
		plane = Plane(aa, bb, cc);
	}
	MyPoint getA() { return a; }
	MyPoint getB() { return b; }
	MyPoint getC() { return c; }
	Plane getPlane() { return plane; }

	bool hasPoint(MyPoint p) {
		MyPoint first = a - p, second = b - p, third = c - p;
		return first.vec(second) * second.vec(third) > 0
			&& second.vec(third) * third.vec(first) > 0;
	}

	bool isIntersectWithRay(IGeom *ray) {
		return hasPoint(ray->intersection(plane));
	}

	MyPoint getIntersectionWithRay(IGeom *ray) {
		if (isIntersectWithRay(ray)) {
			return ray->intersection(plane);
		}
		else {
			return MyPoint(INF, INF, INF);
		}
	}

	bool between(MyPoint first, MyPoint second) {
		Ray ray = Ray(second, first);
		if (!isIntersectWithRay(&ray)) {
			return false;
		}
		return second.distance(first) > second.distance(getIntersectionWithRay(&ray));
	}

	MyPoint getNormal(MyPoint point) {
		return plane.getN();
	}
	Plane getPlaneInPoint(MyPoint point) {
		return plane;
	}
};