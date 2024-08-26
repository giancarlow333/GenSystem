#include <iostream>
#include <random>
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"
using namespace std;

// Function declarations
double initialMassFunction (default_random_engine & e);
bool isSystemMultiple (double mass, default_random_engine & e);
int generateSystemMultiplicity(default_random_engine & e);
double generateHeavyMassRatio(default_random_engine & e);
double generateMassRatio(default_random_engine & e);
bool flipCoin(default_random_engine & e);
double generateDistanceBetweenStars(default_random_engine & e, double primaryMass);

// struct for trinary and quaternary systems
struct MultipleSystem {
	StarSystem firstSystem;
	StarSystem secondSystem;
};

/* MAIN */
int main () {
	cout << "Hello!\n\n";
	cout << "Welcome to GenSystem Version 0.1!" << endl;
	cout << "(c) 2024 Giancarlo Whitaker" << endl << endl;

	//
	MultipleSystem allSystems;

	// construct random engine
	int seed = 333;  // non-random seed for testing
	default_random_engine engine(seed);

	// mass of the primary star
	double baseMass = initialMassFunction(engine);

	cout << "The base mass is: " << baseMass << endl << endl;

	bool isMultiple = isSystemMultiple(baseMass, engine);

	cout << "The system ";
	if (isMultiple) { cout << "IS"; }
	else { cout << "IS NOT"; }
	cout << " multiple!" << endl << endl;

	isMultiple = true; // For testing

	// Create star
	Star starA;
	starA.SetMass(baseMass);

	// Create star system
	allSystems.firstSystem.SetSingleStar(starA);

	// If the star is multiple, determine components
	if (isMultiple) {
		int multiplicity =  generateSystemMultiplicity(engine);
		cout << "multiplicity: " << multiplicity << endl << endl;
		// multiplicity = 4; // for testing

		if (multiplicity == 2) {
			double massRatio = generateMassRatio(engine);
			cout << "Mass ratio: " << massRatio << endl;
			cout << "B mass: " << baseMass * massRatio << endl << endl;

			Star starB(baseMass * massRatio);

			double separation = generateDistanceBetweenStars(engine, baseMass);
			cout << "Separation: " << separation << " AU\n\n";

			allSystems.firstSystem.SetSingleStar(starB);
			allSystems.firstSystem.SetSeparation(separation);
		}
		else if (multiplicity == 3) {
			// flip coin; if heads, C orbits AB, else BC orbits A
			bool systemArrangement = flipCoin(engine);
			cout << "systemArrangement: " << systemArrangement << endl;

			// C orbits close pair AB
			if (systemArrangement) {
				double massRatioAB = generateHeavyMassRatio(engine);
				double massRatioAC = generateMassRatio(engine);

				Star starB(baseMass * massRatioAB);
				Star starC(baseMass * massRatioAC);

				//allSystems.firstSystem.SetSingleStar(starB);
				//vector<Star> theStars = allSystems.firstSystem.GetStars();
				//allSystems.firstSystem.SetStars(theStars);
				//allSystems.secondSystem.SetSingleStar(starC);
			}
			// A orbits close pair BC
			else {
				double massRatioAB = generateMassRatio(engine);
				double massRatioBC = generateHeavyMassRatio(engine);

				Star starB(baseMass * massRatioAB);
				Star starC(baseMass * massRatioAB * massRatioBC);
				//allSystems.firstSystem.SetSingleStar(starA);
				//allSystems.secondSystem.SetSingleStar(starB);
				//allSystems.secondSystem.SetSingleStar(starC);
			}
		} // close trinary
		else { // quaternary
			double massRatioAB = generateHeavyMassRatio(engine);
			Star starB(baseMass * massRatioAB);
			//allSystems.firstSystem.SetSingleStar(starA);
			//allSystems.firstSystem.SetSingleStar(starB);

			double massRatioAC = generateMassRatio(engine);
			Star starC(baseMass * massRatioAC);
			//allSystems.secondSystem.SetSingleStar(starC);

			double massRatioCD = generateHeavyMassRatio(engine);
			Star starD(baseMass * massRatioAC * massRatioCD);
			//allSystems.secondSystem.SetSingleStar(starD);
		}
	}

	/* cout << "ALL_SYSTEMS:\n";
	vector<Star> x = allSystems.firstSystem.GetStars();
	cout << "assigned\n";
	for (int i = 0; i < 2; i++) {
		cout << x[i].GetMass() << endl;
	}
	x = allSystems.secondSystem.GetStars();
	for (int i = 0; i < 2; i++) {
		cout << x[i].GetMass() << endl;
	} */

	//for (int i = 0; i < 2000; i++)
		//cout << generateDistanceBetweenStars(engine, 0.3) << endl;

	return 0;
}

