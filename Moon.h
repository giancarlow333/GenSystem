#ifndef MOON_H
#define MOON_H

#include <iostream>
using namespace std;

class Moon
{
	public:
		// Constructors
		Moon();
		Moon(const Moon & other);
		Moon(double d);
		Moon(double d, double m);
		// Accessors
		double GetDistance ();
		double GetEccentricity ();
		double GetMass ();
		double GetRadius ();
		double GetDensity ();
		double GetGravity ();
		// Mutators
		void SetDistance (double d);
		void SetEccentricity (double e);
		void SetMass (double m);
		void SetRadius (double r);
		void SetDensity (double rho);
		void SetGravity (double g);
		// Operators
		//Moon & operator=(const Moon & rhs);
		//bool operator==(Moon m);
	protected:
	private:
		double distance; // distance from star in *km*
		double eccentricity; // orbital eccentricity
		double mass; // mass of planet in earths
		double radius; // radius of planet in earths
		double density; // density of planet in earths
		double gravity; // gravity of planet in earths
};

#endif // MOON_H
