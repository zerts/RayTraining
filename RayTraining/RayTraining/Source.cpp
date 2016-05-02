#include "Painter.h"

int main()
{
	system("imageReader.exe");
	example();
	in.close();
	out.close();
	system("Imager.exe");
	return 0;
}

//IMAGER CODE

/*
#pragma once

#include <fstream>
#include <vector>
#include <string>

#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace std;
using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;

ifstream in("output.txt");

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

void createImage(vector<vector<ObjectColor> > &pixels, System::String ^fileName)
{
size_t height = pixels[0].size(), width = pixels.size();
Bitmap bitmap(height, width);
Color currColor;
for (int j = 0; j < height; j++)
{
for (int i = 0; i < width; i++)
{
ObjectColor currColor = pixels[i][j];
bitmap.SetPixel(j, width - i - 1, Color::FromArgb(currColor.getRed(),
currColor.getGreen(),
currColor.getBlue()));
}
}
bitmap.RotateFlip(System::Drawing::RotateFlipType::Rotate90FlipNone);
bitmap.Save(fileName, System::Drawing::Imaging::ImageFormat::Png);
}

int main() {
size_t height, width;
in >> height >> width;
vector<vector<ObjectColor> > pixels(height);
for (size_t i = 0; i < height; i++) {
for (size_t j = 0; j < width; j++) {
int r, g, b;
in >> r >> g >> b;
pixels[i].push_back(ObjectColor(r, g, b));
}
}
createImage(pixels, "image.png");
in.close();
return 0;
}
*/