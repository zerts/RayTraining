#pragma once

#include <vector>
#include "ObjectColor.h"
#include <fstream>

using namespace std;

ifstream tin("texture.txt");

class Texture {
private:
	vector<vector<ObjectColor> > pixels;

public:
	void init() {
		size_t height, width;
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
};