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
	primaryStar = other.primaryStar;
	secondaryStar = other.secondaryStar;
	planets = other.planets;
	separation = other.separation;
	eccentricity = other.eccentricity;
}
///////////////////////////////////////
// ACCESSORS
///////////////////////////////////////

Star StarSystem::GetPrimaryStar () {
	return primaryStar;
}

Star StarSystem::GetSecondaryStar () {
	return secondaryStar;
}

double StarSystem::GetSeparation () {
	return separation;
}

double StarSystem::GetEccentricity () {
	return eccentricity;
}

///////////////////////////////////////
// MUTATORS
///////////////////////////////////////

void StarSystem::SetPrimaryStar (Star s) {
	primaryStar = s;
}

void StarSystem::SetSecondaryStar (Star s) {
	secondaryStar = s;
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

