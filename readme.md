##Project Euler

Commonly used code for Project Euler solutions organised into C++ header files, along with some actual C++ solutions.

###Compiling a solution
Assumes g++ exists with -std=C++0x

```
cd [git_repo]/src
build.py [solution number (3 digits)]
```

The compiled executable will then be placed into the bin directory.

###Repository Contains
This repository contains some of my Project Euler solutions, header files containing frequently used code, and a Python script that will compile the specified problem number.

Also included is a base.cpp file that includes the basic timing code in an otherwise empty main method.

Tests are included for some of the generic code. They can be run in the same way as the solutions:

More recent solutions will often begin with a comment block explaining the idea behind the solution.

```
build.py peCalculationTest
./../bin/peCalculationTest
```

###Reason for existence
I got annoyed trawling through old Project Euler solutions looking for code I wanted to reuse.
So I decided to move to working out of a single repository with frequently used snippets (prime number sieves, permutation generators, etc) moved into header files that can easily be included.

Not all of my Project Euler solutions are included.
Many were solved in a different language or the code has been lost to the ages (my first PE problem was solved 7 years ago).
Many others have simply not been converted to use the new templated code in the header files.

(Some aspects of the code are of questionable quality - I'm looking at you peLargeInts.h).
