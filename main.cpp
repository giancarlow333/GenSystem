#include <iostream>
#include <random>
#include <fstream>          // file output
#include <string>           // file names
#include <filesystem>       // directories
#include <iomanip>          // setprecision
#include <cmath>            // ceil
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"
#include "Moon.h"
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
double getOuterSystemProperties(Planet & p, int mod, int pNumber, default_random_engine & e);
double getInnerOrbitalExclusionZone (double pMass, double sMass, double separation, double eccentricity);
double getOuterOrbitalExclusionZone (double pMass, double sMass, double separation, double eccentricity);
vector<Planet> formPlanets (Star & s, default_random_engine & e, double forbiddenZone, bool starIsCircumbinary, double initialLuminosity, double innerExclusionZone);
void printPlanetaryClass (PlanetClass pc, string & className, string & imgFileName);
double getWaterGreenhouse (double temp, double ocean);

// constants
const string VERSION_NUMBER = "0.13";

// struct for overall separation
struct OverallSeparation {
	double separation;
	double eccentricity;
};

// struct for planet formation
struct FormingPlanet {
	Planet planet;
	bool isDominantGasGiant = false;
	bool inExclusionZone = false;
	bool lastBeforeSlowAccretion = false;
	bool penultBeforeSlowAccretion = false;
	bool orbitDisrupted = false;
	bool triggeredGrandTack = false;
	bool planetEjected = false;
	bool finalPlacement = false;
};
void placeRemainingPlanets (vector<FormingPlanet> & pVector, int firstPlanetIndex, int lastPlanetIndex, int countToBePlaced, default_random_engine & e);

