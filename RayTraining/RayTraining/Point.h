#pragma once

#include <cmath>

using namespace std;

const long double EPS = 1e-9;
const long double INF = 1e30;

long double sqr(long double x) {
	return x * x;
}

class MyPoint {
private:
	long double x, y, z;

public:
	MyPoint() {};
	MyPoint(long double xx, long double yy, long double zz) :
		x(xx), y(yy), z(zz) {};

	long double getX() { return x; }
	long double getY() { return y; }
	long double getZ() { return z; }

	long double distanceSqr(MyPoint anotherPoint) {
		return sqr(x - anotherPoint.getX())
			+ sqr(y - anotherPoint.getY())
			+ sqr(z - anotherPoint.getZ());
	}

	long double distance(MyPoint anotherPoint) {
		return sqrtl(distanceSqr(anotherPoint));
	}

	long double length() {
		return distance(MyPoint(0., 0., 0.));
	}

	void normalize() {
		long double d = length();
		x /= d;
		y /= d;
		z /= d;
	}

	bool operator==(MyPoint anotherPoint) {
		return distance(anotherPoint) < EPS;
	}

	long double operator*(MyPoint anotherPoint) {
		return x * anotherPoint.getX()
			+ y * anotherPoint.getY()
			+ z * anotherPoint.getZ();
	}

	MyPoint operator*(long double t) {
		return MyPoint(x * t, y * t, z * t);
	}


	MyPoint operator-(MyPoint anotherPoint) {
		return MyPoint(
			x - anotherPoint.getX(),
			y - anotherPoint.getY(),
			z - anotherPoint.getZ()
			);
	}

	MyPoint operator+(MyPoint anotherPoint) {
		return MyPoint(
			x + anotherPoint.getX(),
			y + anotherPoint.getY(),
			z + anotherPoint.getZ()
			);
	}

	MyPoint vec(MyPoint anotherPoint) {
		return MyPoint(
			y * anotherPoint.getZ() - z * anotherPoint.getY(),
			z * anotherPoint.getX() - x * anotherPoint.getZ(),
			x * anotherPoint.getY() - y * anotherPoint.getX()
			);
	}

	long double getAngleCos(MyPoint anotherPoint) {
		MyPoint curr = MyPoint(x, y, z);
		return (curr * anotherPoint) / (curr.length() * anotherPoint.length());
	}
};