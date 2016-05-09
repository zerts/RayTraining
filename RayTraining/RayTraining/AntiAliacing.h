#pragma once

#include <vector>
#include "ObjectColor.h"

using namespace std;

vector<vector<ObjectColor> > antiAliacing(vector<vector<ObjectColor> > &image) {
	//vector<vector<ObjectColor> > result(image.size());
	for (size_t i = 0; i < image.size(); i++) {
		for (size_t j = 0; j < image[i].size(); j++) {
			if (i > 0u && i + 1u < image.size() && j > 0u && j + 1u < image[i].size() && image[i + 1].size() > 0u) {
				image[i][j] = mean(image[i - 1][j], image[i + 1][j], image[i][j - 1], image[i][j + 1]);
			}
		}
	}
	return image;
}