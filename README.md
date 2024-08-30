# GenSystem

This is a C++ console application which randomly generates a solar system.

## Warning!

The program currently crashes (returns a ```X``` error) while attempting to return from the ```formPlanets()``` function.  This only happens some of the time, and seems to depend on the formed system architecture (and thus ultimately on the seed).  I am roughly 90% certain that the cause of the crash is the program attempting to destruct the ```vector<FormingPlanet> sPlanets``` object.  However, I have *no idea* why this would cause a crash.  Until I resolve this issue, development is kinda paused.

Use the software at your own risk!

## Functionality

TBD

## Usage

TBD.  Program is invoked from the command line once compiled.
https://stackoverflow.com/questions/11246779/vector-push-back-not-working-in-class-object
## Future Development

Paths for future development include:

* Random appearances of Nice events.  Currently, if there was a Grand Tack, there *will* be a Nice event too.
* Habitable gas giant moons.  Currently, moons are generally too small even under favorable conditions, *and* moon details aren't worked out in depth anyway.
* Giant impact moons
* A better algorithm to determine the presence of life (currently just age)
* Orbital resonances between planets (not just moons)
* Forcing the appearance of a habitable planet ("-H command flag")

## License

This program is not currently licensed, as it is in **very** early development.

---

(c) 2024 Giancarlo Whitaker
