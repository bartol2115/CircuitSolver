#include <iostream>
#include <string>
#include <complex>
#include <vector>
#include "Element.h"
#include "PasiveElements.h"
#include "Sources.h"
#include "Solver.h"


using namespace std;


void printHeader(string text) {
    cout << "\n================================" << endl;
    cout << "   " << text << endl;                             //additional function for header style
    cout << "================================" << endl;
}

template <typename T>
T getInput(string prompt) {  //function prevents from invalid input
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "[Error] Invalid input. Please enter a number." << endl;
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear() resets the error, ignore() throws away the bad input
    }
}

int main() {
    while (true) {
        printHeader("SIMPLE CIRCUIT SOLVER");
        cout << "1. Build new circuit\n2. Exit\n";   //Main manu
        int mainOpt = getInput<int>("Choice: ");

        if (mainOpt == 2) break;
        if (mainOpt != 1) continue;

        double omega = getInput<double>("\nEnter angular frequency (omega) [rad/s]: ");
        Solver MyCircuit(omega);

        while (true) {
            printHeader("CIRCUIT BUILDER (omega=" + to_string(omega) + ")");  //submenu
            cout << "1. Resistor    2. Capacitor    3. Inductor\n"
                << "4. Volt Source 5. Curr Source  6. Show Netlist\n"
                << "7. SOLVE        8. BACK\n";

            int opt = getInput<int>("Choice: ");

            if (opt == 8) break;
            if (opt == 6) { MyCircuit.printNetlist(); continue; }
            if (opt == 7) {
                MyCircuit.createMatrix();
                auto res = MyCircuit.solve();
                MyCircuit.printFullReport(res);
                continue;
            }

            if (opt >= 1 && opt <= 5) {
                string name;
                cout << "Name: "; cin >> name;
                double val = getInput<double>("Value: ");
                int nA = getInput<int>("Node A: ");
                int nB = getInput<int>("Node B: ");
                double phase = (opt == 4 || opt == 5) ? getInput<double>("Phase [deg]: ") : 0.0;  //if we are adding any kind of source, we need to input phase

                switch (opt) {
                case 1: MyCircuit.addElement(make_shared<Resistor>(name, val, nA, nB)); break;
                case 2: MyCircuit.addElement(make_shared<Capacitor>(name, val, nA, nB)); break;
                case 3: MyCircuit.addElement(make_shared<Inductor>(name, val, nA, nB)); break;           //Element instantiation based on user choice.
                case 4: MyCircuit.addElement(make_shared<VoltageSource>(name, val, phase, nA, nB)); break;
                case 5: MyCircuit.addElement(make_shared<CurrentSource>(name, val, phase, nA, nB)); break;
                }
                cout << "\n>> Element added successfully." << endl;
            }
        }
    }
    //Przykładowy obwód gotowy:
//Solver mojObwod(2);
//
//mojObwod.addElement(make_shared<VoltageSource>("V1", 5.0, 6.0, 1, 0));
//mojObwod.addElement(make_shared<VoltageSource>("V2", 10.0,7.0, 2, 3));
//mojObwod.addElement(make_shared<CurrentSource>("I1", 1.0,10,4, 1));
//mojObwod.addElement(make_shared<Resistor>("R1", 8.0, 1, 2));
//mojObwod.addElement(make_shared<Resistor>("R2",7.0,2, 0));
//mojObwod.addElement(make_shared<Condensator>("C2", 2.0, 2, 3));
//mojObwod.addElement(make_shared<Inductor>("L1", 3.0, 3, 4));
//mojObwod.addElement(make_shared<Resistor>("R3", 20.0, 4, 1));
//

//mojObwod.createMatrix();
//mojObwod.printNetlist();

//auto wynik = mojObwod.solve();
//mojObwod.printFullReport(wynik);
    return 0;
}
