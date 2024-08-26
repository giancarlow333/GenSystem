#include <vector>
#include <iostream>
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"
using namespace std;

StarSystem::StarSystem() {
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

void StarSystem::SetSingleStar (Star s) {
	stars.push_back(s);
}

void StarSystem::SetStars (vector<Star> vs) {
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

