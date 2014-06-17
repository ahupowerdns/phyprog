phyprog
=======

Code &amp; Notes related to the Physics Programming Workshop 

For more information, please see [here](http://ds9a.nl/phyprog/)

Concept
=======
The concept is a three dimensional array of sensors, which individually can
detect particles with a certain sensitivity and specificity.

Particles cross the sensors in a straight line, affecting sensors close to
their path.

Each sensor emits a value between 0 and 255, indicating a voltage or a
charge, and this value is impacted by the passing of a particle. 

The challenge for our software: from x * y * z reported sensor values,
determine if any particles where seen, and their trajectories.

As a simple example with z = 1:

	0   0 0 0 0 0 0 1 0
	1   0 0 0 0 0 1 0 0
	2   0 0 0 0 1 0 0 0
	3   0 0 0 1 0 0 0 0 
	4   0 0 1 0 0 0 0 0
	5   0 1 0 0 0 0 0 0
	6   1 0 0 0 0 0 0 0
	7   0 0 0 0 0 0 0 0

	    0 1 2 3 4 5 6 7 
   
In this case, the particle intersects the edge of our network at (0,6) and
at (6,0), and this should be the output of the software.

Making it more complicated
==========================

1. Each sensor actually reports a Poisson distributed value, even if no
particle passes by
2. Depending on their path, particles can pass closer to farther away from detectors
3. Detectors are not positioned perfectly or conversely, particles may bend
a bit

Challenge
=========

Several data files are available, in ascending size and difficulty. The
larger and more difficult files are deliberatly unsolvable, the smaller ones
are almost trivial. This is so that programmers at all levels can compete.

Some data files do not in fact contain tracks.

Prizes are awarded not for raw speed or largest data set solved, but for
being good, where good is roughly measured according to [the 20 programming
rules](https://docs.google.com/document/d/1hFO5d5LnLUSI72v6VWAXuoPtDG6N2TOkfypcM9d2x5Q/edit?usp=sharing)

Details
=======
Coordinate system is of course arbitrary, but it helps if we mentally align it identically.
(0,0,0) is the leftmost and hindmost part of a cube.


     (0,0,0)           (1,0,0)
        +----------------+
       /                /|
      /                / |
     /                /  |
    +----------------+   |
    |                |   |
    |                |   | 
    |                |   + (1,0,1)
    |                |  /
    | (0,1,1)        | /
    +----------------+/  (1,1,1)

The file format starts with three numbers in ascii describing X (width), Y (depth) and Z (height) dimensions, 
followed by a single newline. 
Then follow x * y * z bytes, starting with z=0, y=0 and then x=0...width-1 as raw bytes (0-255).

So to find value (x,y,z), read the first line with dimensions, and then seek z * (width * depth) + y * width +x bytes.

Installing
==========
Either install libeigen3-dev (Debian, Ubuntu), or:

    $ wget http://bitbucket.org/eigen/eigen/get/3.2.1.tar.bz2
    $ tar xf 3.2.1.tar.bz2
    $ ln -s eigen-eigen-*/Eigen .

Tools
=====
	gendata x y z signal noise human

x, y and z are width, depth and height. Signal is the strength of a full on
detection of a particle.  Noise is the Poisson mean value of no detection. 
Human is '0' or '1' for binary or human friendly output.  Gendata prints out
the coordinates of where the random particle ray enters and leaves the cube.

 
	rdata datafile x1 y1 z1 x2 y2 z2

Analyzes a datafile made by gendata, and tries how many points a line through 
(x1,y1,z1) to (x2,y2,z2) scores. 
