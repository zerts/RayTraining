#pragma once

#include <cmath>
#include <algorithm>
#include "IObject.h"
#include "Ray.h"

using namespace std;

class Sphere : public IObject {
private:
	MyPoint center;
	long double r;

public:
	Sphere() {};
	Sphere(MyPoint c, long double rr) {
		center = c;
		r = rr;
	}

	MyPoint getCenter() { return center; }
	long double getRad() { return r; }

	bool hasPoint(MyPoint p) {
		return fabs(p.distance(center) - r) < EPS;
	}

	bool isIntersectWithRay(IGeom *ray) {
		return (((ray->getStart() - center).vec(ray->getDirection())).length()) < r + EPS;
	}

	MyPoint getIntersectionWithRay(IGeom *ray) {
		if (isIntersectWithRay(ray)) {
			long double a = -((ray->getStart() - center) * ray->getDirection());
			MyPoint result = ray->getStart() + (ray->getDirection() * a);
			long double b = result.distanceSqr(center);
			long double c = sqrtl(sqr(r) - b);
			return result - (ray->getDirection() * c);
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
		return center - point;
	}
	Plane getPlaneInPoint(MyPoint point) {
		return Plane(point, getNormal(point));
	}
};