/* MAIN */
int main () {
	cout << "Hello!\n";
	cout << "Welcome to GenSystem Version " << VERSION_NUMBER << "!" << endl;
	cout << "(c) 2024 Giancarlo Whitaker" << endl << endl;

	// construct random engine
	int seed = 333;  // non-random seed for testing
	default_random_engine engine(seed);

	// mass of the primary star
	double baseMass = initialMassFunction(engine);
	baseMass = 1.02; // for testing

	bool isMultiple = isSystemMultiple(baseMass, engine);

	/*cout << "The system ";
	if (isMultiple) { cout << "IS"; }
	else { cout << "IS NOT"; }
	cout << " multiple!" << endl << endl;*/

	//isMultiple = true; // For testing

	// Create star
	Star starA, starB, starC, starD;
	starA.SetMass(baseMass);

	// Create star systems
	StarSystem mainSystem, farSystem; // for trinaries and quaternaries
	OverallSeparation overallSeparation;

	// If the star is multiple, determine components
	int multiplicity = 1;
	bool systemArrangement;
	if (isMultiple) {
		multiplicity = generateSystemMultiplicity(engine);
		//multiplicity = 4; // for testing

		// don't implement quaternary yet
		if (multiplicity == 4) { multiplicity = 3; }

		if (multiplicity == 2) {
			double massRatio = generateMassRatio(engine);

			starB.SetMass(baseMass * massRatio);

			double separation = generateDistanceBetweenStars(engine, baseMass);
			double eccen = generateMultipleStarEccentricity(engine, separation);

			mainSystem.SetPrimaryStar(starA);
			mainSystem.SetSecondaryStar(starB);
			mainSystem.SetSeparation(separation);
			mainSystem.SetEccentricity(eccen);
		}
		else if (multiplicity == 3) {
			// flip coin; if heads, C orbits AB, else BC orbits A
			systemArrangement = flipCoin(engine);
			//systemArrangement = 0; // for testing

			// C orbits close pair AB
			if (systemArrangement) {
				double massRatioAB = generateHeavyMassRatio(engine);
				double massRatioAC = generateMassRatio(engine);

				starB.SetMass(baseMass * massRatioAB);
				starC.SetMass(baseMass * massRatioAC);

				mainSystem.SetSecondaryStar(starB);

				double separationAB = generateDistanceBetweenStars(engine, baseMass);
				mainSystem.SetSeparation(separationAB);
				double eccenAB = generateMultipleStarEccentricity(engine, separationAB);
				mainSystem.SetEccentricity(eccenAB);

				// Set separation of (AB)C
				double exclusionZoneAB = getOuterOrbitalExclusionZone(baseMass, baseMass * massRatioAB, separationAB, eccenAB);
				double separationABC =  generateDistanceBetweenStars(engine, baseMass);
				//separationABC = 25; // for testing
				double eccenABC = generateMultipleStarEccentricity(engine, separationABC);
				while ((1 - eccenABC) * separationABC < exclusionZoneAB) {
					separationABC =  generateDistanceBetweenStars(engine, baseMass);
				}
				overallSeparation.separation = separationABC;
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
				double eccenBC = generateMultipleStarEccentricity(engine, separationBC);

				// Set separation of A(BC)
				double separationABC =  generateDistanceBetweenStars(engine, baseMass);
				while (separationABC < 3 * (separationBC * (1 + eccenBC))) {
					separationABC =  generateDistanceBetweenStars(engine, baseMass);
				}
				overallSeparation.separation = separationABC;
				double eccenABC = generateMultipleStarEccentricity(engine, separationABC);
				overallSeparation.eccentricity = eccenABC;
			}
		} // close trinary
		else { // quaternary
			double massRatioAB = generateHeavyMassRatio(engine);
			starB.SetMass(baseMass * massRatioAB);

			double separationAB =  generateDistanceBetweenStars(engine, baseMass);
			double eccenAB = generateMultipleStarEccentricity(engine, separationAB);

			double massRatioAC = generateMassRatio(engine);
			starC.SetMass(baseMass * massRatioAC);
			//mainSystem.secondSystem.SetSingleStar(starC);

			double massRatioCD = generateHeavyMassRatio(engine);
			starD.SetMass(baseMass * massRatioAC * massRatioCD);
			//mainSystem.secondSystem.SetSingleStar(starD);

			double separationCD =  generateDistanceBetweenStars(engine, baseMass * massRatioAC);
			double eccenCD = generateMultipleStarEccentricity(engine, separationCD);

			double maxAB = separationAB * (1 + eccenAB);
			double maxCD = separationCD * (1 + eccenCD);

			double maxSep;
			if (maxAB > maxCD) { maxSep = maxAB; }
			else { maxSep = maxCD; }

			double separationABCD = generateDistanceBetweenStars(engine, baseMass + baseMass * massRatioAC);
				while (separationABCD < 3 * maxSep) {
					separationABCD =  generateDistanceBetweenStars(engine, baseMass + baseMass * massRatioAC);
				}
			double eccenABCD = generateMultipleStarEccentricity(engine, separationABCD);
		}
	} // END IS_MULTIPLE

	// Age, Metallicity, Luminosity, Lifespan
	double systemAge = generateSystemAge(engine);
	//systemAge = 6.5; // for testing
	double metallicity = generateMetallicity(engine, systemAge);
	starA.SetAge(systemAge);
	starA.SetMetallicity(metallicity);

	// evolve Star A
	evolveStar(starA, engine);

	if (multiplicity == 2) {
		starB.SetAge(systemAge);
		starB.SetMetallicity(metallicity);
		evolveStar(starB, engine);

		mainSystem.SetPrimaryStar(starA);
		mainSystem.SetSecondaryStar(starB);
	}
	else if (multiplicity == 3) {
		starB.SetAge(systemAge);
		starB.SetMetallicity(metallicity);
		evolveStar(starB, engine);

		starC.SetAge(systemAge);
		starC.SetMetallicity(metallicity);
		evolveStar(starC, engine);
	}
	else {
		cout << "Not yet implemented (2)!\n\n";
	}

	/* PLANETARY DISK FOR MAIN STAR(S)
	 * First, a dummy "star" is created
	 * In a multiple system, the main planets may orbit *two* of them
	 */
	Star dummyStar;
	bool dummyStarIsCircumbinary = false;
	double initialLuminosity = getInitialLuminosity(starA.GetMass());
	double innerExclusionZone = 0.0;

	if (multiplicity > 1) { // determine if the planets are circumbinary or not
		if (multiplicity == 2) {
			// data is held in overallSeparation
			// get AB outer exclusion zone
			double exclusionZone = getOuterOrbitalExclusionZone(starA.GetMass(), starB.GetMass(), overallSeparation.separation, overallSeparation.eccentricity);
			// if the separation of AB is SMALLER than this, it's circumbinary
			if (overallSeparation.separation < exclusionZone) {
				dummyStarIsCircumbinary = true;
				dummyStar.SetMass(starA.GetMass() + starB.GetMass());
				dummyStar.SetLuminosity(starA.GetLuminosity() + starB.GetLuminosity());
				dummyStar.SetRadius(starA.GetRadius());
				dummyStar.SetTemperature(starA.GetTemperature());
				dummyStar.SetAge(starA.GetAge());
				dummyStar.SetMetallicity(starA.GetMetallicity());
				initialLuminosity = getInitialLuminosity(starA.GetMass()) + getInitialLuminosity(starB.GetMass());
				innerExclusionZone = exclusionZone;
			}
			else {
				dummyStar.SetMass(starA.GetMass());
				dummyStar.SetLuminosity(starA.GetLuminosity());
				dummyStar.SetRadius(starA.GetRadius());
				dummyStar.SetTemperature(starA.GetTemperature());
				dummyStar.SetAge(starA.GetAge());
				dummyStar.SetMetallicity(starA.GetMetallicity());
			}
		} // close multiplicity == 3
		else if (multiplicity == 3 && systemArrangement == 1) { // C orbits AB
			// data is held in mainSystem
			// get AB outer exclusion zone
			double exclusionZone = getOuterOrbitalExclusionZone(starA.GetMass(), starB.GetMass(), mainSystem.GetSeparation(), mainSystem.GetEccentricity());
			// if the separation of AB is SMALLER than this, it's circumbinary
			if (mainSystem.GetSeparation()  < exclusionZone) {
				dummyStarIsCircumbinary = true;
				dummyStar.SetMass(starA.GetMass() + starB.GetMass());
				dummyStar.SetLuminosity(starA.GetLuminosity() + starB.GetLuminosity());
				dummyStar.SetRadius(starA.GetRadius());
				dummyStar.SetTemperature(starA.GetTemperature() + starB.GetTemperature());
				dummyStar.SetAge(starA.GetAge());
				dummyStar.SetMetallicity(starA.GetMetallicity());
				initialLuminosity = getInitialLuminosity(starA.GetMass()) + getInitialLuminosity(starB.GetMass());
				innerExclusionZone = exclusionZone; //getInnerOrbitalExclusionZone(starA.GetMass() + starB.GetMass(), starC.GetMass(), overallSeparation.separation, overallSeparation.eccentricity);
			}
			else {
				dummyStar.SetMass(starA.GetMass());
				dummyStar.SetLuminosity(starA.GetLuminosity());
				dummyStar.SetRadius(starA.GetRadius());
				dummyStar.SetTemperature(starA.GetTemperature());
				dummyStar.SetAge(starA.GetAge());
				dummyStar.SetMetallicity(starA.GetMetallicity());
			}
		} // close multiplicity == 3 && systemArrangement == 1
		else if (multiplicity == 3 && systemArrangement != 1) { // A orbits BC
			dummyStar.SetMass(starA.GetMass());
			dummyStar.SetLuminosity(starA.GetLuminosity());
			dummyStar.SetRadius(starA.GetRadius());
			dummyStar.SetTemperature(starA.GetTemperature());
			dummyStar.SetAge(starA.GetAge());
			dummyStar.SetMetallicity(starA.GetMetallicity());
		}
	}
	else if (multiplicity == 4) {
		cout << "Not yet implemented!\n\n";
	}
	else { // single star
		dummyStar.SetMass(starA.GetMass());
		dummyStar.SetLuminosity(starA.GetLuminosity());
		dummyStar.SetRadius(starA.GetRadius());
		dummyStar.SetTemperature(starA.GetTemperature());
		dummyStar.SetAge(starA.GetAge());
		dummyStar.SetMetallicity(starA.GetMetallicity());
	}

	// put forbidden zones here
	double forbiddenZone = 1000000.0;
	if (multiplicity == 2 && !dummyStarIsCircumbinary) { // A is orbited by B; planets orbit A
		forbiddenZone = getInnerOrbitalExclusionZone (starA.GetMass(), starB.GetMass(), overallSeparation.separation, overallSeparation.eccentricity);
	}
	else if (multiplicity == 3 && systemArrangement && dummyStarIsCircumbinary) { // AB is orbited by C; planets orbit AB
		forbiddenZone = getInnerOrbitalExclusionZone (starA.GetMass() + starB.GetMass(), starC.GetMass(), overallSeparation.separation, overallSeparation.eccentricity);
	}
	else if (multiplicity == 3 && !systemArrangement) { // A is orbited by BC; planets orbit A
		forbiddenZone = getInnerOrbitalExclusionZone (starA.GetMass(), starC.GetMass() + starB.GetMass(), overallSeparation.separation, overallSeparation.eccentricity);
	}

	// Planets around primary star
	vector<Planet> dummyStarPlanets = formPlanets (dummyStar, engine, forbiddenZone, dummyStarIsCircumbinary, initialLuminosity, innerExclusionZone);

	cout << "\nFinal layout...:\n";
	for (int i = 0; i < dummyStarPlanets.size(); i++) {
		cout << i << ": " << dummyStarPlanets[i].GetDistance() << " AU; mass " << dummyStarPlanets[i].GetMass();
		cout << "; eccen " << dummyStarPlanets[i].GetEccentricity();
		cout << "; density " << dummyStarPlanets[i].GetDensity();
		cout << "; radius " << dummyStarPlanets[i].GetRadius();
		cout << "; gravity " << dummyStarPlanets[i].GetGravity();
		cout << "; class " << dummyStarPlanets[i].GetPlanetClass() << endl;
	}


	// file output
	std::filesystem::create_directory("output");

	ofstream outFile;
	string file_out;
	file_out = "output/System_" + to_string(seed);
	file_out = file_out + string(".html");
	cout << "file_out: " << file_out << endl;
	outFile.open(file_out.c_str());

	if (outFile.fail()) {
		  cout << "Could not open output file.\n";
		  exit(1);
	}

	outFile << "<html>\n\t<head>\n\t\t<title>GenSystem #" << seed << "</title>\n";
	outFile << "\t\t<link href=\"../misc/styles.css\" rel=\"stylesheet\" type=\"text/css\" media=\"all\" />\n\t</head>";
	outFile << "\n\t<body>\n";
	outFile << "\t\t<h1>System " << seed << "</h1>\n\n";

	string firstStarName = "System " + to_string(seed);
	if (multiplicity == 1) {
		outFile << "\t\t<table class=\"infobox\">\n";
		outFile << "\t\t\t<colgroup><col width=\"300\" /><col width=\"300\" /></colgroup>\n";
		outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">" << firstStarName << "</th></tr>\n\n";
		outFile << "\t\t\t<tr><td><strong>Spectral type</strong></td><td>";
		outFile << starA.GetSpectralType() << " " << starA.GetLuminosityClass() << "</td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Mass</strong></td><td>";
		outFile << starA.GetMass() << " M<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Radius</strong></td><td>";
		outFile << starA.GetRadius() << " R<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Luminosity</strong></td><td>";
		outFile << starA.GetLuminosity() << " L<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Temperature</strong></td><td>";
		outFile << starA.GetTemperature() << " K</td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Age</strong></td><td>";
		outFile << starA.GetAge() << " Ga</td></tr>\n";
		outFile << "\t\t</table>\n";
	}
	else { // is multiple
		outFile << "\t\t<table class=\"infobox\">\n";
		outFile << "\t\t\t<colgroup><col width=\"300\" /><col width=\"300\" /></colgroup>\n";
		outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">" << firstStarName << "</th></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Age</strong></td><td>";
		outFile << starA.GetAge() << " Ga</td></tr>\n";

		outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">" << firstStarName << " A</th></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Spectral type</strong></td><td>";
		outFile << starA.GetSpectralType() << " " << starA.GetLuminosityClass() << "</td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Mass</strong></td><td>";
		outFile << starA.GetMass() << " M<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Radius</strong></td><td>";
		outFile << starA.GetRadius() << " R<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Luminosity</strong></td><td>";
		outFile << starA.GetLuminosity() << " L<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Temperature</strong></td><td>";
		outFile << starA.GetTemperature() << " K</td></tr>\n";

		outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">" << firstStarName << " B</th></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Spectral type</strong></td><td>";
		outFile << starB.GetSpectralType() << " " << starB.GetLuminosityClass() << "</td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Mass</strong></td><td>";
		outFile << starB.GetMass() << " M<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Radius</strong></td><td>";
		outFile << starB.GetRadius() << " R<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Luminosity</strong></td><td>";
		outFile << starB.GetLuminosity() << " L<sub>&#x2609;</sub></td></tr>\n";
		outFile << "\t\t\t<tr><td><strong>Temperature</strong></td><td>";
		outFile << starB.GetTemperature() << " K</td></tr>\n";
		if (multiplicity == 3 || multiplicity == 4) {
			outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">" << firstStarName << " C</th></tr>\n";
			outFile << "\t\t\t<tr><td><strong>Spectral type</strong></td><td>";
			outFile << starC.GetSpectralType() << " " << starC.GetLuminosityClass() << "</td></tr>\n";
			outFile << "\t\t\t<tr><td><strong>Mass</strong></td><td>";
			outFile << starC.GetMass() << " M<sub>&#x2609;</sub></td></tr>\n";
			outFile << "\t\t\t<tr><td><strong>Radius</strong></td><td>";
			outFile << starC.GetRadius() << " R<sub>&#x2609;</sub></td></tr>\n";
			outFile << "\t\t\t<tr><td><strong>Luminosity</strong></td><td>";
			outFile << starC.GetLuminosity() << " L<sub>&#x2609;</sub></td></tr>\n";
			outFile << "\t\t\t<tr><td><strong>Temperature</strong></td><td>";
			outFile << starC.GetTemperature() << " K</td></tr>\n";
		}
		if (dummyStarIsCircumbinary == true) { firstStarName += " AB"; }
		// print orbits
		if (multiplicity == 2 || (multiplicity == 3 && systemArrangement == 1) ) {
			outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">Orbit</th></tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Primary</strong></td>\n";
			outFile << "\t\t\t\t<td>A</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Companion</strong></td>\n";
			outFile << "\t\t\t\t<td>B</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Period</strong></td>\n";

			double separation, eccentricity;
			separation = mainSystem.GetSeparation();
			eccentricity = mainSystem.GetEccentricity();

			double period = sqrt(pow(separation, 3.0) / (starA.GetMass() + starB.GetMass()));

			outFile << "\t\t\t\t<td>" << period << " a</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Semi-major axis</strong></td>\n";
			outFile << "\t\t\t\t<td>" << separation << " AU</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Eccentricity</strong></td>\n";
			outFile << "\t\t\t\t<td>" << eccentricity << "</td>\n\t\t\t</tr>\n";

			if (multiplicity == 3 && systemArrangement == 1) {
				outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">Orbit</th></tr>\n";
				outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Primary</strong></td>\n";
				outFile << "\t\t\t\t<td>AB</td>\n\t\t\t</tr>\n";
				outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Companion</strong></td>\n";
				outFile << "\t\t\t\t<td>C</td>\n\t\t\t</tr>\n";
				outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Period</strong></td>\n";

				double separation, eccentricity;
				separation = overallSeparation.separation;
				eccentricity = overallSeparation.eccentricity;

				double period = sqrt(pow(separation, 3.0) / (starA.GetMass() + starB.GetMass() + starC.GetMass()));

				outFile << "\t\t\t\t<td>" << period << " a</td>\n\t\t\t</tr>\n";
				outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Semi-major axis</strong></td>\n";
				outFile << "\t\t\t\t<td>" << separation << " AU</td>\n\t\t\t</tr>\n";
				outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Eccentricity</strong></td>\n";
				outFile << "\t\t\t\t<td>" << eccentricity << "</td>\n\t\t\t</tr>\n";
			}
		} // close (multiplicity == 2 || (multiplicity == 3 && systemArrangement == 1))
		else if (multiplicity == 3 && systemArrangement == 0) {
			outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">Orbit</th></tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Primary</strong></td>\n";
			outFile << "\t\t\t\t<td>A</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Companion</strong></td>\n";
			outFile << "\t\t\t\t<td>BC</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Period</strong></td>\n";

			double separation, eccentricity;
			separation = overallSeparation.separation;
			eccentricity = overallSeparation.eccentricity;

			double period = sqrt(pow(separation, 3.0) / (starA.GetMass() + starB.GetMass() + starC.GetMass()));

			outFile << "\t\t\t\t<td>" << period << " a</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Semi-major axis</strong></td>\n";
			outFile << "\t\t\t\t<td>" << separation << " AU</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Eccentricity</strong></td>\n";
			outFile << "\t\t\t\t<td>" << eccentricity << "</td>\n\t\t\t</tr>\n";

			outFile << "\t\t\t<tr><th class=\"star\" colspan=\"2\">Orbit</th></tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Primary</strong></td>\n";
			outFile << "\t\t\t\t<td>B</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Companion</strong></td>\n";
			outFile << "\t\t\t\t<td>C</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Period</strong></td>\n";

			separation = farSystem.GetSeparation();
			eccentricity = farSystem.GetEccentricity();
			period = sqrt(pow(separation, 3.0) / (starC.GetMass() + starB.GetMass()));

			outFile << "\t\t\t\t<td>" << period << " a</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Semi-major axis</strong></td>\n";
			outFile << "\t\t\t\t<td>" << separation << " AU</td>\n\t\t\t</tr>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<td><strong>Eccentricity</strong></td>\n";
			outFile << "\t\t\t\t<td>" << eccentricity << "</td>\n\t\t\t</tr>\n";
		} // close (multiplicity == 3 && systemArrangement == 0)


		outFile << "\t\t</table>\n";
	} // close is multiple
	outFile << "\t\t<h2>" << firstStarName << " planets</h2>\n\n";

	/*
   * SUMMARY TABLE
   */
	outFile << "\t\t<table class=\"infobox\">\n";
	outFile << "\t\t\t<colgroup><col width=\"50\" /><col width=\"50\" /><col width=\"300\" /><col width=\"300\" /><col width=\"300\" /><col width=\"300\" /></colgroup>\n";
	outFile << "\t\t\t<tr><th colspan=\"6\">" << firstStarName << "</th></tr>\n";
	outFile << "\t\t\t<tr><th>&numero;</th><th colspan=\"2\">Type</th><th>Distance</th><th>Mass</th><th>Radius</th></tr>\n";
	for (int i = 0; i < dummyStarPlanets.size(); i++) {
		char planetNo = i + 98;
		outFile << "\t\t\t<tr>\n\t\t\t\t<td><a href=\"#" << firstStarName << " " << planetNo << "\">" << planetNo << "</a></td>\n";
		PlanetClass theClass = dummyStarPlanets[i].GetPlanetClass();
		string className, imgFileName;

		printPlanetaryClass (theClass, className, imgFileName);
		
		outFile << "\t\t\t\t<td><img src=\"../misc/" << imgFileName << "\" width=\"20\" /></td>\n";
		outFile << "\t\t\t\t<td>" << className << "</td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetDistance() << " AU</td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetMass() << " M<sub>E</sub></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetRadius() << " R<sub>E</sub></td>\n";
		outFile << "\t\t\t</tr>\n";
	}
	outFile << "\t\t\t</table>\n";


	/*
	 * FULL DETAILS
	 */
	for (int i = 0; i < dummyStarPlanets.size(); i++) {
		char planetNo = i + 98;
		outFile << "\t\t<p>&nbsp;</p>\n";
		outFile << "\t\t<table class=\"infobox\" id=\"" << firstStarName << " " << planetNo << "\">\n";
		outFile << "\t\t\t<colgroup><col width=\"500\" /><col width=\"300\" /><col width=\"300\" /></colgroup>\n";
		outFile << "\t\t\t<tr>\n\t\t\t\t<th colspan=\"3\">" << firstStarName << " " << planetNo << "</th>\n\t\t\t</tr>\n";

		PlanetClass theClass = dummyStarPlanets[i].GetPlanetClass();
		string className, imgFileName;

		printPlanetaryClass (theClass, className, imgFileName);

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Planet type</strong></td>\n";
		outFile << "\t\t\t\t<td><img src=\"../misc/" << imgFileName << "\" width=\"20\" />&nbsp;" << className << "</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Distance from star(s)</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetDistance() << " AU</td>\n";
		outFile << "\t\t\t</tr>\n";
		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Orbital eccentricity</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetEccentricity() << "</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Orbital period</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetOrbitalPeriod() << " a</td>\n";
		outFile << "\t\t\t\t<td>" << 365.25 * sqrt(pow(dummyStarPlanets[i].GetDistance(), 3.0) / dummyStar.GetMass()) << " d</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Insolation</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStar.GetLuminosity() / pow(dummyStarPlanets[i].GetDistance(), 2.0) << "</td>\n";
		outFile << "\t\t\t\t<td>" << 1321.0 * dummyStar.GetLuminosity() / pow(dummyStarPlanets[i].GetDistance(), 2.0) << " W/m<sup>2</sup></td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Mass</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetMass() << " M<sub>E</sub></td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Radius</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetRadius() << " R<sub>E</sub></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetRadius() * 6371.0 << " km</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Density</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetDensity() << " D<sub>E</sub></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetDensity() * 5.52 << " g/cc</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Gravity</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetGravity() << " g</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Siderial rotation period</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetRotationPeriod() << " h</td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetRotationPeriod() / 24.0 << " d</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Axial tilt</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetAxialTilt() << "&deg;</td>\n";
		outFile << "\t\t\t</tr>\n";

		if (theClass == TERRESTRIAL_PLANET || theClass == LEFTOVER_OLIGARCH || theClass == VENUSIAN || theClass == HYCEAN || theClass == TITANIAN || theClass == GAIAN || theClass == MARTIAN) {
			outFile << "\t\t\t<tr>\n";
			outFile << "\t\t\t\t<td><strong>Albedo</strong></td>\n";
			outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetAlbedo() << " (Bond)</td>\n";
			outFile << "\t\t\t</tr>\n";

			outFile << "\t\t\t<tr>\n";
			outFile << "\t\t\t\t<td><strong>Hydrographic coverage</strong></td>\n";
			outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetOceanPct() * 100.0 << "%</td>\n";
			outFile << "\t\t\t</tr>\n";

			outFile << "\t\t\t<tr>\n";
			outFile << "\t\t\t\t<td><strong>Atmosphere</strong></td>\n";
			outFile << setprecision(4) << "\t\t\t\t<td>";
			Atmosphere a = dummyStarPlanets[i].GetAtmosphere();
			if (a.hydrogen != 0) { outFile << "Hydrogen: " << a.hydrogen * 100.0 << "%<br />"; }
			if (a.helium != 0) { outFile << "Helium: " << a.helium * 100.0 << "%<br />"; }
			if (a.nitrogen != 0) { outFile << "Nitrogen: " << a.hydrogen * 100.0 << "%<br />"; }
			if (a.argon != 0) { outFile << "Argon: " << a.hydrogen * 100.0 << "%<br />"; }
			if (a.carbonDioxide != 0) { outFile << "Carbon dioxide: " << a.carbonDioxide * 100.0 << "%<br />"; }
			if (a.oxygen != 0) { outFile << "Oxygen: " << a.oxygen * 100.0 << "%<br />"; }
			if (a.waterVapor != 0) { outFile << "Water vapor: " << a.waterVapor * 100.0 << "%<br />"; }

			outFile << setprecision(6) << "</td>\n";
			outFile << "\t\t\t</tr>\n";
		}
		else {

		}

		outFile << "\t\t\t<tr>\n";
		outFile << "\t\t\t\t<td><strong>Surface temperature</strong></td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetTemperature() << " K</td>\n";
		outFile << "\t\t\t\t<td>" << dummyStarPlanets[i].GetTemperature() - 273.15 << " &deg;C<br />";
		outFile << (dummyStarPlanets[i].GetTemperature() - 273.15) * 1.8 + 32.0 << " &deg;F</td>\n";
		outFile << "\t\t\t</tr>\n";

		outFile << "\t\t</table>\n\n";

		if (dummyStarPlanets[i].GetNumberOfMoons() != 0) {
			cout << "Planet " << i << " has " << dummyStarPlanets[i].GetNumberOfMoons() << " moons\n";
			outFile << "\t\t<table class=\"infobox\">\n";
			outFile << "\t\t\t<colgroup><col width=\"50\" /><col width=\"300\" /><col width=\"300\" /></colgroup>\n";
			outFile << "\t\t\t<tr>\n\t\t\t\t<th>&numero;</th><th>Distance</th><th>Mass</th>\n";
			vector<Moon> theMoons = dummyStarPlanets[i].GetMoons();
			for (int j = 0; j < theMoons.size(); j++) {
				cout << "Doing moon " << j << endl;
				outFile << "\t\t\t<tr>\n\t\t\t\t<td>" << j + 1 << "</td>\n";
				outFile << "\t\t\t\t<td>" << setprecision(9) << theMoons[j].GetDistance() << " km</td>\n";
				outFile << "\t\t\t\t<td>" << setprecision(6) << theMoons[j].GetMass() << " M<sub>E</sub><br />";
				outFile << theMoons[j].GetMass() / 0.0123 << " M<sub>Moon</sub></td>\n";
				outFile << "\t\t\t</tr>\n";
			}
			outFile << "\t\t</table>\n\n";
		}
	}


	outFile << "\t\t<p>Generated by <a href=\"https://github.com/giancarlow333/GenSystem\">GenSystem</a>!</p>";
	outFile << "\t</body>\n</html>";
	outFile.close();
	cout << "Goodbye, and good luck!" << endl;

	return 0;
}

// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////

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
// The paper makes me think that 45 AU is the *mode*, in which case the mean is ln(45 - sigma^2) or 3.68.
// Keeping the stdev of 2.3 seems to match the upper end of the distro (4.6% above 1500 AU), but not the lower end!
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

double getOuterSystemProperties(Planet & p, int mod, int pNumber, default_random_engine & e) {
	uniform_int_distribution<> diceRoll(1, 6);
	int roll = diceRoll(e) + diceRoll(e) + diceRoll(e) + mod;
	double pMass = p.GetMass();

	if (roll <= 14) {
		p.SetPlanetClass(NONE);
		p.SetMass(0);
		if (pNumber == 5) {
			return 0.25; // mass from middle zone sent to inner zone
		}
	}
	else if (roll <= 18) {
		p.SetPlanetClass(NONE);
		p.SetMass(0);
		if (pNumber == 5) {
			return 0.20;
		}
	}
	else if (roll <= 20) {
		p.SetPlanetClass(FAILED_CORE);
		uniform_real_distribution<> unifRoll(1, 6);
		p.SetMass((unifRoll(e) + 3) * 0.25);
		if (pNumber == 5) {
			return 0.175;
		}
	}
	else if (roll <= 22) {
		p.SetPlanetClass(FAILED_CORE);
		uniform_real_distribution<> unifRoll(1, 6);
		p.SetMass((unifRoll(e) + 6) * 0.25);
		if (pNumber == 5) {
			return 0.15;
		}
	}
	else if (roll <= 24) {
		p.SetPlanetClass(FAILED_CORE);
		uniform_real_distribution<> unifRoll(1, 6);
		p.SetMass((unifRoll(e) + 9) * 0.25);
		if (pNumber == 5) {
			return 0.125;
		}
	}
	else if (roll <= 26) {
		p.SetPlanetClass(FAILED_CORE);
		uniform_real_distribution<> unifRoll(1, 6);
		p.SetMass((unifRoll(e) + 12) * 0.25);
		if (pNumber == 5) {
			return 0.125;
		}
	}
	else if (roll <= 28) {
		p.SetPlanetClass(SMALL_GAS_GIANT);
		p.SetMass(1.1 * pMass);
		if (pNumber == 5) {
			return 0.1;
		}
	}
	else if (roll <= 30) {
		p.SetPlanetClass(SMALL_GAS_GIANT);
		p.SetMass(1.6 * pMass);
		if (pNumber == 5) {
			return 0.1;
		}
	}
	else if (roll <= 32) {
		p.SetPlanetClass(SMALL_GAS_GIANT);
		p.SetMass(2.4 * pMass);
		if (pNumber == 5) {
			return 0.075;
		}
	}
	else if (roll <= 34) {
		p.SetPlanetClass(SMALL_GAS_GIANT);
		p.SetMass(3.6 * pMass);
		if (pNumber == 5) {
			return 0.075;
		}
	}
	else if (roll <= 36) {
		p.SetPlanetClass(MEDIUM_GAS_GIANT);
		p.SetMass(5 * pMass);
		if (pNumber == 5) {
			return 0.05;
		}
	}
	else if (roll <= 38) {
		p.SetPlanetClass(MEDIUM_GAS_GIANT);
		p.SetMass(6 * pMass);
		if (pNumber == 5) {
			return 0.05;
		}
	}
	else if (roll <= 40) {
		p.SetPlanetClass(MEDIUM_GAS_GIANT);
		p.SetMass(7 * pMass);
	}
	else if (roll <= 42) {
		p.SetPlanetClass(MEDIUM_GAS_GIANT);
		p.SetMass(8.5 * pMass);
	}
	else if (roll <= 44) {
		p.SetPlanetClass(LARGE_GAS_GIANT);
		p.SetMass(10.0 * pMass);
	}
	else if (roll <= 46) {
		p.SetPlanetClass(LARGE_GAS_GIANT);
		p.SetMass(12.0 * pMass);
	}
	else if (roll <= 48) {
		p.SetPlanetClass(LARGE_GAS_GIANT);
		p.SetMass(14.0 * pMass);
	}
	else if (roll <= 50) {
		p.SetPlanetClass(LARGE_GAS_GIANT);
		p.SetMass(17.0 * pMass);
	}
	else {
		p.SetPlanetClass(LARGE_GAS_GIANT);
		p.SetMass(20.0 * pMass);
	}

	return 0.0;
}

