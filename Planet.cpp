#include <iostream>
#include <vector>
#include "Moon.h"
#include "Planet.h"
using namespace std;

Planet::Planet()
{
	distance = 0.0;
	eccentricity = 0.0;
	mass = 0.0;
	radius = 0.0;
	density = 0.0;
	gravity = 0.0;
	orbitalPeriod = 0.0;
	rotationPeriod = 0.0;
	axialTilt = 0.0;
	oceanPct = 0.0;
	albedo = 0.0;
	temperature = 0.0;
	classOfPlanet = NONE;
	atmosphere.hydrogen = 0.0;
	atmosphere.helium = 0.0;
	atmosphere.nitrogen = 0.0;
	atmosphere.argon = 0.0;
	atmosphere.carbonDioxide = 0.0;
	atmosphere.oxygen = 0.0;
	atmosphere.waterVapor = 0.0;
	atmosphere.pressure = 0.0;
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
	axialTilt = other.axialTilt;
	oceanPct = other.oceanPct;
	albedo = other.albedo;
	temperature = other.temperature;
	classOfPlanet = other.classOfPlanet;
	moons = other.moons;
	atmosphere = other.atmosphere;
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

double Planet::GetAxialTilt () {
	return axialTilt;
}

double Planet::GetOceanPct () {
	return oceanPct;
}

double Planet::GetAlbedo () {
	return albedo;
}

double Planet::GetTemperature () {
	return temperature;
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

Atmosphere Planet::GetAtmosphere () {
	return atmosphere;
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

void Planet::SetAxialTilt (double a) {
	axialTilt = a;
}

void Planet::SetOceanPct (double o) {
	oceanPct = o;
}

void Planet::SetAlbedo (double ab) {
	albedo = ab;
}

void Planet::SetTemperature (double t) {
	temperature = t;
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

void Planet::SetAtmosphere (Atmosphere a) {
	atmosphere = a;
}
///////////////////////////////////////
// OPERATORS
///////////////////////////////////////

/*Planet & Planet::operator=(const Planet & rhs) {
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
