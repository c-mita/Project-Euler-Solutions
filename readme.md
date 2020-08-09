## Project Euler

Commonly used code for Project Euler solutions organised into C++ header files, along with some actual C++ solutions.

### Running solutions
Assumes a "modern" g++.

To compile and run a solution:
```
make [solution number (3 digits)]
```
To simply compile:
```
make bin/[solution number]
```
The compiled executable will then be placed into the bin directory.

### Repository Contains
This repository contains some of my Project Euler solutions, header files containing frequently used code, and a Makefile.

Also included is a base.cpp file that includes the basic timing code in an otherwise empty main method.

Tests are included for some of the generic code. They can be run in the same way as the solutions:

```
make bin/peCalculationTest
./bin/peCalculationTest
```

More recent solutions will often begin with a comment block explaining the idea behind the solution.

### Reason for existence
I got annoyed trawling through old Project Euler solutions looking for code I wanted to reuse.
So I decided to move to working out of a single repository with frequently used snippets (prime number sieves, permutation generators, etc) moved into header files that can easily be included.

Not all of my Project Euler solutions are included.
Many were solved in a different language or the code has been lost to the ages (my first PE problem was solved at school in 2008).
Many others have simply not been converted to use the new templated code in the header files.