/* getInnerOrbitalExclusionZone
 * Maximum stable orbit around A only
 */
double getInnerOrbitalExclusionZone (double pMass, double sMass, double separation, double eccentricity) {
	double combinedMass = pMass + sMass;
	double bMassFraction = sMass / combinedMass;
	double massRatio = pMass / sMass;

	double r1Egg = separation * (0.49 * pow(massRatio, 2/3)) / (0.6 * pow(massRatio, 2/3) + log(1 + pow(massRatio, 1/3)));

	return r1Egg * (0.733 * pow((1 - eccentricity), 1.2) * pow(bMassFraction, 0.07));
}

/* getOuterOrbitalExclusionZone
 * Minimum stable orbit around both A and B
 */
double getOuterOrbitalExclusionZone (double pMass, double sMass, double separation, double eccentricity) {
	double combinedMass = pMass + sMass;
	double bMassFraction = sMass / combinedMass;

	return 1.93 * separation * (1 + 1.01 * pow(eccentricity, 0.32)) * pow(bMassFraction * (1 - bMassFraction), 0.043);
}

// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////
// ////////////////////////////////////

vector<Planet> formPlanets (Star & s, default_random_engine & e, double forbiddenZone, bool starIsCircumbinary, double initialLuminosity, double innerExclusionZone) {
	double diskMassFactor = generateDiskMassFactor(e);
	double migrationFactor = generateMigrationFactor(e, diskMassFactor);

	double diskInnerEdge = 0.005 * pow(s.GetMass(), 1.0 / 3.0);
	double formationIceLine = 4.0 * sqrt(initialLuminosity);
	double slowAccretionLine = 20.0 * pow(s.GetMass(), 1.0 / 3.0);

	double innerFormationZone = 2.5 * s.GetMass() * s.GetMetallicity() * diskMassFactor;
	double middleFormationZone = 80.0 * s.GetMass() * s.GetMetallicity() * diskMassFactor;
	double outerFormationZone = 18.0 * s.GetMass() * s.GetMetallicity() * diskMassFactor;
	cout << "diskMassFactor: " << diskMassFactor << endl;

	vector<FormingPlanet> sPlanets;

	// place inner planets
	double planet0Distance = 0.6 * sqrt(initialLuminosity);
	FormingPlanet temp0;
	temp0.planet.SetDistance(planet0Distance);
	temp0.planet.SetMass(0.08 * innerFormationZone);
	sPlanets.push_back(temp0);

	double planet1Distance = 0.8 * sqrt(initialLuminosity);
	FormingPlanet temp1;
	temp1.planet.SetDistance(planet1Distance);
	temp1.planet.SetMass(0.41 * innerFormationZone);
	sPlanets.push_back(temp1);

	double planet2Distance = 1.2 * sqrt(initialLuminosity);
	FormingPlanet temp2;
	temp2.planet.SetDistance(planet2Distance);
	temp2.planet.SetMass(0.39 * innerFormationZone);
	sPlanets.push_back(temp2);

	double planet3Distance = 1.8 * sqrt(initialLuminosity);
	FormingPlanet temp3;
	temp3.planet.SetDistance(planet3Distance);
	temp3.planet.SetMass(0.08 * innerFormationZone);
	sPlanets.push_back(temp3);
	
	double planet4Distance = 2.7 * sqrt(initialLuminosity);
	FormingPlanet temp4;
	temp4.planet.SetDistance(planet4Distance);
	temp4.planet.SetMass(0.04 * innerFormationZone);
	sPlanets.push_back(temp4);

	// place middle planets
	double planet5Distance = 4.0 * sqrt(initialLuminosity);
	FormingPlanet temp5;
	temp5.planet.SetDistance(planet5Distance);
	temp5.planet.SetMass(0.4 * middleFormationZone);
	sPlanets.push_back(temp5);

	double planet6Distance = 6.0 * sqrt(initialLuminosity);
	FormingPlanet temp6;
	temp6.planet.SetDistance(planet6Distance);
	temp6.planet.SetMass(0.25 * middleFormationZone);
	sPlanets.push_back(temp6);

	double planet7Distance = 9.0 * sqrt(initialLuminosity);
	FormingPlanet temp7;
	temp7.planet.SetDistance(planet7Distance);
	temp7.planet.SetMass(0.18 * middleFormationZone);
	sPlanets.push_back(temp7);

	double planet8Distance = 13.5 * sqrt(initialLuminosity);
	FormingPlanet temp8;
	temp8.planet.SetDistance(planet8Distance);
	temp8.planet.SetMass(0.17 * middleFormationZone);
	sPlanets.push_back(temp8);

	// place outer planets
	double planet9Distance = 20.0 * sqrt(initialLuminosity);
	FormingPlanet temp9;
	temp9.planet.SetDistance(planet9Distance);
	temp9.planet.SetMass(0.6 * outerFormationZone);
	sPlanets.push_back(temp9);

	double planet10Distance = 30.0 * sqrt(initialLuminosity);
	FormingPlanet temp10;
	temp10.planet.SetDistance(planet10Distance);
	temp10.planet.SetMass(0.3 * outerFormationZone);
	sPlanets.push_back(temp10);

	double planet11Distance = 45.0 * sqrt(initialLuminosity);
	FormingPlanet temp11;
	temp11.planet.SetDistance(planet11Distance);
	temp11.planet.SetMass(0.1 * outerFormationZone);
	sPlanets.push_back(temp11);

	// work exclusion zones
	for (int i = 0; i < sPlanets.size(); i++) {
		double distance = sPlanets[i].planet.GetDistance();
		if (distance < diskInnerEdge || distance > slowAccretionLine || distance > forbiddenZone || (distance < innerExclusionZone && starIsCircumbinary)) {
			sPlanets[i].inExclusionZone = true;
		}
	}
	// Mark last before slow accretiong
	for (int i = 0; i < sPlanets.size(); i++) {
		double distance = sPlanets[i].planet.GetDistance();
		if (i + 1 < sPlanets.size()) {
			if (distance < slowAccretionLine && sPlanets[i + 1].planet.GetDistance() > slowAccretionLine) {
				sPlanets[i].lastBeforeSlowAccretion = true;
				sPlanets[i - 1].penultBeforeSlowAccretion = true;
				break;
			}
		}
		else {
			sPlanets[i].lastBeforeSlowAccretion = true;
			sPlanets[i - 1].penultBeforeSlowAccretion = true;
		}
	}

	// Outer Planetary System
	double massToInnerSystem;
	for (int i = 5; i < 12; i++) {
		double planetesimalMass = sPlanets[i].planet.GetMass();

		int accretionModifier = getAccretionModifier(planetesimalMass);

		// modifier if close to slow accretion line
		if (sPlanets[i].penultBeforeSlowAccretion) { accretionModifier -= 8; }
		if (sPlanets[i].lastBeforeSlowAccretion) { accretionModifier -= 16; }

		double temp = getOuterSystemProperties(sPlanets[i].planet, accretionModifier, i, e);
		if (i == 5) {
			massToInnerSystem = temp;
		}
	}
	middleFormationZone *= (1 - massToInnerSystem);
	innerFormationZone += (massToInnerSystem * middleFormationZone);

	// Find dominant gas giant
	bool thereIsADominantGasGiant = false;
	int gasGiantCount = 0;
	int dominantGasGiantIndex = -1;
	for (int i = 5; i < 12; i++) {
		PlanetClass theClass = sPlanets[i].planet.GetPlanetClass();
		if (theClass == SMALL_GAS_GIANT || theClass == MEDIUM_GAS_GIANT || theClass == LARGE_GAS_GIANT) {
			sPlanets[i].isDominantGasGiant = true;
			thereIsADominantGasGiant = true;
			dominantGasGiantIndex = i;
			break;
		}
	}
	for (int i = 5; i < 12; i++) {
		PlanetClass theClass = sPlanets[i].planet.GetPlanetClass();
		if (theClass == SMALL_GAS_GIANT || theClass == MEDIUM_GAS_GIANT || theClass == LARGE_GAS_GIANT) {
			gasGiantCount++;
		}
	}

	// Dominant Gas Giant Inward Migration
	bool thereWasInwardMigration = false;
	double orbitAfterInwardMigration = 0;
	double formationRadius = 0;
	if (thereIsADominantGasGiant) {
		for (int i = 5; i < 12; i++) {
			if (sPlanets[i].isDominantGasGiant) {
				formationRadius = sPlanets[i].planet.GetDistance();
				orbitAfterInwardMigration = formationRadius * migrationFactor;
				if (orbitAfterInwardMigration < diskInnerEdge && !starIsCircumbinary) { // gas giant migrates inwards
					thereWasInwardMigration = true;
					sPlanets[i].planet.SetDistance(orbitAfterInwardMigration);
				}
				else if (orbitAfterInwardMigration < diskInnerEdge && starIsCircumbinary) {
					thereWasInwardMigration = true;
					if (orbitAfterInwardMigration < innerExclusionZone) {
						sPlanets[i].planet.SetDistance(innerExclusionZone * 1.07);
						orbitAfterInwardMigration = innerExclusionZone * 1.07;
					}
					else {
						sPlanets[i].planet.SetDistance(orbitAfterInwardMigration);
					}
				}
				break;
			}
		}
	}
	if (thereWasInwardMigration) { // mark orbits as disturbed
		for (int i = 0; i < sPlanets.size(); i++) {
			if (sPlanets[i].planet.GetDistance() > orbitAfterInwardMigration && sPlanets[i].planet.GetDistance() < formationRadius) {
				sPlanets[i].orbitDisrupted = true;
			}
		}
	}

	// Grand Tack
	bool thereIsAGrandTack = false;
	if (thereIsADominantGasGiant && gasGiantCount > 1) {
		PlanetClass nextPlanet = sPlanets[dominantGasGiantIndex].planet.GetPlanetClass();
		if (nextPlanet == SMALL_GAS_GIANT || nextPlanet == MEDIUM_GAS_GIANT || nextPlanet == LARGE_GAS_GIANT) { // Grand Tack is *possible*
			uniform_int_distribution<> diceRoll(1, 6);
			int tackRoll = diceRoll(e) + diceRoll(e) + diceRoll(e);
			if (tackRoll >= 12) {
				int tackDistanceRoll = diceRoll(e) + diceRoll(e) + diceRoll(e);
				double finalDistance = (1 + tackDistanceRoll / 10.0) * sPlanets[dominantGasGiantIndex].planet.GetDistance();
				sPlanets[dominantGasGiantIndex].planet.SetDistance(finalDistance);
				sPlanets[dominantGasGiantIndex].triggeredGrandTack = true;
				thereIsAGrandTack = true;
			}
		}
	}
	if (thereIsAGrandTack) {
		for (int i = dominantGasGiantIndex + 1; i < sPlanets.size(); i++) {
			double priorFinalOrbitalRadius = sPlanets[i - 1].planet.GetDistance();
			double currentOrbitalRadius = sPlanets[i].planet.GetDistance();
			sPlanets[i].triggeredGrandTack = true;
			if (priorFinalOrbitalRadius * 1.3 > currentOrbitalRadius) {
				sPlanets[i].planet.SetDistance(priorFinalOrbitalRadius * 1.3);
			}
			else {
				break;
			}
		}
	}

	// Nice Event
	if (thereIsAGrandTack && (!forbiddenZone || forbiddenZone > 1.5 * slowAccretionLine)) { // depends on forbidden zones per AOW p. 47
		// TBD: roll for Nice event!!!
		bool aPlanetIsEjected = false;
		for (int i = 5; i < 12; i++) {
			if (sPlanets[i].triggeredGrandTack == false) {
				uniform_int_distribution<> diceRoll(1, 6);
				int niceRoll = diceRoll(e) + diceRoll(e) + diceRoll(e);
				if (niceRoll >= 12) {
					sPlanets[i].planetEjected = true;
					aPlanetIsEjected = true;
				}
			}
		}
		// find last surviving planet and multiply orbit by 50%
		if (aPlanetIsEjected) {
			for (int i = 12; i > 5; i--) {
				if (sPlanets[i].planetEjected && !sPlanets[i - 1].planetEjected) {
					double oldRadius = sPlanets[i - 1].planet.GetDistance();
					double newRadius = 1.5 * oldRadius;
					sPlanets[i - 1].planet.SetDistance(newRadius);
					break;
				}
			}
		}
	}

	// Orbital resonances
	// Dominant and outer-most gas giant should have final placement now
	sPlanets[dominantGasGiantIndex].finalPlacement = true;
	int finalPlanetIndex = 12;
	for (int i = 12; i > 5; i--) {
		if (!sPlanets[i].planetEjected && sPlanets[i].planet.GetPlanetClass() != NONE) {
			sPlanets[i].finalPlacement = true;
			finalPlanetIndex = i;
			break;
		}
	}
	int countToBePlaced = 0;
	for (int i = 6; i < 12; i++) {
		if (!sPlanets[i].finalPlacement) { countToBePlaced++; }
		else { break; }
	}
	placeRemainingPlanets (sPlanets, dominantGasGiantIndex, finalPlanetIndex, countToBePlaced, e);


	// INNER PLANETARY SYSTEM
	innerFormationZone += 0;
	for (int i = 0; i < 5; i++) {
		double planetesimalMass = innerFormationZone * sPlanets[i].planet.GetMass();
		if (sPlanets[i].orbitDisrupted) { planetesimalMass *= 0.5; }

		normal_distribution<> randomNorm(1.05, 0.2958); // 3d6 / 10
		double newMass = planetesimalMass * randomNorm(e);

		if (newMass < 0.03) {
			sPlanets[i].planet.SetPlanetClass(NONE);
		}
		else if (newMass < 0.18) {
			if (i + 1 == dominantGasGiantIndex) {
				sPlanets[i].planet.SetPlanetClass(LEFTOVER_OLIGARCH);
				sPlanets[i].planet.SetMass(randomNorm(e) / 10.0);
			}
			else {
				sPlanets[i].planet.SetPlanetClass(PLANETOID_BELT);
			}
		}
		else {
			sPlanets[i].planet.SetPlanetClass(TERRESTRIAL_PLANET);
			sPlanets[i].planet.SetMass(newMass);
		}
	}

	// Inner Planetary System Migration
	int innermostPlanetIndex = 0;
	for (int i = 0; i < 5; i++) {
		double innermostMigrationRadius = sPlanets[i].planet.GetDistance() * migrationFactor;
		if (innermostMigrationRadius < diskInnerEdge && !starIsCircumbinary) {
			uniform_int_distribution<> diceRoll(1, 6);
			int roll = diceRoll(e);
			if (roll <= 3) { sPlanets[i].planetEjected = true; }
			else {
				sPlanets[i].planet.SetDistance(diskInnerEdge);
				sPlanets[i].finalPlacement = true;
				innermostPlanetIndex = i;
				break;
			}
		}
		else if (innermostMigrationRadius < diskInnerEdge && starIsCircumbinary) {
			uniform_int_distribution<> diceRoll(1, 6);
			int roll = diceRoll(e);
			if (roll <= 3) { sPlanets[i].planetEjected = true; }
			else {
				if (innermostMigrationRadius > innerExclusionZone) {
					sPlanets[i].planet.SetDistance(diskInnerEdge);
					sPlanets[i].finalPlacement = true;
					innermostPlanetIndex = i;
					break;
				}
				else { // it'd be in the exclusion zone
					sPlanets[i].planet.SetDistance(innerExclusionZone * 1.1);
					sPlanets[i].finalPlacement = true;
					innermostPlanetIndex = i;
					break;
				}
			}
		}
	}
	// If there are no remaining objects before the dominant gas giant, inner formation is done
	// Else, if there are no gas giants, move the final planet outward

	// Place remaining inner system
	// count how many objects remain to be placed between innermost surviving object and either the dominant gas giant or, if no such, the outermost planet
	countToBePlaced = 0;
	for (int i = innermostPlanetIndex + 1; i < dominantGasGiantIndex; i++) { // NOT QUITE RIGHT!
		if (!sPlanets[i].finalPlacement) { countToBePlaced++; }
		else { break; }
	}
	placeRemainingPlanets (sPlanets, innermostPlanetIndex, dominantGasGiantIndex, countToBePlaced, e);

	// Make sure there aren't any in exclusion zones after migration!
	for (int i = 0; i < sPlanets.size(); i++) {
		double distance = sPlanets[i].planet.GetDistance();
		if (distance < innerExclusionZone || distance > forbiddenZone) {
			sPlanets[i].inExclusionZone = true;
		}
	}

	// Remove eliminated orbits
	vector<Planet> sPlanets2;
	for (int i = 0; i < sPlanets.size(); i++) {
		if (!sPlanets[i].orbitDisrupted && !sPlanets[i].planetEjected && !sPlanets[i].inExclusionZone && sPlanets[i].planet.GetPlanetClass() != NONE) {
			sPlanets2.push_back(sPlanets[i].planet);
		}
	}

	// Set orbital eccentricities
	int totalNumberOfPlanets = sPlanets2.size();
	double typicalEccen = getTypicalEccentricity(totalNumberOfPlanets);
	for (int i = 0; i < sPlanets2.size(); i++) {
		normal_distribution<> randomNorm(-0.035, 0.02415); // 2d6-7 / 100
		double eccen = typicalEccen + randomNorm(e);
		if (eccen < 0) { eccen = 0; }
		sPlanets2[i].SetEccentricity(eccen);
	}

	// Density, Radius, and Surface Gravity
	for (int i = 0; i < sPlanets2.size(); i++) {
		PlanetClass pc = sPlanets2[i].GetPlanetClass();
		double density;
		if (pc == SMALL_GAS_GIANT || pc == MEDIUM_GAS_GIANT || pc == LARGE_GAS_GIANT) {
			if (sPlanets2[i].GetMass() <= 200) {
				density = 1.0 / sqrt(sPlanets2[i].GetMass());
			}
			else { // gas giant > 200 masses
				density = pow(sPlanets2[i].GetMass(), 1.27) / 11800.0;
			}
		} // end gas giant
		else if (pc != PLANETOID_BELT) {
			normal_distribution<> randomNorm(0.5, 2.958); // 3d6-10
			density = pow(sPlanets2[i].GetMass(), 0.2) + randomNorm(e) / 100.0;
			if (pc == FAILED_CORE) { density -= 0.1; }
			if (pc == LEFTOVER_OLIGARCH) {
				uniform_int_distribution<> diceRoll(1, 6);
				int roll = diceRoll(e);
				if (roll > 4) { density += 0.4; }
			}
			if (density < 0.18) { density = 0.18; }
			if (density > 1.43) { density = 1.43; }
		} // end not planetoid belt
		else { // it's a planetoid belt
			density = 0.0;
		}
		sPlanets2[i].SetDensity(density);

		// compute radius and surface gravity
		if (pc != PLANETOID_BELT) {
			double radius = pow(sPlanets2[i].GetMass() / density, 1.0 / 3.0);
			sPlanets2[i].SetRadius(radius);
			sPlanets2[i].SetGravity(density * radius);
		}
		else {
			sPlanets2[i].SetRadius(0.0);
			sPlanets2[i].SetGravity(0.0);
		}
	}

	// place moons
	for (int i = 0; i < sPlanets2.size(); i++) {
		double apastron = sPlanets2[i].GetDistance() * (1.0 - sPlanets2[i].GetEccentricity());
		double hillSphereInKm = 2.17e6 * apastron * pow(sPlanets2[i].GetMass() / s.GetMass(), 1.0/3.0);
		int numberOfMajorMoons = 2e-15 * pow(hillSphereInKm, 2.0) / sqrt(sPlanets2[i].GetDistance());
		uniform_int_distribution<> plusMinus2(-2, 2);
		int moonModifier = plusMinus2(e);
		if (numberOfMajorMoons > 8) {
			numberOfMajorMoons = 8;
		}
		if (numberOfMajorMoons != 0) {
			numberOfMajorMoons += moonModifier;
		}
		if (numberOfMajorMoons < 0) {
			numberOfMajorMoons = 0;
		}
		//cout << "Planet " << i << " has " << numberOfMajorMoons << " major moons." << endl;

		int laplaceResonanceCount = 0;
		double priorMoonDistance = 0;
		for (int j = 0; j < numberOfMajorMoons; j++) {
			uniform_int_distribution<> rollDice(1, 6);
			normal_distribution<> randomNorm(10.5, 2.958);
			int roll = randomNorm(e);
			double moonMass = 1e-5 * (roll * sPlanets2[i].GetMass()) / numberOfMajorMoons * diskMassFactor;
			// TBD: a way to form even Mars-sized moons!  Perhaps this?
			double distance = 0;
			if (j == 0) { // it's the first moon
				uniform_real_distribution<> rUnif(3, 8);
				distance = rUnif(e) * sPlanets2[i].GetRadius() * 6371.0;
				priorMoonDistance = distance;
			}
			else {
				if (laplaceResonanceCount == 1) {
					distance = priorMoonDistance * 1.587;
					laplaceResonanceCount++;
				}
				else {
					int roll2 = rollDice(e) + rollDice(e) + rollDice(e);
					if (roll2 >= 9 && roll2 <= 12) {
						laplaceResonanceCount++;
					}
					else { laplaceResonanceCount = 0; }
					distance = getFromMajorSatelliteOrbitalRatioTable(roll2) * priorMoonDistance;
				}
				priorMoonDistance = distance;
			}
			//cout << "distance: " << distance << endl;
			if (distance > hillSphereInKm) { break; }
			Moon temp(distance, moonMass);
			sPlanets2[i].SetSingleMoon(temp);
		}

		// TBD: giant impact moons
	}

	// orbital periods
	for (int i = 0; i < sPlanets2.size(); i++) {
		double period = sqrt(pow(sPlanets2[i].GetDistance(), 3.0) / s.GetMass());
		sPlanets2[i].SetOrbitalPeriod(period);
	}

	// rotation periods and obliquity
	for (int i = 0; i < sPlanets2.size(); i++) {
		double rotationPeriod;
		double tideLockRadius = pow(s.GetAge() * pow(s.GetMass(), 2.0) / 479.0, 1.0 / 6.0);
		bool isTidallyLocked = false;
		if (sPlanets2[i].GetDistance() < tideLockRadius) { // tidally locked in some way
			isTidallyLocked = true;
			double year = sPlanets2[i].GetOrbitalPeriod();
			double eccen = sPlanets2[i].GetEccentricity();
			if (eccen <= 0.12) { rotationPeriod = year; }
			else if (eccen <= 0.25) { rotationPeriod = year * 2.0 / 3.0; }
			else if (eccen <= 0.35) { rotationPeriod = year / 2.0; }
			else if (eccen <= 0.45) { rotationPeriod = year * 2.0 / 5.0; }
			else { rotationPeriod = year / 3.0; }

			rotationPeriod *= 8766; // in hours
		}
		else {
			// temporary measure while I research this
			// based on gut only: mean 24, stdev 1
			// IQR is 12.22 to 47.11 hours (0.51 to 1.96 d)
			lognormal_distribution<> logNorm(3.18, 1.0);
			rotationPeriod = logNorm(e);
			if (rotationPeriod < 4.0) { rotationPeriod = 4.0; }
		}

		sPlanets2[i].SetRotationPeriod(rotationPeriod);

		// axial tilt
		double axis = 0;
		PlanetClass pc;
		if (sPlanets2[i].GetNumberOfMoons() > 0) { // has major moon(s), is terrestrial, leftover oligarch, failed core
			normal_distribution<> randomNorm(30.0, 9.0); // basically 4d6
			axis = randomNorm(e);
		}
		else if (isTidallyLocked) {
			normal_distribution<> randomNorm(2.5, 3.0); // basically 3d6-8
			axis = randomNorm(e);
			if (axis < 0) { axis = 0; }
		}
		else { // not tidally locked, no moons, or gas giant
			lognormal_distribution<> randomLogNorm(3.56, 1.1); // average 35, IQR 16.7 - 73.5
			axis = randomLogNorm(e);
			if (axis < 0) { axis = 0; }
		}
		sPlanets2[i].SetAxialTilt(axis);
	}
	// solar day
	// can do when printing

	// temperature and surface water
	for (int i = 0; i < sPlanets2.size(); i++) {
		PlanetClass pc = sPlanets2[i].GetPlanetClass();
		// blackbody temp
		double blackBodyTemp = 278.0 * pow(s.GetLuminosity(), 0.25) / sqrt(sPlanets2[i].GetDistance());
		cout << "blackBodyTemp: " << blackBodyTemp << endl;
		// minimum molecular weight retained
		double squaredRadius = pow(sPlanets2[i].GetRadius() * 6371.0, 2.0);
		double minMWR = 676300.0 * (blackBodyTemp / (sPlanets2[i].GetDensity() * squaredRadius));
		minMWR = ceil(minMWR);

		// ocean formation
		bool thereWasARunawayGreenhouse = false;
		if (pc == TERRESTRIAL_PLANET || pc == LEFTOVER_OLIGARCH) { 
			double oceanPctge;
			if (minMWR <= 2) { oceanPctge = 1.0; }
			else if (minMWR <= 28) {
				if (sPlanets2[i].GetDistance() > formationIceLine) { oceanPctge = 1.0; }
				else { // did not form beyond ice line
					normal_distribution<> randomNorm(0.55, 0.33); // basically 3d6
					oceanPctge = randomNorm(e);
					if (thereIsAGrandTack) {
						oceanPctge += 0.2;
					}
					if (oceanPctge < 0.0) { oceanPctge = 0.0; }
					if (oceanPctge > 1.0) { oceanPctge = 1.0; }
				}
			}
			else { // MMWR >= 29
				if (blackBodyTemp >= 125) {
					// also if major gas giant moon closer than 8 radii
					oceanPctge = 0.0;
				}
				else {
					oceanPctge = 1.0;
				}
			} // close ocean else
			// possible loss of primordial water
			if (oceanPctge < 0.15 && blackBodyTemp >= 300) {
				uniform_int_distribution<> diceRoll(1, 6);
				int roll = diceRoll(e) + diceRoll(e) + diceRoll(e);
				if (blackBodyTemp + roll > 318) {
					oceanPctge = 0.0;
				}
			}
			if (oceanPctge > 0.15 && blackBodyTemp >= 300) {
				uniform_int_distribution<> diceRoll(1, 6);
				int roll = diceRoll(e) + diceRoll(e) + diceRoll(e);
				if (blackBodyTemp + roll > 318) { // runaway greenhouse
					thereWasARunawayGreenhouse = true;
					oceanPctge = 0.0;
				}
			}
			sPlanets2[i].SetOceanPct(oceanPctge);
			// end ocean formation

			// Atmosphere
			normal_distribution<> threeD6Over100(0.106, 0.02958);

			double molecularHydrogen = 0.0;
			double helium = 0.0;
			double nitrogen = 0.0;
			double argon = 0.0;
			double retentionFactor = 1.0;
			cout << "minMWR: " << minMWR << endl;

			// BS'd retention factor
			// Don't really know what to base this on other than AOW's tables, so I'm using mass as a proxy
			retentionFactor = pow(sPlanets2[i].GetMass(), 2.0);
			if (retentionFactor > 3.0) { retentionFactor = 3.0; }
			cout << "retentionFactor: " << retentionFactor << endl;
			if (minMWR <= 2) {
				molecularHydrogen = (0.9 + threeD6Over100(e)) * 100.0 * retentionFactor;
			}
			if (minMWR <= 4) {
				double kFactor = 1.0;
				if (minMWR <= 2) { kFactor = 25.0; }
				else if (minMWR <= 3) { kFactor = 5.0; }
				helium = (0.9 + threeD6Over100(e)) * kFactor * retentionFactor;
			}
			if (minMWR <= 28) {
				nitrogen = (0.9 + threeD6Over100(e)) * 0.7 * retentionFactor;
				if (blackBodyTemp <= 125 && oceanPctge == 1.0) {
					nitrogen *= 15.0;
				}
			}
			if (minMWR <= 40 && blackBodyTemp >= 90) {
				argon = (0.9 + threeD6Over100(e)) * 0.01 * retentionFactor * s.GetMetallicity();
			}

			// Reassign world classes
			// I'm assuming "significant" atmosphere is between Mars and Venus, or mass 0.07 (geomean), which is retentionFactor 0.005
			PlanetClass newPlanetClass = pc;
			if (thereWasARunawayGreenhouse) {
				newPlanetClass = VENUSIAN;
			}
			else if (molecularHydrogen > 0 && retentionFactor > 0.005) {
				newPlanetClass = HYCEAN;
			}
			else if (molecularHydrogen == 0 && nitrogen > 0 && blackBodyTemp >= 80 && blackBodyTemp <= 125 && retentionFactor > 0.005) {
				newPlanetClass = TITANIAN;
			}
			else if (molecularHydrogen == 0 && nitrogen > 0 && blackBodyTemp > 125 && retentionFactor > 0.005) {
				newPlanetClass = GAIAN;
			}
			else if (molecularHydrogen == 0 && nitrogen == 0 && helium == 0 && blackBodyTemp > 195 && retentionFactor > 0.005) {
				newPlanetClass = MARTIAN;
			}
			sPlanets2[i].SetPlanetClass(newPlanetClass);

			// albedo
			double albedo;
			albedo = threeD6Over100(e);

			if (newPlanetClass == VENUSIAN) {
				albedo += 0.65;
			}
			if (newPlanetClass == HYCEAN) {
				albedo += 0.20;
			}
			if (newPlanetClass == TITANIAN) {
				albedo += 0.10;
			}
			if (newPlanetClass == GAIAN || newPlanetClass == MARTIAN) {
				double oceans = sPlanets2[i].GetOceanPct();
				if (oceans = 0) { albedo += 0.15; }
				else if (oceans < 0.15) { albedo += 0.16; }
				else if (oceans < 0.65) { albedo += 0.19; }
				else if (oceans < 1.0) { albedo += 0.22; }
				else { albedo += 0.25; }
			}
			if (newPlanetClass == TERRESTRIAL_PLANET) {
				double oceans = sPlanets2[i].GetOceanPct();
				if (oceans = 0) { albedo += 0.01; }
				else if (oceans < 0.15) { albedo += 0.02; }
				else if (oceans < 0.65) { albedo += 0.08; }
				else if (oceans < 1.0) { albedo += 0.14; }
				else { albedo += 0.20; }
				// if temp < 80K add 0.3
			}
			sPlanets2[i].SetAlbedo(albedo);

			// First CO2 estimate
			double firstCO2Estimate = (0.90 + threeD6Over100(e)) * 10.0 * retentionFactor;
			if (newPlanetClass == VENUSIAN) {
				firstCO2Estimate = (0.90 + threeD6Over100(e)) * 100.0 * retentionFactor;
			}

			bool isACarbonSilicateCycle = false;
			int tValue = (blackBodyTemp * pow(1 - albedo, 0.25)) + (9.97 * log10(firstCO2Estimate)) + 31.8;
			if (tValue >= 260) { isACarbonSilicateCycle = true; }

			// life
			// I feel guilty for slavishly following AOW, so I'm only gonna do partial here
			// TBD: a better algorithm!
			bool thereIsLife = false;
			bool thereWasAnOxygenCatastrophe = false;
			double atmosphericOxygen = 0.0;
			double atmosphericWaterVapor = 0.0;
			if (newPlanetClass != VENUSIAN && (isACarbonSilicateCycle || oceanPctge > 0)) {
				if (s.GetAge() > 1) { thereIsLife = true; }
				if (s.GetAge() > 2) { thereWasAnOxygenCatastrophe = true; }
			}
			if (thereIsLife) {
				atmosphericOxygen = threeD6Over100(e) * 0.2;
			}
			else if (thereWasAnOxygenCatastrophe) {
				atmosphericOxygen = (threeD6Over100(e) + 0.15) * retentionFactor;
			}

			// surface temperature
			double averageSurfaceTemperature;
			if (newPlanetClass == VENUSIAN) {
				averageSurfaceTemperature = blackBodyTemp * pow(1.0 - albedo, 0.25) + (250.0 * log10(firstCO2Estimate));
			}
			else if (newPlanetClass == TERRESTRIAL_PLANET || retentionFactor == 0) {
				averageSurfaceTemperature = blackBodyTemp * pow(1.0 - albedo, 0.25);
			}
			else { // Gaian, etc surface temp
				averageSurfaceTemperature = blackBodyTemp * pow(1.0 - albedo, 0.25);

				double methaneGreenhouse = 0.0;
				double ozoneGreenhouse = 0.0;
				double carbonDioxideGreenhouse = 0.0;
				double waterGreenhouse = 0.0;

				if (newPlanetClass == HYCEAN || newPlanetClass == TITANIAN || (newPlanetClass == GAIAN && thereIsLife)) {
					methaneGreenhouse = 2.1 + (9.97 * log10(retentionFactor));
				}
				if (thereWasAnOxygenCatastrophe) {
					ozoneGreenhouse = 1.7 + (9.97 * log10(retentionFactor));
				}
				averageSurfaceTemperature += methaneGreenhouse;
				averageSurfaceTemperature += ozoneGreenhouse;

				// CO2 greenhouse and adjustment
				if (isACarbonSilicateCycle) {
					double minC = 260 - averageSurfaceTemperature;
					if (minC > 8.0) { carbonDioxideGreenhouse = minC; }
					else { carbonDioxideGreenhouse = 8.0; }

					// revise CO2 mass fraction
					firstCO2Estimate = 6.46e-4 * pow(10.0, carbonDioxideGreenhouse / 9.97);
				}
				else {
					carbonDioxideGreenhouse = 31.8 + (9.97 * log10(firstCO2Estimate));
				}

				averageSurfaceTemperature += carbonDioxideGreenhouse;

				// Water vapor greenhouse
				if (minMWR <= 18 && blackBodyTemp > 260 && oceanPctge > 0.15) {
					waterGreenhouse = getWaterGreenhouse(averageSurfaceTemperature, oceanPctge);

					atmosphericWaterVapor = 2.93e-5 * pow(10.0, waterGreenhouse / 9.97);
				}
				averageSurfaceTemperature += waterGreenhouse;
			} // END Gaian, etc surface temp
			// set surface temperature here
			sPlanets2[i].SetTemperature(averageSurfaceTemperature);

			// finalize atmosphere
			double atmosphericMass = molecularHydrogen + helium + nitrogen + argon + firstCO2Estimate + atmosphericOxygen + atmosphericWaterVapor;

			//double componentK = (1.0 / atmosphericMass) * ((2.0 * molecularHydrogen) + (4.0 * helium) + (18.0 * atmosphericWaterVapor) + (28.0 * nitrogen) + (32.0 * atmosphericOxygen) + (40.0 * argon) + (44.0 * firstCO2Estimate));
			//double scaleHeight = 0.856 * (averageSurfaceTemperature / (componentK * sPlanets2[i].GetGravity()));

			double atmosphericPressure = atmosphericMass * sPlanets2[i].GetGravity();

			Atmosphere atmos;
			atmos.hydrogen = molecularHydrogen / atmosphericMass;
			atmos.helium = helium / atmosphericMass;
			atmos.nitrogen = nitrogen / atmosphericMass;
			atmos.argon = argon / atmosphericMass;
			atmos.carbonDioxide = firstCO2Estimate / atmosphericMass;
			atmos.waterVapor = atmosphericWaterVapor / atmosphericMass;
			atmos.oxygen = atmosphericOxygen / atmosphericMass;
			atmos.pressure = atmosphericPressure;
			sPlanets2[i].SetAtmosphere(atmos);


		} // end if (pc == TERRESTRIAL_PLANET || pc == LEFTOVER_OLIGARCH)
	}

	return sPlanets2;
}

