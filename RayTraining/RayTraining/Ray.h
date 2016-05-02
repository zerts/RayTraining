#pragma once

#include <cmath>
#include <vector>
#include <algorithm>
#include "Plane.h"
#include "Printer.h"
#include "Light.h"
#include "Camera.h"
#include "BoundinBox.h"
#include "KdTree.h"
 
using namespace std;

class Ray : public IGeom {
private:
	MyPoint start, direction;
	Printer printer;

	long double getCurrBrightness(MyPoint camera, MyPoint point, Light* light, KdTree *root, IObject *currObject) {
		
		if (Ray(light->getPlace(), point).getMinDistanceToObject(root) + EPS < light->getPlace().distance(point)) {
			return 0.;
		}
		MyPoint ray = MyPoint(point - light->getPlace());
		return max(0.l, light->getPower() * ray.getAngleCos(currObject->getNormal(point)) / (point.distanceSqr(light->getPlace()) / 100000));
	}

	long double getBrightness(MyPoint camera, MyPoint point, KdTree *root, vector<Light*> &lights, IObject *currObject) {
		long double result = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			result += getCurrBrightness(camera, point, lights[i], root, currObject) / lights.size();
		}
		return min(1.l, result);
	}

	long double _getCurrBrightness(MyPoint camera, MyPoint point, Light* light, vector<IObject*> &obj, int currObj) {
		for (size_t i = 0; i < obj.size(); i++) {
			if (i != currObj && obj[i]->between(point, light->getPlace())) {
				return 0.;
			}
			if (obj[currObj]->getPlaneInPoint(point).putPoint(camera)
				* obj[currObj]->getPlaneInPoint(point).putPoint(light->getPlace()) < 0) {
				return 0.;
			}
		}
		MyPoint ray = MyPoint(point - light->getPlace());
		return max(0.l, light->getPower() * ray.getAngleCos(obj[currObj]->getNormal(point)) / (point.distanceSqr(light->getPlace()) / 10000));
	}

	long double _getBrightness(MyPoint camera, MyPoint point, vector<IObject*> &obj, vector<Light*> &lights, int currObj) {
		long double result = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			result += _getCurrBrightness(camera, point, lights[i], obj, currObj) / lights.size();
		}
		return min(1.l, result);
	}

	long double distanceToBoxSide(MyPoint first, MyPoint second, MyPoint third, BoundinBox box, 
							long double isX, long double isY, long double isZ) {
		Plane plane(first, second, third);
		MyPoint inter = intersection(plane);
		long double xu = box.getXU() * isX, xd = box.getXD() * isX, xcurr = inter.getX() * isX,
					yu = box.getYU() * isY, yd = box.getYD() * isY, ycurr = inter.getY() * isY,
					zu = box.getZU() * isZ, zd = box.getZD() * isZ, zcurr = inter.getZ() * isZ;
		if (xcurr >= xd && xcurr <= xu
			&& ycurr >= yd && ycurr <= yu
			&& zcurr >= zd && zcurr <= zu) {
			return start.distance(inter);
		}
		else {
			return INF;
		}
	}

	long double distanceToBox(BoundinBox box) {
		//return 1.;
		vector<long double> result(6);
		result[0] = distanceToBoxSide(
			MyPoint(box.getXU(), box.getYU(), box.getZU()),
			MyPoint(box.getXU(), box.getYD(), box.getZU()),
			MyPoint(box.getXD(), box.getYU(), box.getZU()),
			box, 1., 1., 0.);
		result[1] = distanceToBoxSide(
			MyPoint(box.getXU(), box.getYU(), box.getZD()),
			MyPoint(box.getXU(), box.getYD(), box.getZD()),
			MyPoint(box.getXD(), box.getYU(), box.getZD()),
			box, 1., 1., 0.);

		result[2] = distanceToBoxSide(
			MyPoint(box.getXU(), box.getYU(), box.getZU()),
			MyPoint(box.getXU(), box.getYU(), box.getZD()),
			MyPoint(box.getXD(), box.getYU(), box.getZU()),
			box, 1., 0., 1.);
		result[3] = distanceToBoxSide(
			MyPoint(box.getXU(), box.getYD(), box.getZU()),
			MyPoint(box.getXU(), box.getYD(), box.getZD()),
			MyPoint(box.getXD(), box.getYD(), box.getZU()),
			box, 1., 0., 1.);

		result[4] = distanceToBoxSide(
			MyPoint(box.getXU(), box.getYU(), box.getZU()),
			MyPoint(box.getXU(), box.getYU(), box.getZD()),
			MyPoint(box.getXU(), box.getYD(), box.getZU()),
			box, 0., 1., 1.);
		result[5] = distanceToBoxSide(
			MyPoint(box.getXD(), box.getYU(), box.getZU()),
			MyPoint(box.getXD(), box.getYU(), box.getZD()),
			MyPoint(box.getXD(), box.getYD(), box.getZU()),
			box, 0., 1., 1.);

		sort(result.begin(), result.end());
		return result[0];
	}

