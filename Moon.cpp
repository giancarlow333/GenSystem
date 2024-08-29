#include <iostream>
#include "Moon.h"
using namespace std;

Moon::Moon()
{
	distance = 0.0;
	eccentricity = 0.0;
	mass = 0.0;
	radius = 0.0;
	density = 0.0;
	gravity = 0.0;
}

Moon::Moon(const Moon & other) {
	distance = other.distance;
	eccentricity = other.eccentricity;
	mass = other.mass;
	radius = other.radius;
	density = other.density;
	gravity = other.gravity;
}

//alternate constructor
Moon::Moon(double d) {
	distance = d;
}

//alternate constructor 2
Moon::Moon(double d, double m) {
	distance = d;
	mass = m;
}

///////////////////////////////////////
// ACCESSORS
///////////////////////////////////////

double Moon::GetDistance () {
	return distance;
}

double Moon::GetEccentricity () {
	return eccentricity;
}

double Moon::GetMass () {
	return mass;
}

double Moon::GetRadius () {
	return radius;
}

double Moon::GetDensity () {
	return density;
}

double Moon::GetGravity () {
	return gravity;
}
///////////////////////////////////////
// MUTATORS
///////////////////////////////////////

void Moon::SetDistance (double d) {
	distance = d;
}

void Moon::SetEccentricity (double e) {
	eccentricity = e;
}

void Moon::SetMass (double m) {
	mass = m;
}

void Moon::SetRadius (double r) {
	radius = r;
}

void Moon::SetDensity (double rho) {
	density = rho;
}

void Moon::SetGravity (double g) {
	gravity = g;
}
///////////////////////////////////////
// OPERATORS
///////////////////////////////////////

Moon & Moon::operator=(const Moon & rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}

/*bool operator==(Moon a, Moon b) {
	return (a.mass == b.mass) && (a.age == b.age) && (a.metallicity == b.metallicity) && (a.temperature == b.temperature) && (a.luminosity == b.luminosity) && (a.radius == b.radius);
}

/*
ostream& operator<<(ostream & outs, const Moon & Moon)
{
    //string L =
    outs << Moon.GetSpectralClass();
    outs << Moon.GetSubClass();
    outs << Moon.GetLuminosityClass();
}*/
