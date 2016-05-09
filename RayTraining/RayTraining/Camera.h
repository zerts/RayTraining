#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include "Printer.h"
#include "Screen.h"
#include "IObject.h"
#include "Ray.h"
#include "Light.h"
#include "KdTree.h"

using namespace std;

const size_t NUMBER_OF_THREADS = 7;

void threadPixelArrayCreator(vector<vector<ObjectColor> > &result,
	MyScreen *screen, KdTree *objectTree, vector<Light*> &lights, MyPoint placeOfCam) {
	Printer printer;
	for (size_t i = 0; i < screen->getHeight(); i++) {
		while (screen->hasNext()) {
			result[i].push_back(Ray(placeOfCam, screen->next()).getColor(1., placeOfCam, objectTree, lights));
		}
		screen->nextString();
	}
}

class Camera {
private:
	long double x, y, z;
	Printer printer;
	KdTree* objectTree;
public:
	Camera() {};
	Camera(long double xx, long double yy, long double zz) :
		x(xx), y(yy), z(zz) {};
	Camera(MyPoint p) {
		x = p.getX();
		y = p.getY();
		z = p.getZ();
	}

	MyPoint getPlace() {
		return MyPoint(x, y, z);
	}

	vector<vector<ObjectColor> > createPixelArray(MyScreen screen, vector<IObject*> &objects, vector<Light*> &lights) {
		int currHeight = screen.getHeight() / NUMBER_OF_THREADS;
		vector<vector<ObjectColor> > res(currHeight), wholeResult(screen.getHeight());
		vector<vector<vector<ObjectColor> > > result(NUMBER_OF_THREADS, res);
		vector<MyScreen> screens;
		for (int i = 0; i < NUMBER_OF_THREADS; i++) {
			MyPoint currCorner = MyPoint(
				screen.getCorner().getX(), 
				screen.getCorner().getY() + (long double)(currHeight * i) * screen.getStep(), 
				screen.getCorner().getZ());
			screens.push_back(MyScreen(currCorner, currHeight, screen.getWidth(), screen.getStep()));
			screens[i].clearNext();
		}
		vector<thread> threads;
		vector<Camera> cams(NUMBER_OF_THREADS);
		objectTree = new KdTree(objects.begin(), objects.end(), 0);
		for (size_t i = 0; i < NUMBER_OF_THREADS; i++) {
			threads.push_back(thread(
				threadPixelArrayCreator, ref(result[i]), &screens[i], objectTree, ref(lights), getPlace()));
		}
		for (size_t i = 0; i < NUMBER_OF_THREADS; i++) {
			threads[i].join();
		}
		for (size_t i = 0; i < NUMBER_OF_THREADS; i++) {
			for (size_t j = 0; j < result[i].size(); j++) {
				for (size_t e = 0; e < result[i][j].size(); e++) {
					wholeResult[(currHeight * i) + j].push_back(result[i][j][e]);
				}
			}
		}
		return wholeResult;
	}
};
