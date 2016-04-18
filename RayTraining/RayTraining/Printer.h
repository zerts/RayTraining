#pragma once

#include <fstream>
#include <string>
#include "ObjectColor.h"
#include "Plane.h"

using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");

class Printer {
public:
	void print(ObjectColor color) {
		out << "Color = (" << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ")\n";
	}
	void print(MyPoint p) {
		out << "Point = (" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")\n";
	}
	void print(string s) {
		out << s << endl;
	}
	void print(Plane p) {
		out << "Plane = (" << p.getA() << ", " << p.getB() << ", " << p.getC() << ", " << p.getD() << ")\n";
	}
	void print(long double i) {
		out << i << endl;
	}
};