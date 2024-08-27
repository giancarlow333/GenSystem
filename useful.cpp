#include <string>
#include <iostream>
#include "Planet.h"
using namespace std;

string GetSpectralClass (double temp) {
	if (temp <= 600) { return "Y0"; }
	else if (temp <= 725) { return "T9"; }
	else if (temp <= 775) { return "T8"; }
	else if (temp <= 825) { return "T7"; }
	else if (temp <= 875) { return "T6"; }
	else if (temp <= 925) { return "T5"; }
	else if (temp <= 975) { return "T4"; }
	else if (temp <= 1050) { return "T3"; }
	else if (temp <= 1150) { return "T2"; }
	else if (temp <= 1250) { return "T1"; }
	else if (temp <= 1350) { return "T0"; }

	else if (temp <= 1450) { return "L9"; }
	else if (temp <= 1550) { return "L8"; }
	else if (temp <= 1650) { return "L7"; }
	else if (temp <= 1750) { return "L6"; }
	else if (temp <= 1850) { return "L5"; }
	else if (temp <= 1950) { return "L4"; }
	else if (temp <= 2050) { return "L3"; }
	else if (temp <= 2150) { return "L2"; }
	else if (temp <= 2250) { return "L1"; }
	else if (temp <= 2350) { return "L0"; }

	else if (temp <= 2450) { return "M9"; }
	else if (temp <= 2575) { return "M8"; }
	else if (temp <= 2725) { return "M7"; }
	else if (temp <= 2900) { return "M6"; }
	else if (temp <= 3100) { return "M5"; }
	else if (temp <= 3300) { return "M4"; }
	else if (temp <= 3475) { return "M3"; }
	else if (temp <= 3625) { return "M2"; }
	else if (temp <= 3775) { return "M1"; }
	else if (temp <= 3910) { return "M0"; }

	else if (temp <= 4030) { return "K9"; }
	else if (temp <= 4150) { return "K8"; }
	else if (temp <= 4270) { return "K7"; }
	else if (temp <= 4390) { return "K6"; }
	else if (temp <= 4535) { return "K5"; }
	else if (temp <= 4705) { return "K4"; }
	else if (temp <= 4875) { return "K3"; }
	else if (temp <= 5045) { return "K2"; }
	else if (temp <= 5215) { return "K1"; }
	else if (temp <= 5330) { return "K0"; }

	else if (temp <= 5390) { return "G9"; }
	else if (temp <= 5450) { return "G8"; }
	else if (temp <= 5510) { return "G7"; }
	else if (temp <= 5570) { return "G6"; }
	else if (temp <= 5630) { return "G5"; }
	else if (temp <= 5690) { return "G4"; }
	else if (temp <= 5750) { return "G3"; }
	else if (temp <= 5810) { return "G2"; }
	else if (temp <= 5870) { return "G1"; }
	else if (temp <= 5960) { return "G0"; }

	else if (temp <= 6080) { return "F9"; }
	else if (temp <= 6200) { return "F8"; }
	else if (temp <= 6320) { return "F7"; }
	else if (temp <= 6440) { return "F6"; }
	else if (temp <= 6570) { return "F5"; }
	else if (temp <= 6710) { return "F4"; }
	else if (temp <= 6850) { return "F3"; }
	else if (temp <= 6990) { return "F2"; }
	else if (temp <= 7130) { return "F1"; }
	else if (temp <= 7300) { return "F0"; }

	else if (temp <= 7500) { return "A9"; }
	else if (temp <= 7700) { return "A8"; }
	else if (temp <= 7900) { return "A7"; }
	else if (temp <= 8100) { return "A6"; }
	else if (temp <= 8350) { return "A5"; }
	else if (temp <= 8650) { return "A4"; }
	else if (temp <= 8950) { return "A3"; }
	else if (temp <= 9250) { return "A2"; }
	else if (temp <= 9550) { return "A1"; }
	else { return "A0"; }
}

int getAccretionModifier (double pMass) {
	if (pMass <= 2) { return 0; }
	else if (pMass <= 2.38) { return 2; }
	else if (pMass <= 2.83) { return 4; }
	else if (pMass <= 3.36) { return 6; }
	else if (pMass <= 4.00) { return 8; }
	else if (pMass <= 4.75) { return 10; }
	else if (pMass <= 5.66) { return 12; }
	else if (pMass <= 6.73) { return 14; }
	else if (pMass <= 8.00) { return 16; }
	else if (pMass <= 9.51) { return 18; }
	else if (pMass <= 11.31) { return 20; }
	else if (pMass <= 13.45) { return 22; }
	else if (pMass <= 16.00) { return 24; }
	else if (pMass <= 19.03) { return 26; }
	else if (pMass <= 22.62) { return 28; }
	else if (pMass <= 26.91) { return 30; }
	else if (pMass <= 32.00) { return 32; }
	else if (pMass <= 38.05) { return 34; }
	else if (pMass <= 45.25) { return 36; }
	else if (pMass <= 53.82) { return 38; }
	else if (pMass <= 64) { return 40; }
	else {
		int log2pMass = log(pMass) / log(2);
		return 40 + 8 * (log2pMass - 6);
	}
}


