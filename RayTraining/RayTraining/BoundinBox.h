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

	BoundinBox(BoundinBox first, BoundinBox second) {
		xu = max(first.getUpCorner().getX(), second.getUpCorner().getX());
		xd = min(first.getDownCorner().getX(), second.getDownCorner().getX());

		yu = max(first.getUpCorner().getY(), second.getUpCorner().getY());
		yd = min(first.getDownCorner().getY(), second.getDownCorner().getY());

		zu = max(first.getUpCorner().getZ(), second.getUpCorner().getZ());
		zd = min(first.getDownCorner().getZ(), second.getDownCorner().getZ());
	}

	long double getXU() { return xu; }
	long double getXD() { return xd; }

	long double getYU() { return yu; }
	long double getYD() { return yd; }

	long double getZU() { return zu; }
	long double getZD() { return zd; }

	MyPoint getUpCorner() {
		return MyPoint(xu, yu, zu);
	}

	MyPoint getDownCorner() {
		return MyPoint(xd, yd, zd);
	}

	BoundinBox(long double _xu, long double _xd, 
			   long double _yu, long double _yd, 
			   long double _zu, long double _zd) :
		xu(_xu), xd(_xd), yu(_yu), yd(_yd), zu(_zu), zd(_zd) {};
};
