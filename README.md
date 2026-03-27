# CircuitSolver

## Project Description
A C++ console application designed for Direct and Alternating Current (DC) circuit analysis. The program calculates parameters for various passive elements and power sources based on fundamental electrical engineering principles.

## Features
* Calculation of currents and voltages in circuit branches/elements.
* Support for passive elements (resistors, capacitors and inductors) and energy sources (voltage(AC/DC)/current(AC/DC)).
* Implementation of Modified Nodal Analysis with Gauss's elimination method.

## Technical Stack
* **Language:** C++
* **IDE:** Microsoft Visual Studio
* **Standard:** C++17 / C++20

## How to Run
1. Download the `CircuitSolver.exe` file from the Repository
2. Run the file using the Windows Command Prompt (CMD) or PowerShell.

## How to Use (Principles)
1. While choosing nodes ensure the circuit is closed
2. While choosing nodes for voltage source the first node defines higher potencial
3. While choosing nodes for current source, current flows from node A to node B
