#pragma once

#include "IObject.h"
#include <vector>
#include <algorithm>

using namespace std;

class KdTree {
private:
	KdTree *left, *right;
	IObject *object;
	BoundinBox box;
	int currCoordinate;

	int nextCoordinate(int curr) {
		return (curr + 1) % 3;
	}

public:
	BoundinBox getBox() {
		return box;
	}

	KdTree(vector<IObject*>::iterator begin, vector<IObject*>::iterator end, int currCoord) {
		currCoordinate = currCoord;
		if (end != begin + 1) {
			int n = (end - begin) / 2;
			if (currCoordinate == 0) {
				nth_element(begin, begin + n, end, xCompare);
			}
			else if (currCoordinate == 1) {
				nth_element(begin, begin + n, end, yCompare);
			}
			else {
				nth_element(begin, begin + n, end, zCompare);
			}
			object = *(begin + n);
			right = NULL;
			left = new KdTree(begin, begin + n, nextCoordinate(currCoordinate));
			box = BoundinBox(object->getBox(), left->getBox());
			if (end != begin + 2) {
				right = new KdTree(begin + n + 1, end, nextCoordinate(currCoordinate));
				box = BoundinBox(box, right->getBox());
			}
		}
		else {
			object = *begin;
			box = object->getBox();
			left = NULL;
			right = NULL;
		}
	}

	IObject* getObject() {
		return object;
	}
	KdTree* getLeft() {
		return left;
	}
	KdTree* getRight() {
		return right;
	}

	int getCoordinate() {
		return currCoordinate;
	}
};
