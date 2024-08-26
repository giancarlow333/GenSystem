#include <vector>
#include <iostream>
#include "Star.h"
#include "Planet.g"
using namespace std;

Star::StarSystem() {
	//ctor
}

StarSystem::StarSystem(const StarSystem& other) {
	stars = other.stars;
	planets = other.planets;
}
///////////////////////////////////////
// ACCESSORS
///////////////////////////////////////

vector<Star> StarSystem::GetStars () {
	return stars;
}

///////////////////////////////////////
// MUTATORS
///////////////////////////////////////

void StarSystem::SetStar (Star s) {
	stars.push_back(s);
}

void StarSystem::SetStar (vector<Star> vs) {
	stars = vs;
}

///////////////////////////////////////
// OPERATORS
///////////////////////////////////////

StarSystem & StarSystem::operator=(const StarSystem & rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}

