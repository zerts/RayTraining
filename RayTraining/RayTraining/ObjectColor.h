#pragma once

class ObjectColor {
private:
	int red, green, blue;
public:
	ObjectColor() {};
	ObjectColor(int r, int g, int b) {
		red = r % 256;
		green = g % 256;
		blue = b % 256;
	}
	int getRed() { return red; }
	int getGreen() { return green; }
	int getBlue() { return blue; }

	ObjectColor operator*(long double t) {
		return ObjectColor(red * t, green * t, blue * t);
	}
};