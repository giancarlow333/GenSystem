#include <vector>
#include <iostream>
#include "StarSystem.h"
#include "Star.h"
#include "Planet.h"
using namespace std;

StarSystem::StarSystem() {
	//ctor
	separation = 0;
	eccentricity = 0;
}

StarSystem::StarSystem(const StarSystem& other) {
	stars = other.stars;
	planets = other.planets;
	separation = other.separation;
	eccentricity = other.eccentricity;
}
///////////////////////////////////////
// ACCESSORS
///////////////////////////////////////

vector<Star> StarSystem::GetStars () {
	return stars;
}

double StarSystem::GetSeparation () {
	return separation;
}

double StarSystem::GetEccentricity () {
	return eccentricity;
}

int StarSystem::GetNumberOfStars () {
	return stars.size();
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

void StarSystem::SetSeparation (double s) {
	separation = s;
}

void StarSystem::SetEccentricity (double e) {
	eccentricity = e;
}

///////////////////////////////////////
// OPERATORS
///////////////////////////////////////

StarSystem & StarSystem::operator=(const StarSystem & rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}

