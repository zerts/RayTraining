#pragma once

#include <vector>
#include <chrono>
#include "Camera.h"
#include "Triangle.h"
#include "Sphere.h"
#include "Poligon.h"
#include "Light.h"
#include "Reader.h"
#include "AntiAliacing.h"

using namespace std;

void example() {
	Printer printer;
	Reader reader;
	/*MyScreen screen(MyPoint(-100, -100, 20), 1000, 1000, 0.25);
	Camera camera(MyPoint(1000, 1000, 1000));
	reader.init(camera, screen);
	vector<IObject*> obj = reader.getObjects();
	 
	*/Texture *texture = new Texture(), *texture2 = new Texture(), *texture3 = new Texture();
	texture3->init("wall");
	texture2->init("grass");
	texture->init("metal");
	MyScreen screen(MyPoint(0, 0, 0), 1000, 1000, 1);
	Camera camera(MyPoint(500, 500, 1000));


	//printer.print(texture->getHeight());
	vector<IObject*> obj;
	//obj.push_back(new Triangle(MyPoint(0., 0, -100), MyPoint(100., 0., -100.), MyPoint(100., 100, -100)));
	//obj.back()->setColor(200, 0, 0);
	//obj.back()->setAlpha(2);
	//obj.back()->setMirror(70);
	//obj.back()->setTexture(texture);

	//obj.push_back(new Triangle(MyPoint(0., 0, -10), MyPoint(200, 0., -10.), MyPoint(200., 200, -10)));
	//obj.back()->setColor(200, 200, 0);
	//obj.back()->setMirror(70);

	/*obj.push_back(new Triangle(MyPoint(100., 100., -100.), MyPoint(100, 0, -100.), MyPoint(100., 50., -20)));
	obj.back()->setColor(200, 200, 200);
	obj.back()->setMirror(70);*/

	/*obj.push_back(new Triangle(MyPoint(0., 0., -100.), MyPoint(200., 300., -30), MyPoint(0., 600, -100)));
	obj.back()->setColor(0, 0, 200);
	obj.back()->setMirror(100);
	obj.back()->setTexture(texture);

	obj.push_back(new Triangle(MyPoint(0., 0., -100.), MyPoint(400., 300, -100), MyPoint(200., 300., -30)));
	obj.back()->setColor(0, 0, 200);
	obj.back()->setMirror(100);
	obj.back()->setTexture(texture);

	obj.push_back(new Triangle(MyPoint(400., 300, -100), MyPoint(0, 600, -100), MyPoint(200, 300, -30.)));
	obj.back()->setColor(0, 0, 200);
	obj.back()->setMirror(100);
	obj.back()->setTexture(texture);*/
	
	obj.push_back(new Poligon(MyPoint(0., 0., -1000.), MyPoint(1000., 0, -1000), MyPoint(1000., 1000., -1000.), MyPoint(0, 1000, -1000)));
	obj.back()->setColor(0, 0, 200);
	obj.back()->setTexture(texture);
	obj.back()->setTextureScale(3);
	//obj.back()->setMirror(30);

	obj.push_back(new Poligon(MyPoint(1000., 0., 1000), MyPoint(1000, 1000, 1000), MyPoint(1000, 1000., -1000.), MyPoint(1000, 0, -1000)));
	obj.back()->setColor(0, 200, 0);
	obj.back()->setTexture(texture2);
	obj.back()->setTextureScale(3);
	//obj.back()->setMirror(30);

	obj.push_back(new Poligon(MyPoint(0., 1000., -1000.), MyPoint(1000., 1000, -1000), MyPoint(1000., 1000., 1000), MyPoint(0, 1000, 1000)));
	obj.back()->setColor(0, 200, 200);
	obj.back()->setTexture(texture);
	obj.back()->setTextureScale(3);
	//obj.back()->setMirror(30);

	obj.push_back(new Poligon(MyPoint(0., 0, -1000.), MyPoint(0, 0, 1000), MyPoint(1000., 0, 1000), MyPoint(1000., 0, -1000)));
	obj.back()->setColor(0, 0, 200);
	//obj.back()->setTexture(texture);
	obj.back()->setMirror(30);

	obj.push_back(new Poligon(MyPoint(0, 0., 1000), MyPoint(0, 0, -1000), MyPoint(0, 1000., -1000.), MyPoint(0, 1000, 1000)));
	obj.back()->setColor(0, 200, 0);
	//obj.back()->setTexture(texture2);
	obj.back()->setMirror(30);

	//obj.back()->setTexture(texture);
	//obj.push_back(new Sphere(MyPoint(50, 50, -100), 100));
	//obj.back()->setColor(200, 0, 0);

	obj.push_back(new Sphere(MyPoint(500, 500, -500), 300));
	obj.back()->setColor(200, 200, 0);
	//obj.back()->setAlpha(2);
	//obj.back()->setMirror(50);
	obj.back()->setTexture(texture3);
	obj.back()->setTextureScale(10);

	/*obj.push_back(new Triangle(MyPoint(0, 0, 0), MyPoint(0, 1000, -1000), MyPoint(1000, 1000, -1000)));
	obj.back()->setColor(0, 0, 200);
	obj.back()->setMirror(50);*/


	/*obj.push_back(new Triangle(MyPoint(500, 0, 0), MyPoint(1000, 500, 0), MyPoint(1000, 500, -1000)));
	obj.back()->setColor(200, 0, 0);
	obj.back()->setMirror(50);
	//obj.back()->setAlpha(2);*/
	//printer.print(obj.size());

	vector<Light*> lights;
	//lights.push_back(new Light(MyPoint (30, 30, 30.), 0.8));
	//lights.push_back(new Light(MyPoint(-30, -30, 30), 0.8));
	//lights.push_back(new Light(MyPoint(800, 800, 0), 1));
	lights.push_back(new Light(MyPoint(800, 800, -800), 1));
	//lights.push_back(new Light(MyPoint(900, 900, 0), 1));
	lights.push_back(new Light(MyPoint(700, 700, 0), 1));
	lights.push_back(new Light(MyPoint(800, 50, 0), 1));
	lights.push_back(new Light(MyPoint(200, 200, -800), 1));

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	printer.printImage(antiAliacing(camera.createPixelArray(screen, obj, lights)));
	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;
	dout << "time with kdtree = " << elapsed_seconds.count() << std::endl;
	

	start = std::chrono::system_clock::now();
	//createImage(camera._createPixelArray(screen, obj, lights), "image2.png");
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
	dout << "time without kdtree = " << elapsed_seconds.count() << std::endl;
}