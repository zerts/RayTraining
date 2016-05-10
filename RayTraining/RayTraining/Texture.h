#pragma once

#include <vector>
#include "ObjectColor.h"
#include <fstream>
#include <string>

using namespace std;

class Texture {
private:
	vector<vector<ObjectColor> > pixels;

public:
	void init(string fileName) {
		ofstream tout("textureName.txt");
		tout << fileName;
		tout.close();
		system("imageReader.exe");
		size_t height, width;
		ifstream tin(fileName + ".txt");
		tin >> height >> width;
		pixels.resize(height);
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width; j++) {
				int r, g, b;
				tin >> r >> g >> b;
				pixels[i].push_back(ObjectColor(r, g, b));
			}
		}
		tin.close();
	}
	ObjectColor getColor(int i, int j) {
		return pixels[i % pixels.size()][j % pixels[i % pixels.size()].size()];
	}
	int getHeight() { return pixels.size(); }
	int getWidth() { return pixels[0].size(); }
};