#include <cmath>
#include <sstream>
#include "main.h"

static const int N_BITS = 32;


/**
 * (y_i) sequence of random 32bit ints
 * (x_i) given by:
 * x_0 = 0
 * x_k = x_k-1 | y_k-1
 * For any (y_i) there is N s.t. i>=N => x_i = 2^32-1 (all bits set)
 * What is expected value of N to 10 d.p.?
 *
 * Consider a single bit "b" at step i
 * P(bi = 0) = (1/2)^i
 * P(bi = 1) = 1 - (1/2)^i
 *
 * n = number of bits
 * Write -1 for "all bits set"
 * P(x_i = -1) = P(all bi = 1) = (1 - (1/2)^i)^n
 * P(x_i != -1) = 1 - (1 - (1/2)^i)^n
 *
 * Note that P(N > v) = P(x_v != -1) for any v
 * E(N) = sum(P(N > v) for v=0 to infinity)
 */
std::string solution() {
    double sum = 0;
    double limit = 0.00000000001;
    double term = 1;
    int i = 1;
    while (term > limit) {
        sum += term;
        double v = 1.0 - (1.0 / std::pow(2, i));
        term = 1 - std::pow(v, N_BITS);
        i += 1;
    }
    std::ostringstream s;
    s.precision(10);
    s << std::fixed << sum;
    return s.str();
}