public:
	Ray() {};
	Ray(MyPoint st, MyPoint fin) {
		start = st;
		direction = fin - st;
		direction.normalize();
	}

	MyPoint getStart() { return start; }
	MyPoint getDirection() { return direction; }

	MyPoint intersection(Plane plane) {
		if (direction * plane.getN() == 0.) {
			return MyPoint(INF, INF, INF);
		}
		long double t0 = (start * plane.getN() + plane.getD()) / (direction * plane.getN());
		return start + (direction * (-t0));
	}

	bool isIntersection(IObject *obj) {
		return obj->isIntersectWithRay(this);
	}

	MyPoint intersection(IObject *obj) {
		return obj->getIntersectionWithRay(this);
	}

	ObjectColor getColorIfIntersect(IObject *obj) {
		if (isIntersection(obj)) {
			return obj->getColor();
		}
		else {
			return ObjectColor(0, 0, 0);
		}
	}

	long double getMinDistanceToObject(KdTree *root) {
		long double minDist = INF;
		bool putTwoToStack = true;
		vector<KdTree*> treeStack;
		treeStack.push_back(root);
		while (!treeStack.empty()) {
			KdTree* currNode = treeStack.back();
			treeStack.pop_back();

			if (isIntersection(currNode->getObject()) && start.distance(intersection(currNode->getObject())) < minDist) {
				//printer.print("Yes");
				minDist = start.distance(intersection(currNode->getObject()));
				//treeStack.clear();

			}

			long double leftDistance = INF, rightDistance = INF;
			if (currNode->getLeft() != NULL) {
				leftDistance = distanceToBox(currNode->getLeft()->getBox());
				if (currNode->getRight() != NULL) {
					rightDistance = distanceToBox(currNode->getRight()->getBox());
				}
				if (rightDistance != INF && rightDistance > leftDistance) {
					treeStack.push_back(currNode->getRight());
					treeStack.push_back(currNode->getLeft());
				}
				else if (rightDistance != INF && rightDistance <= leftDistance) {
					if (leftDistance != INF) {
						treeStack.push_back(currNode->getLeft());
						treeStack.push_back(currNode->getRight());
					}
					else {
						treeStack.push_back(currNode->getRight());
					}
				}
				else if (leftDistance != INF) {
					treeStack.push_back(currNode->getLeft());
				}
			}
		}
		return minDist;
	}

	ObjectColor getColor(bool hasMirrored, MyPoint camera, KdTree *root, vector<Light*> &lights) {
		ObjectColor returnColor = ObjectColor(0, 0, 0);
		if (distanceToBox(root->getBox()) == INF) {
			return returnColor;
		}
		bool putTwoToStack = true;
		IObject *resultObject = NULL;
		long double minDist = INF;
		MyPoint resultPoint;
		vector< KdTree*> treeStack;
		treeStack.push_back(root);
		while (!treeStack.empty()) {
			KdTree* currNode = treeStack.back();
			treeStack.pop_back();

			if (isIntersection(currNode->getObject()) && start.distance(intersection(currNode->getObject())) < minDist
				&& direction.getAngleCos(intersection(currNode->getObject()) - start) > 0.) {
				//printer.print("Yes");
				resultPoint = intersection(currNode->getObject());
				returnColor = currNode->getObject()->getTextureColor(resultPoint);
				minDist = start.distance(intersection(currNode->getObject()));
				resultObject = currNode->getObject();
				//treeStack.clear();

			}

			long double leftDistance = INF, rightDistance = INF;
			if (currNode->getLeft() != NULL) {
				leftDistance = distanceToBox(currNode->getLeft()->getBox());
				if (currNode->getRight() != NULL) {
					rightDistance = distanceToBox(currNode->getRight()->getBox());
				}
				if (rightDistance != INF && rightDistance > leftDistance) {
					treeStack.push_back(currNode->getRight());
					treeStack.push_back(currNode->getLeft());
				}
				else if (rightDistance != INF && rightDistance <= leftDistance) {
					if (leftDistance != INF) {
						treeStack.push_back(currNode->getLeft());
						treeStack.push_back(currNode->getRight());
					}
					else {
						treeStack.push_back(currNode->getRight());
					}
				}
				else if (leftDistance != INF) {
					treeStack.push_back(currNode->getLeft());
				}
			}
		}
		if (resultObject == NULL) {
			return returnColor;
		}
		MyPoint norm = resultObject->getNormal(resultPoint);
		MyPoint newDirection = (resultPoint - camera) + (norm * (norm * (camera - resultPoint)) * (2));
		//printer.print(newDirection);
		/*if (hasMirrored) {
			return returnColor;// *getBrightness(camera, resultPoint, root, lights, resultObject);
		}*/
		//printer.print(returnColor);
		return (returnColor * resultObject->getMirror() * getBrightness(camera, resultPoint, root, lights, resultObject));
			/*+ (Ray(resultPoint + (newDirection * 0.1), resultPoint + newDirection)
				.getColor(true, resultPoint, root, lights) * (1. - resultObject->getMirror()));*/
	}

	ObjectColor _getColor(MyPoint camera, vector<IObject*> &objects, vector<Light*> &lights) {
		ObjectColor returnColor = ObjectColor(0, 0, 0);
		int currObject = -1;
		long double minDist = INF;
		MyPoint resultPoint;
		for (size_t i = 0; i < objects.size(); i++) {
			if (isIntersection(objects[i]) && start.distance(intersection(objects[i])) < minDist) {
				//printer.print("Yes");
				returnColor = objects[i]->getColor();
				minDist = start.distance(intersection(objects[i]));
				resultPoint = intersection(objects[i]);
				currObject = i;
			}
			else {
			//printer.print("No");
			}
		}
		if (currObject == -1) {
			return returnColor;
		}
		return returnColor * _getBrightness(camera, resultPoint, objects, lights, currObject);
	}
};