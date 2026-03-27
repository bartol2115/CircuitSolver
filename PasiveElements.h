#pragma once

#include <iostream>
#include <string>
#include <complex>
#include "Element.h"


// This file defines specific passive components (R, L, C) and overrides virtual methods from the Element class

using namespace std;

class Resistor : public Element {
public:
	Resistor(string name, double value, int nA, int nB, string symbol = "-/\\/\\/\\-") : Element(name, value, symbol, nA, nB) {};

	complex<double> getValue() const override {
		return { e_value, 0.0 };                  // getValue() returns the physical value of the element (Resistance in Ohms, Capacitance in Farads, or Inductance in Henrys).
	}
	string getName() const override {
		return e_name;
	}
	string getSymbol() const override {
		return e_symbol;
	}
	complex<double> getImpedance(double omega) {
		complex<double> Z = getValue();            //Z=R
		return Z;
	}
	void elementInfo() const override {
		cout << "Element: Resistor(" << e_name << "), Value = " << e_value << "[Ohm], Symbol: " << e_symbol << " Nodes: " << nodeA << "," << nodeB << endl;
	}
};

class Capacitor : public Element {
public:
	Capacitor(string name, double value, int nA, int nB, string symbol = "-||-") : Element(name, value, symbol, nA, nB) {};

	complex<double> getValue() const override {
		return { e_value, 0.0 };    
	}
	string getName() const override {
		return e_name;
	}
	string getSymbol() const override {
		return e_symbol;
	}
	complex<double> getImpedance(double omega) {
		complex<double> Z = 1.0 / (j * omega * getValue());  //  Z=1/jwc   w=omega
		return Z;
	}

	void elementInfo() const override {
		cout << "Element: Capacitor(" << e_name << "), Value = " << e_value << "[F], Symbol: " << e_symbol << " Nodes: " << nodeA << "," << nodeB << endl;
	}
};

class Inductor :public Element {
public:
	Inductor(string name, double value, int nA, int nB, string symbol = "-oooo-") : Element(name, value, symbol, nA, nB) {};

	complex<double> getValue() const override {
		return { e_value, 0.0 };
	}
	string getName() const override {
		return e_name;
	}
	string getSymbol() const override {
		return e_symbol;
	}
	complex<double> getImpedance(double omega) {
		complex<double> Z = j * omega * getValue();  //Z=jwL   w=omega
		return Z;
	}

	void elementInfo() const override {
		cout << "Element: Inductor(" << e_name << "), Value = " << e_value << "[H], Symbol: " << e_symbol << " Nodes: " << nodeA << "," << nodeB << endl;
	}
};