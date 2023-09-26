#include <iomanip>
#include <sstream>
#include "main.h"


/*
 * Problem description:
 * A firecracker explodes at a height of 100 m above level ground.
 * It breaks into a large number of very small fragments, which move in every
 * direction; all of them have the same initial velocity of 20 m/s.
 *
 * We assume that the fragments move without air resistance, in a uniform
 * gravitational field with g = 9.81 m/s^2.
 *
 * Find the volume (in m) of the region through which the fragments move before
 * reaching the ground. Give your answer rounded to four decimal places.
 */

/*
 * This is solved with simple formula derived via calculus.
 * Consider the 2D case, with the firecracker at x=0 and y=100.
 * The set of all ballistic trajectories form a convex hull which can be
 * described as a parabola.
 *
 * The formula for this hull is:
 * f(x) = h + (v^2 / 2g) - x^2 * (g / 2*v^2)
 * Where:
 * h = initial height (100)
 * v = initial velocity (20)
 * g = gravity (9.81)
 *
 * We then apply a volume-of-rotation integral to calculate the solution.
 * V = 2*pi * Integral(x * f(x) dx) between 0 and m
 * where m is the solution to f(x) = 0
 */

std::string solution() {
    const double h = 100;
    const double v = 20;
    const double g = 9.81;

    // If we pretend f(x) is of the form f(x) = a - bx^2, it's really easy to
    // solve.
    const double a = h + (v * v) / (2 * g);
    const double b = g / (2 * v * v);

    // The integral resolves to (pi * a^2) / (2 * b)
    const double pi = 3.141592653589793;
    const double volume = pi * a * a / (2 * b);

    std::stringstream ss;
    ss << std::fixed << std::setprecision(4) << volume;
    return ss.str();
}
