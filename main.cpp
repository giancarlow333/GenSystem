#include <iostream>
#include <random>
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"
#include "useful.cpp"
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
double generateSystemAge (default_random_engine & e);
double generateMetallicity (default_random_engine & e, double age);
double getInitialLuminosity (double mass);
double getStellarLifespan (double mass);
double getInitialTemperature (double mass);
double getStellarRadius (double lum, double temp);
void evolveStar (Star & s, default_random_engine & e);
double generateDiskMassFactor (default_random_engine & e);
double generateMigrationFactor (default_random_engine & e, double diskMassFactor);

// constants
const string VERSION_NUMBER = "0.4";

// struct for overall separation
struct OverallSeparation {
	double separation;
	double eccentricity;
};

/* MAIN */
int main () {
	cout << "Hello!\n\n";
	cout << "Welcome to GenSystem Version " << VERSION_NUMBER << "!" << endl;
	cout << "(c) 2024 Giancarlo Whitaker" << endl << endl;

	// construct random engine
	int seed = 333;  // non-random seed for testing
	default_random_engine engine(seed);

	// mass of the primary star
	double baseMass = initialMassFunction(engine);
	//baseMass = 1.2; // for testing

	cout << "The base mass is: " << baseMass << endl << endl;

	bool isMultiple = isSystemMultiple(baseMass, engine);

	cout << "The system ";
	if (isMultiple) { cout << "IS"; }
	else { cout << "IS NOT"; }
	cout << " multiple!" << endl << endl;

	isMultiple = false; // For testing

	// Create star
	Star starA, starB, starC, starD;
	starA.SetMass(baseMass);

	// Create star systems
	StarSystem mainSystem, farSystem; // for trinaries and quaternaries
	OverallSeparation overallSeparation;

	// If the star is multiple, determine components
	int multiplicity;
	if (isMultiple) {
		multiplicity = generateSystemMultiplicity(engine);
		cout << "multiplicity: " << multiplicity << endl << endl;
		multiplicity = 2; // for testing

		if (multiplicity == 2) {
			double massRatio = generateMassRatio(engine);
			cout << "Mass ratio: " << massRatio << endl;
			cout << "B mass: " << baseMass * massRatio << endl << endl;

			starB.SetMass(baseMass * massRatio);

			double separation = generateDistanceBetweenStars(engine, baseMass);
			cout << "Separation: " << separation << " AU\n\n";
			double eccen = generateMultipleStarEccentricity(engine, separation);
			cout << "Eccentricity: " << eccen << "\n\n";

			mainSystem.SetPrimaryStar(starA);
			mainSystem.SetSecondaryStar(starB);
			mainSystem.SetSeparation(separation);
			mainSystem.SetEccentricity(eccen);
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

				starB.SetMass(baseMass * massRatioAB);
				starC.SetMass(baseMass * massRatioAC);

				mainSystem.SetSecondaryStar(starB);

				double separationAB = generateDistanceBetweenStars(engine, baseMass);
				mainSystem.SetSeparation(separationAB);
				cout << "Separation, AB: " << separationAB << " AU\n";
				double eccenAB = generateMultipleStarEccentricity(engine, separationAB);
				cout << "Eccentricity, AB: " << eccenAB << "\n\n";
				mainSystem.SetEccentricity(eccenAB);

				// Set separation of (AB)C
				double separationABC =  generateDistanceBetweenStars(engine, baseMass);
				while (separationABC < 3 * (separationAB * (1 + eccenAB))) {
					cout << "Looping.../n";
					separationABC =  generateDistanceBetweenStars(engine, baseMass);
				}
				cout << "Separation, (AB)C: " << separationABC << " AU\n";
				overallSeparation.separation = separationABC;
				double eccenABC = generateMultipleStarEccentricity(engine, separationABC);
				cout << "Eccentricity, (AB)C: " << eccenABC << "\n\n";
				overallSeparation.eccentricity = eccenABC;
			}
			// A orbits close pair BC
			else {
				double massRatioAB = generateMassRatio(engine);
				double massRatioBC = generateHeavyMassRatio(engine);

				starB.SetMass(baseMass * massRatioAB);
				starC.SetMass(baseMass * massRatioAB * massRatioBC);

				farSystem.SetPrimaryStar(starB);
				farSystem.SetSecondaryStar(starC);

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
				overallSeparation.separation = separationABC;
				double eccenABC = generateMultipleStarEccentricity(engine, separationABC);
				cout << "Eccentricity, A(BC): " << eccenABC << "\n\n";
				overallSeparation.eccentricity = eccenABC;
			}
		} // close trinary
		else { // quaternary
			double massRatioAB = generateHeavyMassRatio(engine);
			starB.SetMass(baseMass * massRatioAB);

			double separationAB =  generateDistanceBetweenStars(engine, baseMass);
			cout << "Separation, AB: " << separationAB << " AU\n";
			double eccenAB = generateMultipleStarEccentricity(engine, separationAB);
			cout << "Eccentricity, AB: " << eccenAB << "\n\n";

			double massRatioAC = generateMassRatio(engine);
			starC.SetMass(baseMass * massRatioAC);
			//mainSystem.secondSystem.SetSingleStar(starC);

			double massRatioCD = generateHeavyMassRatio(engine);
			starD.SetMass(baseMass * massRatioAC * massRatioCD);
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

	// Age, Metallicity, Luminosity, Lifespan
	double systemAge = generateSystemAge(engine);
	//systemAge = 6.5; // for testing
	cout << "Age: " << systemAge << " Ga" << endl << endl;
	double metallicity = generateMetallicity(engine, systemAge);
	cout << "Metallicity: " << metallicity << " x Sol" << endl << endl;
	starA.SetAge(systemAge);
	starA.SetMetallicity(metallicity);

	// evolve Star A
	evolveStar(starA, engine);

	cout << "A Luminosity: " << starA.GetLuminosity() << endl;
	cout << "A Temperature: " << starA.GetTemperature() << endl;
	cout << "A Radius: " << starA.GetRadius() << endl;
	cout << "A Type: " << starA.GetSpectralType() << " " << starA.GetLuminosityClass() << endl;
	if (multiplicity == 2) {
		starB.SetAge(systemAge);
		starB.SetMetallicity(metallicity);

		evolveStar(starB, engine);

		cout << "B Luminosity: " << starB.GetLuminosity() << endl;
		cout << "B Temperature: " << starB.GetTemperature() << endl;
		cout << "B Radius: " << starB.GetRadius() << endl;
		cout << "B Type: " << starB.GetSpectralType() << " " << starB.GetLuminosityClass() << endl;

		mainSystem.SetPrimaryStar(starA);
		mainSystem.SetSecondaryStar(starB);
	}
	else if (multiplicity == 3) {
		cout << "Not yet implemented (1)!\n\n";
	}
	else {
		cout << "Not yet implemented (2)!\n\n";
	}

	// PLANETARY DISK FOR STAR A

	double diskMassFactor = generateDiskMassFactor(engine);
	cout << "diskMassFactor: " << diskMassFactor << endl;
	double migrationFactor = generateMigrationFactor(engine, diskMassFactor);
	cout << "migrationFactor: " << migrationFactor << endl << endl;

	double diskInnerEdge = 0.005 * pow(starA.GetMass(), 1 / 3);
	double formationIceLine = 4.0 * sqrt(getInitialLuminosity(starA.GetMass()));
	double slowAccretionLine = 20 * pow(starA.GetMass(), 1 / 3);
	cout << "diskInnerEdge: " << diskInnerEdge << endl;
	cout << "formationIceLine: " << formationIceLine << endl;
	cout << "slowAccretionLine: " << slowAccretionLine << endl << endl;

	// put forbidden zones here

	double innerFormationZone = 2.5 * starA.GetMass() * starA.GetMetallicity() * diskMassFactor;
	double middleFormationZone = 80.0 * starA.GetMass() * starA.GetMetallicity() * diskMassFactor;
	double outerFormationZone = 18.0 * starA.GetMass() * starA.GetMetallicity() * diskMassFactor;
	cout << "innerFormationZone: " << innerFormationZone << endl;
	cout << "middleFormationZone: " << middleFormationZone << endl;
	cout << "outerFormationZone: " << outerFormationZone << endl;

	// determine if planets orbit A directly here


	vector<Planet> starAPlanets;

	// place inner planets
	double planet0Distance = 0.6 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp0(planet0Distance, 0.08 * innerFormationZone);
	starAPlanets.push_back(temp0);

	double planet1Distance = 0.8 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp1(planet1Distance, 0.4 * innerFormationZone);
	starAPlanets.push_back(temp1);

	double planet2Distance = 1.2 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp2(planet2Distance, 0.4 * innerFormationZone);
	starAPlanets.push_back(temp2);

	double planet3Distance = 1.8 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp3(planet3Distance, 0.08 * innerFormationZone);
	starAPlanets.push_back(temp3);
	
	double planet4Distance = 2.7 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp4(planet4Distance, 0.04 * innerFormationZone);
	starAPlanets.push_back(temp4);

	// place middle planets
	double planet5Distance = 4.0 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp5(planet5Distance, 0.4 * middleFormationZone);
	starAPlanets.push_back(temp5);

	double planet6Distance = 6.0 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp6(planet6Distance, 0.25 * middleFormationZone);
	starAPlanets.push_back(temp6);

	double planet7Distance = 9.0 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp7(planet7Distance, 0.175 * middleFormationZone);
	starAPlanets.push_back(temp7);

	double planet8Distance = 13.5 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp8(planet8Distance, 0.175 * middleFormationZone);
	starAPlanets.push_back(temp8);

	// place outer planets
	double planet9Distance = 20.0 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp9(planet9Distance, 0.6 * outerFormationZone);
	starAPlanets.push_back(temp9);

	double planet10Distance = 30.0 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp10(planet10Distance, 0.3 * outerFormationZone);
	starAPlanets.push_back(temp10);

	double planet11Distance = 45.0 * sqrt(getInitialLuminosity(starA.GetMass()));
	Planet temp11(planet11Distance, 0.1 * outerFormationZone);
	starAPlanets.push_back(temp11);

	// print for testing
	for (int i = 0; i < starAPlanets.size(); i++) {
		cout << i << ": " << starAPlanets[i].GetDistance() << " AU; mass " << starAPlanets[i].GetMass() << endl;
	}

	// Outer Planetary System
	for (int i = 5; i < 12; i++) {
		double planetesimalMass = starAPlanets[i].GetMass();
	}












	// work exclusion zones here
	vector<Planet> starAPlanets_excluded;
	for (int i = 0; i < starAPlanets.size(); i++) {
		double distance = starAPlanets[i].GetDistance();
		if (distance < diskInnerEdge || distance > slowAccretionLine) {
			cout << "Planet " << i << " is out of bounds!\n";
		}
		else {
			starAPlanets_excluded.push_back(starAPlanets[i]);
		}
	}

	// print for testing
	cout << "Modified:\n\n";
	for (int i = 0; i < starAPlanets_excluded.size(); i++) {
		cout << i << ": " << starAPlanets_excluded[i].GetDistance() << " AU; mass " << starAPlanets_excluded[i].GetMass() << endl;
	}

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
	double mass;
	
	if (basis <= 0.1869) {
		uniform_real_distribution<> secondGen(2.1334, 3.9811);
		double second = secondGen(e);
		mass = pow(second, 1 / -0.3);
	}
	else if (basis <= 3.1944) {
		uniform_real_distribution<> secondGen(2.4623, 26.6675);
		double second = secondGen(e);
		mass = pow(second, 1 / -1.3);
	}
	else if (basis <= 4.3192) {
		uniform_real_distribution<> secondGen(1, 4.9246);
		double second = secondGen(e);
		mass = pow(second, 1 / -2.3);
	}
	else {
		uniform_real_distribution<> secondGen(0.1219, 1);
		double second = secondGen(e);
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

	if (randomU <= 0.75) { return 2; }
	else if (randomU <= 0.95) { return 3; }
	else { return 4; }
}

double generateMassRatio(default_random_engine & e) {
	uniform_real_distribution<> rUnif(0.05, 1);
	double randomU = rUnif(e);

	return randomU;
}

double generateHeavyMassRatio(default_random_engine & e) {
	uniform_real_distribution<> rUnif(0.35, 1);
	double randomU = rUnif(e);

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
		if (eccentricity < 0) { return 0.0; }
		else if (eccentricity > 0.9) { return 0.9; }
		else { return eccentricity; }
	}
}

/* generateSystemAge
 * Procedure taken from "Architect of Worlds 0.8"
 */
double generateSystemAge (default_random_engine & e) {
	uniform_int_distribution<> percentileRoll(1, 100);
	uniform_real_distribution<> randU(0, 1);

	int roll = percentileRoll(e);
	double age;

	if (roll <= 5) { // Extreme Population I
		return 0.0 + randU(e) * 0.5;
	}
	else if (roll <= 31) { // Young Population I
		return 0.5 + randU(e) * 2.5;
	}
	else if (roll <= 82) { // Intermediate Population I
		return 3.0 + randU(e) * 5.0;
	}
	else if (roll <= 97) { // Disk Population
		return 8.0 + randU(e) * 1.5;
	}
	else if (roll <= 99) { // Intermediate Population II
		return 9.5 + randU(e) * 2.5;
	}
	else { // Extreme Population II
		return 12.0 + randU(e) * 1.5;
	}
}

/* generateMetallicity
 * Procedure taken from "Architect of Worlds 0.8"
 */
double generateMetallicity (default_random_engine & e, double age) {
	uniform_int_distribution<> diceRoll(1, 6);

	int roll = diceRoll(e) + diceRoll(e) + diceRoll(e);
	
	return (roll / 10) * (1.2 - age / 13.5);
}

/* getInitialLuminosity
 * Get the luminosity the star had at formation
 * I derived this equation by doing two regressions on the table found in
 * "Architect of Worlds 0.8," which is ultimately from Mamajek (2016) and
 * Townsend (2016).
 */
double getInitialLuminosity (double mass) {
	if (mass < 0.5) {
		return 0.2106 * pow(mass, 2.3357);
	}
	else {
		return 0.7329 * pow(mass, 4.6128);
	}
}

/* getStellarLifespan
 */
double getStellarLifespan (double mass) {
	if (mass < 0.43) {
		return 43.0 * pow(mass, -1.3);
	}
	else if (mass < 2) {
		return 10.0 * pow(mass, -3.0);
	}
	else {
		return 7.1 * pow(mass, -2.5);
	}
}

/* getInitialTemperature
 * Get the temperature the star had at formation
 * I derived this equation by doing multiple regressions on the table found in
 * "Architect of Worlds 0.8," which is ultimately from Mamajek (2016) and
 * Townsend (2016).  This equation is very ugly but it avoid a table at least!
 */
double getInitialTemperature (double mass) {
	if (mass < 0.5) {
		return 4335.95 + 684.72 * log (mass);
	}
	else if (mass < 1.3) {
		double denominator = 1 + 4.2427 * exp(-2.914 * mass);
		return 7007.56 / denominator;
	}
	else {
		return 2857.576 * mass + 2537.984;
	}
}

/* getStellarRadius
 */
double getStellarRadius (double lum, double temp) {
	return pow(lum, 0.5) / pow(temp / 5772.0, 2.0);
}

/* evolveStar
 * When invoked, age and mass need to have been calculated!
 */
void evolveStar (Star & s, default_random_engine & e) {
	double systemAge = s.GetAge();
	double starMass = s.GetMass();

	if (starMass < 0.08) { // it's a brown dwarf
		double upper = pow(starMass, 0.83);
		double lower = pow(systemAge, 0.32);
		double temp = 18600 * upper / lower;
		s.SetTemperature(temp);

		s.SetLuminosity(pow(temp, 4.0) / 1.1e17);

		s.SetSpectralType(GetSpectralClass(s.GetTemperature()));
		s.SetLuminosityClass("V");

		return;
	}
	
	double lifespan = getStellarLifespan(starMass);

	if (systemAge <= lifespan) { // main sequence
		double initLum = getInitialLuminosity(starMass);
		double lum = initLum * pow(2.2, systemAge / lifespan);
		s.SetLuminosity(lum);

		double temp = getInitialTemperature(starMass);
		s.SetTemperature(temp);

		double radius = getStellarRadius(lum, temp);
		s.SetRadius(radius);

		s.SetSpectralType(GetSpectralClass(temp));
		s.SetLuminosityClass("V");
	}
	else if (systemAge <= 1.15 * lifespan) {
		uniform_int_distribution<> diceRoll(1, 100);
		int roll = diceRoll(e);

		if (roll <= 60) { // subgiant
			uniform_real_distribution<> newLumRatio(2.0, 2.4);
			
			double initLum = getInitialLuminosity(starMass);
			s.SetLuminosity(newLumRatio(e) * initLum);

			double initTemp = getInitialTemperature(starMass);
			uniform_real_distribution<> newTemp(5000, initTemp);
			double newTemperature = newTemp(e);
			s.SetTemperature(newTemperature);

			double radius = getStellarRadius(newLumRatio(e) * initLum, newTemperature);
			s.SetRadius(radius);

			s.SetSpectralType(GetSpectralClass(newTemperature));
			s.SetLuminosityClass("IV");
		}
		else if (roll <= 90) { // red giant branch
			uniform_real_distribution<> randomU(0, 1);
			double randomNumber = randomU(e);

			s.SetTemperature(5000 - randomNumber * 2000);
			s.SetLuminosity(pow(50, 1 + randomNumber));
			s.SetRadius(getStellarRadius(s.GetLuminosity(), s.GetTemperature()));

			s.SetSpectralType(GetSpectralClass(s.GetTemperature()));
			s.SetLuminosityClass("III");
		}
		else { // Horizontal branch
			uniform_real_distribution<> randomLum(50, 100);
			s.SetLuminosity(randomLum(e));

			normal_distribution<> randomTemp(5000, 50);
			s.SetTemperature(randomTemp(e));

			s.SetRadius(getStellarRadius(s.GetLuminosity(), s.GetTemperature()));

			s.SetSpectralType(GetSpectralClass(s.GetTemperature()));
			s.SetLuminosityClass("II");
		}
	} // close 1.15 * lifespan
	else { // white dwarf
		double newMass = 0.43 + starMass / 10.4;
		s.SetMass(newMass);

		double postLifespan = systemAge - (1.15 * lifespan);
		double upper = pow(newMass, 0.25);
		double lower = pow(postLifespan, 0.35);
		double temp = 13500 * upper / lower;
		s.SetTemperature(temp);

		double radiusKM = 5500 / pow(newMass, 1 / 3);
		double radius = radiusKM / 695700;
		s.SetRadius(radius);

		s.SetLuminosity(pow(radius, 2) * pow(temp / 5772, 4));

		// Set Spectral Class
		s.SetSpectralType("D");
		s.SetLuminosityClass("WD");
	}

	return;
}

/* generateDiskMassFactor
 * Generates the size of the protoplanetary disk relative to the Sun's
 * Implements algorithm on AOW pp. 37-38
 */
double generateDiskMassFactor (default_random_engine & e) {
	uniform_int_distribution<> diceRoll(1, 6);
	int roll = diceRoll(e) + diceRoll(e) + diceRoll(e);

	if (roll == 3) { return 0.25; }
	else if (roll <= 5) { return 0.36; }
	else if (roll <= 7) { return 0.50; }
	else if (roll <= 9) { return 0.70; }
	else if (roll <= 11) { return 1.00; }
	else if (roll <= 13) { return 1.40; }
	else if (roll <= 15) { return 2.00; }
	else if (roll <= 17) { return 2.80; }
	else { return 4.00; }
}

double generateMigrationFactor (default_random_engine & e, double diskMassFactor) {
	uniform_int_distribution<> diceRoll(1, 6);
	int roll = diceRoll(e) + diceRoll(e) + diceRoll(e);

	if (diskMassFactor >= 2.0) { roll -= 3; }
	if (diskMassFactor < 1.0) { roll += 3; }

	if (roll <= 5) { return 0; }
	else if (roll == 6) { return 0.05; }
	else if (roll == 7) { return 0.1; }
	else if (roll == 8) { return 0.2; }
	else if (roll == 9) { return 0.3; }
	else if (roll == 10) { return 0.4; }
	else if (roll == 11) { return 0.5; }
	else if (roll == 12) { return 0.6; }
	else if (roll == 13) { return 0.7; }
	else if (roll == 14) { return 0.8; }
	else if (roll == 15) { return 0.9; }
	else { return 1.0; } // roll >= 16
}
