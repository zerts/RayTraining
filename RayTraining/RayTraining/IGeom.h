#pragma once

#include "Plane.h"

class IGeom {
public:
	virtual MyPoint intersection(Plane) = 0;
	virtual MyPoint getDirection() = 0;
	virtual MyPoint getStart() = 0;
};