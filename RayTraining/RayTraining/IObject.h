#pragma once

#include "IGeom.h"
#include "Point.h"
#include "Plane.h"
#include "Printer.h"
#include "ObjectColor.h"
#include "BoundinBox.h"

class IObject {
protected:
	ObjectColor color;
	Printer printer;
	BoundinBox box;

	long double threeMax(long double aa, long double bb, long double cc) {
		return max(max(aa, bb), cc);
	}
	long double threeMin(long double aa, long double bb, long double cc) {
		return min(min(aa, bb), cc);
	}
	long double fourMax(long double aa, long double bb, long double cc, long double dd) {
		return max(max(aa, bb), max(cc, dd));
	}
	long double fourMin(long double aa, long double bb, long double cc, long double dd) {
		return min(min(aa, bb), max(cc, dd));
	}

public:
	ObjectColor getColor() { return color; }
	void setColor(int r, int g, int b) {
		color = ObjectColor(r, g, b);
	}

	BoundinBox getBox() {
		return box;
	}

	virtual bool isIntersectWithRay(IGeom*) = 0;

	virtual MyPoint getIntersectionWithRay(IGeom*) = 0;

	virtual bool between(MyPoint, MyPoint) = 0;

	virtual MyPoint getNormal(MyPoint) = 0;

	virtual Plane getPlaneInPoint(MyPoint) = 0;

	virtual void createBoundinBox() = 0;
};

bool xCompare(IObject *first, IObject *second) {
	return first->getBox().getDownCorner().getX() < second->getBox().getDownCorner().getX();
}

bool yCompare(IObject *first, IObject *second) {
	return first->getBox().getDownCorner().getY() < second->getBox().getDownCorner().getY();
}

bool zCompare(IObject *first, IObject *second) {
	return first->getBox().getDownCorner().getZ() < second->getBox().getDownCorner().getZ();
}