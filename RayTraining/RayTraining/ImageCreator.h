/*#pragma once

#include"IObject.h"
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
}*/