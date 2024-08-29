#ifndef STAR_H
#define STAR_H

#include <vector>
#include <iostream>
#include "Planet.h"
using namespace std;

class Star
{
	public:
		// Constructors
		Star();
		Star(double m);
		Star(const Star & other);
		// Accessors
		double GetMass ();
		double GetAge ();
		double GetMetallicity ();
		double GetTemperature ();
		double GetLuminosity ();
		double GetRadius ();
		string GetSpectralType ();
		string GetLuminosityClass ();
		// Mutators
		void SetMass (double m);
		void SetAge (double a);
		void SetMetallicity (double met);
		void SetTemperature (double t);
		void SetLuminosity (double l);
		void SetRadius (double r);
		void SetSpectralType (string st);
		void SetLuminosityClass (string lc);
		// Operators
		Star & operator=(const Star & rhs);
		//bool operator==(Star a, Star b)
	protected:
	private:
		double mass;
		double age;
		double metallicity;
		double temperature;
		double luminosity;
		double radius;
		string spectralType;
		string luminosityClass;
};

#endif // STAR_H
