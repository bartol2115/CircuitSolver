#pragma once

#include <iostream>
#include <string>
#include <complex>


//Polymorphic base class for circuit components

using namespace std;

inline complex<double> j(0.0, 1.0); //Imaginary unit j, used in complex impedance formulas, "inline" prevents multiple definition errors when included in multiple files.

class Element {
protected:
	string e_name;
	double e_value;
	string e_symbol;
	int nodeA, nodeB;
public:
	Element(string name, double value, string symbol, int nA, int nB) :e_name(name), e_value(value), e_symbol(symbol), nodeA(nA), nodeB(nB) {};
	virtual complex<double> getValue(void) const = 0;
	virtual string getSymbol(void) const = 0;
	virtual string getName(void) const = 0;
	virtual complex<double> getImpedance(double omega) = 0;   //Virutal methods representing data of each element.
	virtual void elementInfo(void) const = 0;
	int getNodeA() const { return nodeA; }
	int getNodeB() const { return nodeB; }
	virtual ~Element() = default;

};

