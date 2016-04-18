#pragma once

#include "Point.h"
#include <cmath>
#include <algorithm>

using namespace std;

class BoundinBox {
private:
	long double xu, xd, yu, yd, zu, zd;

public:
	BoundinBox() {};
	BoundinBox(MyPoint first, MyPoint second) {
		xu = max(first.getX(), second.getX());
		xd = min(first.getX(), second.getX());

		yu = max(first.getY(), second.getY());
		yd = min(first.getY(), second.getY());

		zu = max(first.getZ(), second.getZ());
		zd = min(first.getZ(), second.getZ());
	}
};
