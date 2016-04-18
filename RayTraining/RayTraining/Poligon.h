#pragma once

#include "IObject.h"
#include "Point.h"
#include "Ray.h"
#include "Triangle.h"

using namespace std;

class Poligon : public IObject {
private:
	MyPoint a, b, c, d;
	Plane plane;
public:
	Poligon() {};
	Poligon(MyPoint aa, MyPoint bb, MyPoint cc, MyPoint dd) {
		if (Triangle(aa, bb, cc).hasPoint(dd)) {
			a = aa;
			b = bb;
			c = cc;
			d = dd;
		}
		else if (Triangle(bb, cc, dd).hasPoint(aa)) {
			a = bb;
			b = cc;
			c = dd;
			d = aa;
		}
		else if (Triangle(cc, dd, aa).hasPoint(bb)) {
			a = cc;
			b = dd;
			c = aa;
			d = bb;
		}
		else {
			a = dd;
			b = aa;
			c = bb;
			d = cc;
		}

		plane = Plane(aa, bb, cc);
	}
	MyPoint getA() { return a; }
	MyPoint getB() { return b; }
	MyPoint getC() { return c; }
	MyPoint getD() { return d; }
	Plane getPlane() { return plane; }

	bool hasPoint(MyPoint p) {
		return Triangle(a, b, d).hasPoint(p) || Triangle(b, c, d).hasPoint(p);
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