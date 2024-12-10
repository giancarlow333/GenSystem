# GenSystem

This is a C++ console application which randomly generates a solar system and produces HTML output detailing that system.

## Functionality

TBD

## Usage

The program is compiled using the ```make``` command.  It is invoked from the command line once compiled.  Currently, the only way to change the output is to invoke with the **-s** command: ```./GenSystem -s SEED```, where SEED is an integer.

## Known Issues

* TBD

## Future Development

Paths for future development include:

* Multiple systems.  Currently, even if there are multiple stars, only a single system is generated.
* Random appearances of Nice events.  Currently, if there was a Grand Tack, there *will* be a Nice event too.
* Habitable gas giant moons.  Currently, moons are generally too small even under favorable conditions, *and* moon details aren't worked out in depth anyway.
* Giant impact moons
* A better algorithm to determine the presence of life (currently just age)
* A better algorithm for classifying planets.  In particular, a lot of planets marked "Gaian" could be better classified, as they aren't particularly earthlike.
* Orbital resonances between planets (not just moons)
* Forcing the appearance of a habitable planet ("-H command flag")

## Sources

* The overall procedure is generally borrowed from [*Architect of Worlds*](https://www.adastragames.com/products/architect-of-worlds), an RPG system by [Jon F. Zeigler](https://wordpress.sharrukinspalace.com/about-me/)—in some cases loosely, in others slavishly.  Indeed, AOW was the ultimate inspiration for this project.  However, it does not exactly duplictate is rules and is not intended for direct use with it.  No infringement is intended, and I hope to move away from it further in the future.
* Most of the icons for planets found in the system tables (and the ```/misc``` folder) are from [*StarGen*](https://www.eldacur.com/~brons/NerdCorner/StarGen/StarGen.html), an older solar system generation program written by [Jim Burrows](https://www.eldacur.com/~brons/) in 2003.  StarGen was a secondary inspiration for this project.
* The icon for hycean planets is derived from an image on [Wikipedia](https://en.wikipedia.org/wiki/File:Hyceanplanet.jpg) created [CC BY-SA 4.0](https://creativecommons.org/licenses/by-sa/4.0/deed.en) by [Pablo Carlos Budassi](https://commons.wikimedia.org/wiki/User:Celestialobjects).  The image was cropped and scaled down by me.

## License

This program is not currently licensed, as it is in **very** early development.

---

(c) 2024 Giancarlo Whitaker
