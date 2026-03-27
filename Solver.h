#pragma once

#include <iostream>
#include <string>
#include <complex>
#include <vector>
#include "Element.h"

// Main simulation engine. Transforms circuit topology into a system of complex linear equations (Ax = B).

using namespace std;

class Solver {
private:
	double omega;
	vector<shared_ptr<Element>> circuitComponents;
	int maxNode = 0;
	vector<vector<complex<double>>> matrixA;  //Admittance Matrix ( left side of Equation)
	vector<complex<double>> vectorB;  //Currents vector, right side of equation , x-unknown Voltages
public:
	Solver(double w) : omega(w) {};

	void addElement(shared_ptr<Element> e) {   
		circuitComponents.push_back(e);			// Adds new elements to the circuitComponents vector.
		if (e->getNodeA() > maxNode) maxNode = e->getNodeA();  //The container dynamically expands
		if (e->getNodeB() > maxNode) maxNode = e->getNodeB(); 
	}

	void createMatrix() {
		int numVoltageSources = 0;

		for (const auto& e : circuitComponents) {
			if (e->getName()[0] == 'E' || e->getName()[0] == 'V') { numVoltageSources++; }; //If name of component starts with E or V(like typicall voltage source), numVoltageSources++
		}

		int size = maxNode + numVoltageSources;   //In modyfied Nodal Analysis voltage sources add new rows and columns to matrix A and new components to vector B.
		matrixA.assign(size, vector<complex<double>>(size, complex<double>(0.0, 0.0))); // Initializes Matrix A of size (maxNode + numVoltageSources) with zero complex values.
		vectorB.assign(size, complex<double>(0.0, 0.0));   //Initializes Vector B with zeros

		//Initialising Matrix A with passive Elements
		for (const auto& e : circuitComponents) {
			if (e->getName()[0] == 'E' || e->getName()[0] == 'V' || e->getName()[0] == 'I') { continue; }; //If voltage or current source skip for now
			complex <double> Z = e->getImpedance(omega);
			complex<double> Y = 1.0 / Z; //calulating Admittance

			int a = e->getNodeA();
			int b = e->getNodeB();

			if (a > 0) { matrixA[a - 1][a - 1] += Y; }  // Diagonal: Adds admittance to the nodes connected to the elements.
			if (b > 0) { matrixA[b - 1][b - 1] += Y; }  // "+=" Adds and assigns  (A = A + value)
			if (a > 0 && b > 0) {
				matrixA[a - 1][b - 1] -= Y;  // Off-diagonal: Represents the admittance between nodes a and b.
				matrixA[b - 1][a - 1] -= Y;  //"-=" Substracts and assigns  (A = A - value)
			}
		}
		//Initialising Vector B with current sources
		for (const auto& e : circuitComponents) {
			if (e->getName()[0] == 'I') { //if current source
				int a = e->getNodeA();
				int b = e->getNodeB();
				complex<double> I_val = e->getValue();

				if (a > 0) vectorB[a - 1] -= I_val; // Direction: A -> B.
				if (b > 0) vectorB[b - 1] += I_val;
			}
		}

		// Including voltage sources in Matrix A and Vector B (KVL).

		int sourceIndex = 0;
		for (const auto& e : circuitComponents) {
			if (e->getName()[0] != 'E' && e->getName()[0] != 'V') { continue; }; //if voltage source
			int a = e->getNodeA();
			int b = e->getNodeB();
			complex<double> V_val = e->getValue();
			int rowIdx = maxNode + sourceIndex; //Admittance part ends in Maxnode, the rest is voltage effect.
			if (a > 0) matrixA[a - 1][rowIdx] += 1.0;  //Only 1 and -1
			if (b > 0) matrixA[b - 1][rowIdx] -= 1.0;
															//Adds branch current as a new variable and implements the KVL equation (Va - Vb = Vsource).

			if (a > 0) matrixA[rowIdx][a - 1] += 1.0;
			if (b > 0) matrixA[rowIdx][b - 1] -= 1.0;

			vectorB[rowIdx] = V_val;

			sourceIndex++;
		}

	}

