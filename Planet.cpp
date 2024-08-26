#include <iostream>
#include "Planet.h"
using namespace std;

Planet::Planet()
{
	//ctor
}

Planet::Planet(const Planet & other) {
	distance = other.distance;
	mass = other.mass;
	radius = other.radius;
}

//alternate constructor
Planet::Planet(double d) {
	distance = d;
}

///////////////////////////////////////
// ACCESSORS
///////////////////////////////////////

double Planet::GetMass () {
	return mass;
}

///////////////////////////////////////
// MUTATORS
///////////////////////////////////////

void Planet::SetMass (double m) {
	mass = m;
}

///////////////////////////////////////
// OPERATORS
///////////////////////////////////////

Planet & Planet::operator=(const Planet & rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}

/*bool operator==(Planet a, Planet b) {
	return (a.mass == b.mass) && (a.age == b.age) && (a.metallicity == b.metallicity) && (a.temperature == b.temperature) && (a.luminosity == b.luminosity) && (a.radius == b.radius);
}

/*
ostream& operator<<(ostream & outs, const Planet & Planet)
{
    //string L =
    outs << Planet.GetSpectralClass();
    outs << Planet.GetSubClass();
    outs << Planet.GetLuminosityClass();
}*/
