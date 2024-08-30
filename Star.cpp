#include <utility>  // for overloading remaining comparison operators
#include <string>
#include <iostream>
#include "Star.h"
#include "Planet.h"
using namespace std;

Star::Star()
{
	//ctor
}

Star::Star(const Star& other) {
	mass = other.mass;
	age = other.age;
	metallicity = other.metallicity;
	temperature = other.temperature;
	luminosity = other.luminosity;
	radius = other.radius;
}

//alternate constructor
Star::Star(double m)
{
    mass = m;
}

///////////////////////////////////////
// ACCESSORS
///////////////////////////////////////

double Star::GetMass () {
	return mass;
}

double Star::GetAge () {
	return age;
}

double Star::GetMetallicity () {
	return metallicity;
}

double Star::GetTemperature () {
	return temperature;
}

double Star::GetLuminosity () {
	return luminosity;
}

double Star::GetRadius () {
	return radius;
}

string Star::GetSpectralType () {
	return spectralType;
}

string Star::GetLuminosityClass () {
	return luminosityClass;
}
/*char Star::GetSpectralClass () const
{   // BAFGKMLTDX
    switch (spectral) {
        case 0 : return 'B';
        case 1 : return 'A';
        case 2 : return 'F';
        case 3 : return 'G';
        case 4 : return 'K';
        case 5 : return 'M';
        case 6 : return 'L';
        case 7 : return 'T';
        case 8 : return 'D';
        case 9 : return 'X';
    }
}

int Star::GetSubClass () const
{
    return sub;
}

string Star::GetLuminosityClass () const
{   // III, IV, V
    switch (luminos) {
        case 0 : return "III";
        case 1 : return "IV";
        case 2 : return "V";
        case -1 : return "";
    }
}

string GetStar ()
{
    string sp, sb, l;
    sp = GetSpectralClass();
    sb = GetSubClass();
    l = GetLuminosityClass();

}*/

///////////////////////////////////////
// MUTATORS
///////////////////////////////////////

void Star::SetMass (double m) {
	mass = m;
}

void Star::SetAge (double a) {
	age = a;
}

void Star::SetMetallicity (double met) {
	metallicity = met;
}

void Star::SetTemperature (double t) {
	temperature = t;
}

void Star::SetLuminosity (double l) {
	luminosity = l;
}

void Star::SetRadius (double r) {
	radius = r;
}

void Star::SetSpectralType (string st) {
	spectralType = st;
}
void Star::SetLuminosityClass (string lc) {
	luminosityClass = lc;
}
///////////////////////////////////////
// OPERATORS
///////////////////////////////////////

Star & Star::operator=(const Star & rhs) {
	if (this == &rhs) return *this; // handle self assignment
	//assignment operator
	return *this;
}

/*bool operator==(Star a, Star b) {
	return (a.mass == b.mass) && (a.age == b.age) && (a.metallicity == b.metallicity) && (a.temperature == b.temperature) && (a.luminosity == b.luminosity) && (a.radius == b.radius);
}

/*
ostream& operator<<(ostream & outs, const Star & star)
{
    //string L =
    outs << star.GetSpectralClass();
    outs << star.GetSubClass();
    outs << star.GetLuminosityClass();
}*/
