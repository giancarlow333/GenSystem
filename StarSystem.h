#ifndef STARSYSTEM_H
#define STARSYSTEM_H

#include <vector>
#include <iostream>
#include "Star.h"
#include "Planet.h"
using namespace std;

class StarSystem {
	public:
		// Constructors
		StarSystem();
		StarSystem(const StarSystem & other);
		// Accessors
		Star GetPrimaryStar ();
		Star GetSecondaryStar ();
		double GetSeparation ();
		double GetEccentricity ();
		// Mutators
		void SetPrimaryStar (Star s);
		void SetSecondaryStar (Star s);
		void SetSeparation (double s);
		void SetEccentricity (double e);
		// Operators
		StarSystem & operator=(const StarSystem & rhs);
		//bool operator==(StarSystem a, StarSystem b)
	protected:
	private:
		Star primaryStar;
		Star secondaryStar;
		vector<Planet> planets; // planets that orbit *all* stars in system!
		// Relationship between stars
		double separation;
		double eccentricity;
};

#endif // STARSYSTEM_H
