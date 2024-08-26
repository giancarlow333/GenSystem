#ifndef PLANET_H
#define PLANET_H

#include <iostream>
using namespace std;

class Planet
{
	public:
		// Constructors
		Planet();
		Planet(const Planet & other);
		Planet(double d);
		// Accessors
		double GetMass ();
		// Mutators
		void SetMass (double m);
		// Operators
		Planet & operator=(const Planet & rhs);
		bool operator==(Planet a, Planet b)
	protected:
	private:
		double distance; // distance from star in AU
		double mass; // mass of planet in earths
		double radius; // radius of planet in earths	
};

#endif // PLANET_H
