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
	ObjectColor operator+(ObjectColor anotherColor) {
		return ObjectColor(
			red + anotherColor.getRed(), 
			green + anotherColor.getGreen(), 
			blue + anotherColor.getBlue());
	}
};

ObjectColor mean(ObjectColor first, ObjectColor second, ObjectColor third, ObjectColor forth) {
	return ObjectColor(
		(first.getRed() + second.getRed() + third.getRed() + forth.getRed()) / 4,
		(first.getGreen() + second.getGreen() + third.getGreen() + forth.getGreen()) / 4,
		(first.getBlue() + second.getBlue() + third.getBlue() + forth.getBlue()) / 4
		);
}