#pragma once

#include "Point.h"

using namespace std;

class Light {
private:
	MyPoint place;
	long double power;

public:
	Light() {};
	Light(MyPoint p, long double pow) {
		place = p;
		power = pow;
	}
	MyPoint getPlace() { return place; }
	long double getPower() { return power; }
};