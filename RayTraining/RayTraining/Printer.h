#pragma once

#include <fstream>
#include <string>
#include <mutex>
#include "ObjectColor.h"
#include "Plane.h"

using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");
ofstream dout("debugOutput.txt");

class Printer {
private:
	//mutex mut;
public:
	void print(ObjectColor color) {
		//mut.lock();
		dout << "Color = (" << color.getRed() << ", " << color.getGreen() << ", " << color.getBlue() << ")\n";
		//mut.unlock();
	}
	void print(vector<vector<ObjectColor> > pixels) {
		for (size_t i = 0; i < pixels.size(); i++) {
			for (size_t j = 0; j < pixels[i].size(); j++) {
				print(pixels[i][j]);
			}
		}
	}
	void print(MyPoint p) {
		//mut.lock();
		dout << "Point = (" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ")\n";
		//mut.unlock();
	}
	void print(string s) {
		dout << s << endl;
	}
	void print(Plane p) {
		dout << "Plane = (" << p.getA() << ", " << p.getB() << ", " << p.getC() << ", " << p.getD() << ")\n";
	}
	void print(long double i) {
		dout << i << endl;
	}
	void printImage(vector<vector<ObjectColor> > &pixels) {
		out << pixels.size() << " " << pixels[0].size() << endl;
		for (size_t i = 0; i < pixels.size(); i++) {
			for (size_t j = 0; j < pixels[i].size(); j++) {
				out << pixels[i][j].getRed() << " " 
					<< pixels[i][j].getGreen()<< " " 
					<< pixels[i][j].getBlue() << endl;
			}
		}
	}
};