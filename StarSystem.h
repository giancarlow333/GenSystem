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
		vector<Star> GetStars ();
		double GetSeparation ();
		double GetEccentricity ();
		// Mutators
		void SetSingleStar (Star s);
		void SetStars (vector<Star> vs);
		void SetSeparation (double s);
		void SetEccentricity (double e);
		// Operators
		StarSystem & operator=(const StarSystem & rhs);
		//bool operator==(StarSystem a, StarSystem b)
	protected:
	private:
		vector<Star> stars;
		vector<Planet> planets;
		// Relationship between stars
		double separation;
		double eccentricity;
};

#endif // STARSYSTEM_H
