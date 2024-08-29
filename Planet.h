#ifndef PLANET_H
#define PLANET_H

#include <iostream>
#include <vector>
#include "Moon.h"
using namespace std;

enum PlanetClass {
	NONE,
	FAILED_CORE,
	SMALL_GAS_GIANT,
	MEDIUM_GAS_GIANT,
	LARGE_GAS_GIANT,
	TERRESTRIAL_PLANET,
	PLANETOID_BELT,
	LEFTOVER_OLIGARCH,
	VENUSIAN,
	HYCEAN,
	TITANIAN,
	GAIAN,
	MARTIAN
};

struct Atmosphere {
	double hydrogen = 0.0;
	double helium = 0.0;
	double nitrogen = 0.0;
	double argon = 0.0;
	double carbonDioxide = 0.0;
	double oxygen = 0.0;
	double waterVapor = 0.0;
	double pressure = 0.0;
};

class Planet
{
	public:
		// Constructors
		Planet();
		Planet(const Planet & other);
		Planet(double d);
		Planet(double d, double m);
		// Accessors
		double GetDistance ();
		double GetEccentricity ();
		double GetMass ();
		double GetRadius ();
		double GetDensity ();
		double GetGravity ();
		double GetOrbitalPeriod ();
		double GetRotationPeriod ();
		double GetAxialTilt ();
		double GetOceanPct ();
		double GetAlbedo ();
		double GetTemperature ();
		PlanetClass GetPlanetClass ();
		vector<Moon> GetMoons ();
		int GetNumberOfMoons ();
		Atmosphere GetAtmosphere ();
		// Mutators
		void SetDistance (double d);
		void SetEccentricity (double e);
		void SetMass (double m);
		void SetRadius (double r);
		void SetDensity (double rho);
		void SetGravity (double g);
		void SetOrbitalPeriod (double y);
		void SetRotationPeriod (double r);
		void SetAxialTilt (double a);
		void SetOceanPct (double o);
		void SetAlbedo (double ab);
		void SetTemperature (double t);
		void SetPlanetClass (PlanetClass pc);
		void SetMoons (vector<Moon> mv);
		void SetSingleMoon (Moon m);
		void SetAtmosphere (Atmosphere a);
		// Operators
		//Planet & operator=(const Planet & rhs);
		//bool operator==(Planet b);
	protected:
	private:
		double distance; // distance from star in AU
		double eccentricity; // orbital eccentricity
		double mass; // mass of planet in earths
		double radius; // radius of planet in earths
		double density; // density of planet in earths
		double gravity; // gravity of planet in earths
		double orbitalPeriod; // year length in earth years
		double rotationPeriod; // day length in HOURS
		double axialTilt; // in degrees
		double oceanPct;
		double albedo;
		double temperature;
		PlanetClass classOfPlanet;
		vector<Moon> moons;
		Atmosphere atmosphere;
};

#endif // PLANET_H
