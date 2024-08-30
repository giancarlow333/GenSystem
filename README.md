# GenSystem

This is a C++ console application which randomly generates a solar system.

## Warning!

The program currently crashes (returns a ```malloc(): invalid next size (unsorted)``` error) while attempting to return from the ```formPlanets()``` function.  This only happens some of the time, and seems to depend on the formed system architecture (and thus ultimately on the seed).  I am roughly 90% certain that the cause of the crash is the program attempting to destruct the ```vector<FormingPlanet> sPlanets``` object.  However, I have *no idea* why this would cause a crash.  Until I resolve this issue, development is kinda paused.

Use the software at your own risk!

## Functionality

TBD

## Usage

The program is compiled using the ```make``` command.  It is invoked from the command line once compiled.  Currently, the only way to change the output is to invoke with the **-s** command: ```./GenSystem -s SEED```, where SEED is an integer.

## Future Development

Paths for future development include:

* Random appearances of Nice events.  Currently, if there was a Grand Tack, there *will* be a Nice event too.
* Habitable gas giant moons.  Currently, moons are generally too small even under favorable conditions, *and* moon details aren't worked out in depth anyway.
* Giant impact moons
* A better algorithm to determine the presence of life (currently just age)
* Orbital resonances between planets (not just moons)
* Forcing the appearance of a habitable planet ("-H command flag")

## Sources

The overall procedure is generally taken from [*Architect of Worlds*](https://www.adastragames.com/products/architect-of-worlds), an RPG system by [Jon F. Zeigler](https://wordpress.sharrukinspalace.com/about-me/)—in some cases loosely, in others slavishly.  Indeed, AOW was the ultimate inspiration for this project.  However, it does not exactly duplictate is rules and is not intended for direct use with it.  No infringement is intended, and I hope to move away from it further in the future.

## License

This program is not currently licensed, as it is in **very** early development.

---

(c) 2024 Giancarlo Whitaker
