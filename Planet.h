#ifndef PLANET_H
#define PLANET_H

#include <iostream>
using namespace std;

enum PlanetClass {
	NONE,
	FAILED_CORE,
	SMALL_GAS_GIANT,
	MEDIUM_GAS_GIANT,
	LARGE_GAS_GIANT,
	TERRESTRIAL_PLANET,
	PLANETOID_BELT,
	LEFTOVER_OLIGARCH
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
		PlanetClass GetPlanetClass ();
		// Mutators
		void SetDistance (double d);
		void SetEccentricity (double e);
		void SetMass (double m);
		void SetRadius (double r);
		void SetDensity (double rho);
		void SetGravity (double g);
		void SetPlanetClass (PlanetClass pc);
		// Operators
		Planet & operator=(const Planet & rhs);
		bool operator==(Planet b);
	protected:
	private:
		double distance; // distance from star in AU
		double eccentricity; // orbital eccentricity
		double mass; // mass of planet in earths
		double radius; // radius of planet in earths
		double density; // density of planet in earths
		double gravity; // gravity of planet in earths
		PlanetClass classOfPlanet;
};

#endif // PLANET_H
