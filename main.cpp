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
double generateMultipleStarEccentricity(default_random_engine & e, double separation);



/* MAIN */
int main () {
	cout << "Hello!\n\n";
	cout << "Welcome to GenSystem Version 0.1!" << endl;
	cout << "(c) 2024 Giancarlo Whitaker" << endl << endl;

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
	StarSystem mainSystem;
	mainSystem.SetSingleStar(starA);

	// If the star is multiple, determine components
	if (isMultiple) {
		int multiplicity =  generateSystemMultiplicity(engine);
		cout << "multiplicity: " << multiplicity << endl << endl;
		multiplicity = 4; // for testing

		if (multiplicity == 2) {
			double massRatio = generateMassRatio(engine);
			cout << "Mass ratio: " << massRatio << endl;
			cout << "B mass: " << baseMass * massRatio << endl << endl;

			Star starB(baseMass * massRatio);

			double separation = generateDistanceBetweenStars(engine, baseMass);
			cout << "Separation: " << separation << " AU\n\n";

			mainSystem.SetSingleStar(starB);
			mainSystem.SetSeparation(separation);
		}
		else if (multiplicity == 3) {
			// flip coin; if heads, C orbits AB, else BC orbits A
			bool systemArrangement = flipCoin(engine);
			cout << "systemArrangement: " << systemArrangement << endl;
			systemArrangement = 0; // for testing

			// C orbits close pair AB
			if (systemArrangement) {
				double massRatioAB = generateHeavyMassRatio(engine);
				double massRatioAC = generateMassRatio(engine);

				Star starB(baseMass * massRatioAB);
				Star starC(baseMass * massRatioAC);

				mainSystem.SetSingleStar(starB);

				StarSystem farSystem;
				farSystem.SetSingleStar(starC);

				double separationAB =  generateDistanceBetweenStars(engine, baseMass);
				mainSystem.SetSeparation(separationAB);
			cout << "Separation, AB: " << separationAB << " AU\n";
				double eccenAB = generateMultipleStarEccentricity(engine, separationAB);
			cout << "Eccentricity, AB: " << eccenAB << "\n\n";

				// Set separation of (AB)C
				double separationABC =  generateDistanceBetweenStars(engine, baseMass);
				while (separationABC < 3 * (separationAB * (1 + eccenAB))) {
					cout << "Looping.../n";
					separationABC =  generateDistanceBetweenStars(engine, baseMass);
				}
				cout << "Separation, (AB)C: " << separationABC << " AU\n";
				double eccenABC = generateMultipleStarEccentricity(engine, separationABC);
			cout << "Eccentricity, (AB)C: " << eccenABC << "\n\n";
			}
			// A orbits close pair BC
			else {
				double massRatioAB = generateMassRatio(engine);
				double massRatioBC = generateHeavyMassRatio(engine);

				Star starB(baseMass * massRatioAB);
				Star starC(baseMass * massRatioAB * massRatioBC);

				StarSystem farSystem;
				farSystem.SetSingleStar(starB);
				farSystem.SetSingleStar(starC);

				double separationBC =  generateDistanceBetweenStars(engine, baseMass * massRatioAB);
				farSystem.SetSeparation(separationBC);
				cout << "Separation, BC: " << separationBC << " AU\n\n";
				double eccenBC = generateMultipleStarEccentricity(engine, separationBC);
				cout << "Eccentricity, BC: " << eccenBC << "\n\n";

				// Set separation of A(BC)
				double separationABC =  generateDistanceBetweenStars(engine, baseMass);
				while (separationABC < 3 * (separationBC * (1 + eccenBC))) {
					cout << "Looping.../n";
					separationABC =  generateDistanceBetweenStars(engine, baseMass);
				}
				cout << "Separation, A(BC): " << separationABC << " AU\n\n";
				double eccenABC = generateMultipleStarEccentricity(engine, separationABC);
				cout << "Eccentricity, A(BC): " << eccenABC << "\n\n";
			}
		} // close trinary
		else { // quaternary
			double massRatioAB = generateHeavyMassRatio(engine);
			Star starB(baseMass * massRatioAB);
			//mainSystem.firstSystem.SetSingleStar(starA);
			//mainSystem.firstSystem.SetSingleStar(starB);

			double separationAB =  generateDistanceBetweenStars(engine, baseMass);
			cout << "Separation, AB: " << separationAB << " AU\n";
				double eccenAB = generateMultipleStarEccentricity(engine, separationAB);
			cout << "Eccentricity, AB: " << eccenAB << "\n\n";

			double massRatioAC = generateMassRatio(engine);
			Star starC(baseMass * massRatioAC);
			//mainSystem.secondSystem.SetSingleStar(starC);

			double massRatioCD = generateHeavyMassRatio(engine);
			Star starD(baseMass * massRatioAC * massRatioCD);
			//mainSystem.secondSystem.SetSingleStar(starD);

			double separationCD =  generateDistanceBetweenStars(engine, baseMass * massRatioAC);
			cout << "Separation, CD: " << separationCD << " AU\n";
			double eccenCD = generateMultipleStarEccentricity(engine, separationCD);
			cout << "Eccentricity, CD: " << eccenCD << "\n\n";

			double maxAB = separationAB * (1 + eccenAB);
			double maxCD = separationCD * (1 + eccenCD);

			double maxSep;
			if (maxAB > maxCD) { maxSep = maxAB; }
			else { maxSep = maxCD; }

			double separationABCD = generateDistanceBetweenStars(engine, baseMass + baseMass * massRatioAC);
				while (separationABCD < 3 * maxSep) {
					cout << "Looping...\n";
					separationABCD =  generateDistanceBetweenStars(engine, baseMass + baseMass * massRatioAC);
				}
			cout << "Separation, (AB)(CD): "  << separationABCD << " AU\n";
			double eccenABCD = generateMultipleStarEccentricity(engine, separationABCD);
			cout << "Eccentricity, (AB)(CD): " << eccenABCD << "\n\n";
		}
	} // END IS_MULTIPLE

	/* cout << "ALL_SYSTEMS:\n";
	vector<Star> x = mainSystem.firstSystem.GetStars();
	cout << "assigned\n";
	for (int i = 0; i < 2; i++) {
		cout << x[i].GetMass() << endl;
	}
	x = mainSystem.secondSystem.GetStars();
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

double generateMultipleStarEccentricity(default_random_engine & e, double separation) {
	if (separation <= 0.2) { // ~20 days, M = 1.4 combined
		return 0.0;
	}
	else {
		normal_distribution<> generator(0.4, 0.1);
		double eccentricity = generator(e);
		cout << "eccentricity: " << eccentricity << endl;
		if (eccentricity < 0) { return 0.0; }
		else if (eccentricity > 0.9) { return 0.9; }
		else { return eccentricity; }
	}
}
