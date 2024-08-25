#include <iostream>
#include <random>
using namespace std;

int main () {
	cout << "Hello!\n\n";
	cout << "Welcome to GenSystem Version 0.01!" << endl;
	cout << "(c) 2024 Giancarlo Whitaker" << endl << endl;

	// construct random engine
	int seed = 333;  // non-random seed for testing
	default_random_engine engine(seed);

	return 0;
}
