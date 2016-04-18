#pragma once

#include <vector>
#include "Camera.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Poligon.h"
#include "ImageCreator.h"
#include "Light.h"
#include "Reader.h"

using namespace std;

void example() {
	Reader reader;
	MyScreen screen(MyPoint(0., 0., 0.), 1000, 1000);
	Camera camera(MyPoint(100., 100., 100.));
	reader.init(camera, screen);

	vector<IObject*> obj = reader.getObjects();
	//obj.push_back(new Triangle(MyPoint(0., 0., -1.), MyPoint(100., 100., -1.), MyPoint(0., 100., -1.)));
	//obj.back()->setColor(200, 0, 0);

	//obj.push_back(new Triangle(MyPoint(0., 0., -20.), MyPoint(200., 200., -20.), MyPoint(0., 200., -50.)));
	//obj.back()->setColor(0, 200, 0);

	//obj.push_back(new Triangle(MyPoint(0., 0., -30.), MyPoint(300., 300, -30), MyPoint(0., 300., -100.)));
	//obj.back()->setColor(0, 0, 200);

	//obj.push_back(new Poligon(MyPoint(0., 0., -30.), MyPoint(300., 300, -30), MyPoint(0., 300., -30.), MyPoint(100, 150, -30)));
	//obj.back()->setColor(200, 0, 200);

	//obj.push_back(new Sphere(MyPoint(100, 100, -100), 100));
	//obj.back()->setColor(200, 200, 0);
	Printer printer;
	printer.print(obj.size());

	vector<Light*> lights;
	lights.push_back(new Light(MyPoint(50., 50., 50.), 1));
	lights.push_back(new Light(MyPoint(200, 200, 50), 1));
	//lights.push_back(new Light(MyPoint(50, 150, 50), 1));
	//lights.push_back(new Light(MyPoint(150, 50, 50), 1));

	createImage(camera.createPixelArray(screen, obj, lights));
}