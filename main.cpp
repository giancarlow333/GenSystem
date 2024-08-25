#include <iostream>
#include <random>
using namespace std;

// Function declarations
double initialMassFunction (default_random_engine & e);

/* MAIN */
int main () {
	cout << "Hello!\n\n";
	cout << "Welcome to GenSystem Version 0.01!" << endl;
	cout << "(c) 2024 Giancarlo Whitaker" << endl << endl;

	// construct random engine
	int seed = 333;  // non-random seed for testing
	default_random_engine engine(seed);

	double baseMass = initialMassFunction(engine);

	cout << "The base mass is: " << baseMass << endl << endl;

	return 0;
}

/*
 * FUNCTION DEFINITIONS
 */

double initialMassFunction (default_random_engine & e) {
	uniform_real_distribution<> baseGen(0, 4.7511);
	double basis = baseGen(e);
	cout << "basis: " << basis << endl;
	double mass;
	
	if (basis <= 0.1869) {
		uniform_real_distribution<> secondGen(2.1334, 3.9811);
		double second = secondGen(e);
		cout << "second: " << second << endl;
		mass = pow(second, 1 / -0.3);
	}
	else if (basis <= 3.1944) {
		uniform_real_distribution<> secondGen(2.4623, 26.6675);
		double second = secondGen(e);
		cout << "second: " << second << endl;
		mass = pow(second, 1 / -1.3);
	}
	else if (basis <= 4.3192) {
		uniform_real_distribution<> secondGen(1, 4.9246);
		double second = secondGen(e);
		cout << "second: " << second << endl;
		mass = pow(second, 1 / -2.3);
	}
	else {
		uniform_real_distribution<> secondGen(0.1219, 1);
		double second = secondGen(e);
		cout << "second: " << second << endl;
		mass = pow(second, 1 / -2.7);
	}

	return mass;
}