void placeRemainingPlanets (vector<FormingPlanet> & pVector, int firstPlanetIndex, int lastPlanetIndex, int countToBePlaced, default_random_engine & e) {
	double expectedRatio = pow(pVector[lastPlanetIndex].planet.GetDistance() / pVector[firstPlanetIndex].planet.GetDistance(), 1.0 / (countToBePlaced + 1));

	normal_distribution<> randomOrbitalRatio(1.025, 0.22); // TBD AOW p. 48
	for (int i = firstPlanetIndex + 1; i < lastPlanetIndex; i++) {
		// place planet
		double baseOrbitRatio = randomOrbitalRatio(e);
		double finalOrbitRatio = expectedRatio * baseOrbitRatio;
		if (finalOrbitRatio < 1.211) { finalOrbitRatio = 1.211; } // 4:3 resonance
		double lastDistance = pVector[i - 1].planet.GetDistance();
		pVector[i].planet.SetDistance(finalOrbitRatio * lastDistance);
		pVector[i].finalPlacement = true;
		// TBD: orbital resonance
	}
	return;
}

void printPlanetaryClass (PlanetClass pc, string & className, string & imgFileName) {
	switch(pc) {
		case NONE:
			className = "None";
			imgFileName = "";
			break;
		case FAILED_CORE:
			className = "Failed core";
			imgFileName = "GasDwarfPlanet.gif";
			break;
		case SMALL_GAS_GIANT:
			className = "Small gas giant";
			imgFileName = "Sub-JovianPlanet.gif";
			break;
		case MEDIUM_GAS_GIANT:
			className = "Medium gas giant";
			imgFileName = "JovianPlanet.gif";
			break;
		case LARGE_GAS_GIANT:
			className = "Large gas giant";
			imgFileName = "JovianPlanet.gif";
			break;
		case TERRESTRIAL_PLANET:
			className = "Terrestrial planet";
			imgFileName = "RockPlanet.gif";
			break;
		case PLANETOID_BELT:
			className = "Planetoid belt";
			imgFileName = "AsteroidsPlanet.gif";
			break;
		case LEFTOVER_OLIGARCH:
			className = "Leftover oligarch";
			imgFileName = "1FacePlanet.gif";
			break;
		case VENUSIAN:
			className = "Venusian";
			imgFileName = "1FacePlanet.gif";
			break;
		case HYCEAN:
			className = "Hycean";
			imgFileName = "1FacePlanet.gif";
			break;
		case TITANIAN:
			className = "Titanian";
			imgFileName = "1FacePlanet.gif";
			break;
		case GAIAN:
			className = "Gaian";
			imgFileName = "1FacePlanet.gif";
			break;
		case MARTIAN:
			className = "Martian";
			imgFileName = "1FacePlanet.gif";
			break;
	}
}

/* getWaterGreenhouse
 * This is my own regression on AOW's tables on p. 102.
 * Moderate: -430.418 + 80.4529 * ln(temp); r^2 = 0.99352
 * Excessive: -427.418 + 80.4529 * ln(temp)
 * Massive: -426.418 + 80.452917 * ln(temp)
 */
double getWaterGreenhouse (double temp, double ocean) {
	double returnThis;
	if (ocean < 0.65) {
		returnThis = -430.418 + 80.4529 * log(temp);
		if (temp > 334) { returnThis = 37.0; }
	}
	else if (ocean < 1) {
		returnThis = -427.418 + 80.4529 * log(temp);
		if (temp > 334) { returnThis = 40.0; }
	}
	else {
		returnThis = -426.418 + 80.4529 * log(temp);
		if (temp > 334) { returnThis = 41.0; }
	}

	return returnThis;
}
