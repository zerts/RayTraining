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
		return max(0.l, light->getPower() * ray.getAngleCos(currObject->getNormal(point, camera)) 
			/ (point.distanceSqr(light->getPlace()) / 300000));
	}

	long double getBrightness(MyPoint camera, MyPoint point, KdTree *root, vector<Light*> &lights, IObject *currObject) {
		long double result = 0;
		for (size_t i = 0; i < lights.size(); i++) {
			result += getCurrBrightness(camera, point, lights[i], root, currObject) / lights.size();
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

			if (currNode->getObject()->getAlpha() < EPS && isIntersection(currNode->getObject()) 
				&& start.distance(intersection(currNode->getObject())) < minDist) {
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

	ObjectColor getColor(long double hasMirrored, MyPoint camera, KdTree *root, vector<Light*> &lights) {
		ObjectColor returnColor = ObjectColor(0, 0, 0);
		if (hasMirrored < 0.01 || distanceToBox(root->getBox()) == INF) {
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

		//return returnColor;
		//printer.print(returnColor);
		MyPoint norm = resultObject->getNormal(resultPoint, camera), newDirection;
		norm.normalize();
		if (resultObject->getAlpha() == 0.) {
			newDirection = (norm * (norm * (camera - resultPoint)) * (2.)) - (camera - resultPoint);
			newDirection = newDirection * 0.0001;
			if (resultObject->getMirror() > 1. - EPS) {
				return returnColor * getBrightness(camera, resultPoint, root, lights, resultObject);
			}
			return ((returnColor * resultObject->getMirror())
				+ (Ray(resultPoint + (newDirection * 0.1), resultPoint + newDirection)
					.getColor(hasMirrored * ((1. - resultObject->getMirror())), resultPoint, root, lights) 
					* (1. - resultObject->getMirror())))
				* getBrightness(camera, resultPoint, root, lights, resultObject);
		}
		else {
			MyPoint fromCam = (camera - resultPoint);
			norm = norm * norm.getAngleCos(fromCam);
			fromCam.normalize();
			long double currSin = sqrtl(1 - sqr(norm.getAngleCos(fromCam))),
				newSin = currSin / resultObject->getAlpha();
			if (newSin >= 1. - EPS) {
				return returnColor * getBrightness(camera, resultPoint, root, lights, resultObject);
			}
			newDirection = norm * (-sqrtl(1. - sqr(newSin))) + ((norm - fromCam) * newSin);
			return Ray(resultPoint + (newDirection * 0.1), resultPoint + newDirection)
					.getColor(hasMirrored, resultPoint, root, lights);
		}
	}
};