/*
 * FUNCTION DEFINITIONS
 */

/* initialMassFunction
 * This generates the *base* mass of the primary star
 * This is based on Kroupa (2002)
 * A random "basis" number is generated, based on the piecewise integral for the cumulative
 * distribution function for mass.  This number is a real number between 0 and 4.7511, the
 * area under the curve for masses between 0.01 and 2.18 solar masses (the latter being class
 * A0 V).  This basis is used to determine which mass category the star falls into, i.e.
 * which sub-equation to use.  A "second" real number is generated on a uniform distribution,
 * and this produces the mass of the star.
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


/* isSystemMultiple
 * Randomly determines if the system is multiple
 * This is based on Duchene & Kraus (2013)
 * TK
 */
bool isSystemMultiple (double mass, default_random_engine & e) {
	uniform_real_distribution<> rUnif(0, 1);
	double randomU = rUnif(e);
	cout << "randomU: " << randomU << endl;

	bool isMult = false;

	if (mass <= 0.1) {
		if (randomU <= 0.22) { isMult = true; };
	}
	else if (mass <= 0.5) {
		if (randomU <= 0.26) { isMult = true; };
	}
	else if (mass <= 1.3) {
		if (randomU <= 0.44) { isMult = true; };
	}
	else {
		if (randomU <= 0.55) { isMult = true; };
	}

	return isMult;
}


/* generateSystemMultiplicity
 * If the system generated is multiple, determine how many stars it consists of.
 * Currently only binary, trinary, and quaternary systems are supported.
 * This is based on TK
 * TK
 */
int generateSystemMultiplicity(default_random_engine & e) {
	uniform_real_distribution<> rUnif(0, 1);
	double randomU = rUnif(e);
	cout << "randomU: " << randomU << endl;

	if (randomU <= 0.75) { return 2; }
	else if (randomU <= 0.95) { return 3; }
	else { return 4; }
}

double generateMassRatio(default_random_engine & e) {
	uniform_real_distribution<> rUnif(0.05, 1);
	double randomU = rUnif(e);
	cout << "randomU: " << randomU << endl;

	return randomU;
}

double generateHeavyMassRatio(default_random_engine & e) {
	uniform_real_distribution<> rUnif(0.35, 1);
	double randomU = rUnif(e);
	cout << "randomU: " << randomU << endl;

	return randomU;
}

bool flipCoin(default_random_engine & e) {
	discrete_distribution<int> coinFlipper(0, 1);

	return coinFlipper(e);
}

// I can't make sense of the paper
// The paper makes me think that 45 AU is the *mode*, in which case the mean is ln(45 - sigma^2) or 3.68.  Keeping the stdev of 2.3 seems to match the upper end of the distro (4.6% above 1500 AU), but not the lower end!
double generateDistanceBetweenStars(default_random_engine & e, double primaryMass) {
	if (primaryMass <= 0.1) {
		lognormal_distribution<> generator(1.45, 0.5);
		return generator(e);
	}
	else if (primaryMass <= 0.5) {
		lognormal_distribution<> generator(1.28, 1.3);
		return generator(e);
	}
	else {
		lognormal_distribution<> generator(3.68, 2.3);
		return generator(e);
	}
}
