#pragma once

#include "IGeom.h"
#include "Point.h"
#include "Plane.h"
#include "Printer.h"
#include "ObjectColor.h"

class IObject {
protected:
	ObjectColor color;
	Printer printer;
public:
	ObjectColor getColor() { return color; }
	void setColor(int r, int g, int b) {
		color = ObjectColor(r, g, b);
	}

	virtual bool isIntersectWithRay(IGeom*) = 0;

	virtual MyPoint getIntersectionWithRay(IGeom*) = 0;

	virtual bool between(MyPoint, MyPoint) = 0;

	virtual MyPoint getNormal(MyPoint) = 0;

	virtual Plane getPlaneInPoint(MyPoint) = 0;
};