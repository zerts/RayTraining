#pragma once

#include "Point.h"

using namespace std;

class Plane {
private:
	long double a, b, c, d;

public:
	Plane() {};
	Plane(long double aa, long double bb, long double cc, long double dd) :
		a(aa), b(bb), c(cc), d(dd) {};

	Plane(MyPoint first, MyPoint second, MyPoint third) {
		a = (first - second).vec(third - second).getX();
		b = (first - second).vec(third - second).getY();
		c = (first - second).vec(third - second).getZ();
		d = (-1.) * (second.getX() * a + second.getY() * b + second.getZ() * c);
	}

	Plane(MyPoint p, MyPoint n) {
		a = n.getX();
		b = n.getY();
		c = n.getZ();
		d = (-1.) * (p.getX() * a + p.getY() * b + p.getZ() * c);
	}

	long double getA() { return a; }
	long double getB() { return b; }
	long double getC() { return c; }
	long double getD() { return d; }

	long double putPoint(MyPoint point) {
		return a * point.getX() + b * point.getY() + c * point.getZ() + d;
	}

	MyPoint getN() {
		MyPoint ret = MyPoint(a, b, c);
		return ret;
	}
};