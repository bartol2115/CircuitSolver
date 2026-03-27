#pragma once

#include <iostream>
#include <string>
#include <complex>
#include "Element.h"

//Implementation of AC / DC sources. Extends basic element data with phase shift[deg] required for phasor.

using namespace std;

class VoltageSource :public Element {
private:
	double phase;
public:
	VoltageSource(string name, double value, double ph, int nA, int nB, string symbol = "-(+|-)-") : phase(ph), Element(name, value, symbol, nA, nB) {};

	complex<double> getValue() const override {
		double rad = phase * 3.14159265 / 180.0;   // Converts phase[deg] to phase[rad]
		return polar(e_value, rad); //returns fhasor   // "polar" converts amplitude and phase into complex number
	}
	string getName() const override {
		return e_name;
	}
	string getSymbol() const override {
		return e_symbol;
	}
	complex<double> getImpedance(double omega) {
		complex<double> Z = (0.0, 0.0);   // An ideal voltage source has 0 impedance (Z = 0 + j0).
		return Z;
	}

	void elementInfo() const override {
		cout << "Element: VoltageSource(" << e_name << "), Value = " << e_value << "[V]," << " Phase: " << phase << "[deg] Symbol: " << e_symbol << " Nodes: " << nodeA << "," << nodeB << endl;
	}
};

class CurrentSource :public Element {
private:
	double phase;
public:
	CurrentSource(string name, double value, double ph, int nA, int nB, string symbol = "-(-->)-") : phase(ph), Element(name, value, symbol, nA, nB) {};

	complex<double> getValue() const override {
		double rad = phase * 3.14159265 / 180.0;
		return polar(e_value, rad); //returns fhasor
	}
	string getName() const override {
		return e_name;
	}
	string getSymbol() const override {
		return e_symbol;
	}
	complex<double> getImpedance(double omega) {
		complex<double> Z = { 1e18, 0.0 }; // An ideal current source has infinite (modeled as 1e18) impedance Z={ 1e18, 0.0 }
		return Z;
	}

	void elementInfo() const override {
		cout << "Element: CurrentSource(" << e_name << "), Value = " << e_value << "[A]," << " Phase: " << phase << "[deg] Symbol: " << e_symbol << " Nodes: " << nodeA << "," << nodeB << endl;
	}
};

