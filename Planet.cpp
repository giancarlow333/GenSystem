#include <iostream>
#include <vector>
#include "Moon.h"
#include "Planet.h"
using namespace std;

Planet::Planet()
{
	//ctor
}

Planet::Planet(const Planet & other) {
	distance = other.distance;
	eccentricity = other.eccentricity;
	mass = other.mass;
	radius = other.radius;
	density = other.density;
	gravity = other.gravity;
	orbitalPeriod = other.orbitalPeriod;
	rotationPeriod = other.rotationPeriod;
	classOfPlanet = other.classOfPlanet;
	moons = other.moons;
}

//alternate constructor
Planet::Planet(double d) {
	distance = d;
}

//alternate constructor 2
Planet::Planet(double d, double m) {
	distance = d;
	mass = m;
}

///////////////////////////////////////
// ACCESSORS
///////////////////////////////////////

double Planet::GetDistance () {
	return distance;
}

double Planet::GetEccentricity () {
	return eccentricity;
}

double Planet::GetMass () {
	return mass;
}

double Planet::GetRadius () {
	return radius;
}

double Planet::GetDensity () {
	return density;
}

double Planet::GetGravity () {
	return gravity;
}

double Planet::GetOrbitalPeriod () {
	return orbitalPeriod;
}

double Planet::GetRotationPeriod () {
	return rotationPeriod;
}

PlanetClass Planet::GetPlanetClass () {
	return classOfPlanet;
}

vector<Moon> Planet::GetMoons () {
	return moons;
}

int Planet::GetNumberOfMoons () {
	return moons.size();
}
///////////////////////////////////////
// MUTATORS
///////////////////////////////////////

void Planet::SetDistance (double d) {
	distance = d;
}

void Planet::SetEccentricity (double e) {
	eccentricity = e;
}

void Planet::SetMass (double m) {
	mass = m;
}

void Planet::SetRadius (double r) {
	radius = r;
}

void Planet::SetDensity (double rho) {
	density = rho;
}

void Planet::SetGravity (double g) {
	gravity = g;
}


void Planet::SetOrbitalPeriod (double y) {
	orbitalPeriod = y;
}

void Planet::SetRotationPeriod (double r) {
	rotationPeriod = r;
}

void Planet::SetPlanetClass (PlanetClass pc) {
	classOfPlanet = pc;
}

void Planet::SetMoons (vector<Moon> mv) {
	moons = mv;
}
void Planet::SetSingleMoon (Moon m) {
	moons.push_back(m);
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