	void printMatrix() {
		int n = matrixA.size();
		cout << "--- Macierz A | Wektor B ---" << endl;

		for (int i = 0; i < n; i++) {
			//Printing matrixA
			for (int j = 0; j < n; j++) {
				cout << matrixA[i][j] << " ";
			}

			// Printing Vector B
			cout << "| " << vectorB[i] << endl;
		}
	}
	void printNetlist() {
		cout << "--- NETLISTA (STRUKTURA OBWODU) ---" << endl;
		for (const auto& e : circuitComponents) {
			e->elementInfo();                               //Netlist 
		}
		cout << "-----------------------------------" << endl;
	}

	vector<complex<double>> solve() {
		createMatrix();
		int n = matrixA.size();
		if (n == 0) return {};

		// Gauss Algorithm
		//Forward elimination - transforms matrix A into upper triangular form.
		for (int i = 0; i < n; i++) { //Diagonal elements
			for (int k = i + 1; k < n; k++) { //rows below diagonal elements
				complex<double> factor = matrixA[k][i] / matrixA[i][i]; 
				for (int j = i; j < n; j++) {
					matrixA[k][j] -= factor * matrixA[i][j]; 
				}
				vectorB[k] -= factor * vectorB[i];  //And now the last equation has only one unknown
			}
		}
		
		// Back substitution - solves for unknowns starting from the last row.
		vector<complex<double>> x(n);
		for (int i = n - 1; i >= 0; i--) {
			complex<double> sum = 0;
			for (int j = i + 1; j < n; j++) {
				sum += matrixA[i][j] * x[j];
			}
			x[i] = (vectorB[i] - sum) / matrixA[i][i];
		}

		return x; //x is a vector with Nodal Voltages V1,V2 etc.

	}

	complex<double> getCurrent(shared_ptr<Element> e, const vector<complex<double>>& x) {
		int a = e->getNodeA();
		int b = e->getNodeB();

		complex<double> Va = (a > 0) ? x[a - 1] : 0.0;
		complex<double> Vb = (b > 0) ? x[b - 1] : 0.0;


		return (Va - Vb) / e->getImpedance(this->omega);  // I=V/Z, this->omega: ensures impedance is calculated for the current simulation pulsation.
	}
	complex<double> getVoltage(shared_ptr<Element> e, const vector<complex<double>>& x) {
		int a = e->getNodeA();
		int b = e->getNodeB();

		complex<double> Va = (a > 0) ? x[a - 1] : 0.0;
		complex<double> Vb = (b > 0) ? x[b - 1] : 0.0;


		return (Va - Vb); // Node-to-node voltage difference, voltage drop across the component
	}

	void printResults(const vector<complex<double>>& x) {  //reference to solution vector x
		cout << "\n--- CIRCUIT ANALYSIS REPORT ---" << endl;

		//Nodal Voltages
		for (int i = 0; i < maxNode; i++) {
			double amplitude = abs(x[i]);  //amplitude
			double phase = arg(x[i]) * 180.0 / 3.14159265; //phase converted to [deg]

			cout << "Node " << (i + 1) << ": "
				<< amplitude << " V, "							//printing results
				<< "Phase: " << phase << " deg" << endl;
		}

		// Voltage source currant
		for (size_t i = maxNode; i < x.size(); i++) {
			double amplitude = abs(x[i]);
			double phase = arg(x[i]) * 180.0 / 3.14159265;

			std::cout << "Source Current " << (i - maxNode + 1) << ": "
				<< amplitude << " A, "								//printing results
				<< "Phase: " << phase << " deg" << endl;
		}
	}

	void printFullReport(const vector<complex<double>>& x) {
		cout << "\n--- FULL CIRCUIT ANALYSIS REPORT ---" << endl;

		//  Nodal Voltages
		printResults(x);

		// Branch Parameters for each element
		cout << "\n--- COMPONENT PARAMETERS ---" << endl;
		for (const auto& e : circuitComponents) {
			complex<double> U = getVoltage(e, x);
			complex<double> I = getCurrent(e, x);
			cout << "Element: " << e->getName() << "\t| "
				<< "U = " << abs(U) << "V, phase: " << arg(U) * 180 / 3.14 << " deg\t| "
				<< "I = " << abs(I) << "A, phase: " << arg(I) * 180 / 3.14 << " deg" << endl;
		}
	}


};