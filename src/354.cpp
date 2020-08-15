#include <algorithm>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
#include "pePrimes.h"
#include "main.h"

typedef long long int int64;
typedef boost::multiprecision::cpp_int bigint;

static const int64 LIMIT = 500000000000ll;
static const int64 LIMIT_SCALED = LIMIT / std::sqrt(3);

/**
 * Based on https://arxiv.org/abs/math/0508201 (https://arxiv.org/pdf/math/0508201.pdf) - Oscar Marmon
 * Page 8 is the important bit.
 *
 * Given a radius r, write r^2:
 * r^2 = 3^b * p_1^a_1 * p_2*a_2 * ... * p_n^a_n * q_1^c_1 * ... * q_n^c_n
 * Where p_i = 1 mod 3 and q_i = 2 mod 3
 *
 * From the paper, we have the number of hexagonal lattice points on a circle of radius r, l(r):
 * l(r) = 6 * product(a_i + 1)
 * Where a_i are the exponents of p_i in the prime factorisation of r^2 where p_i = 1 mod 3
 * We are also told that c_i are even when r is solvable.
 *
 * l(r) = 450 -> 75 = product(a_i + 1)
 * 75 has only a few decompositions
 * 75 = 75
 * 75 = 3 * 25
 * 75 = 5 * 15
 * 75 = 3 * 5 * 5
 *
 * Notice that therefore, the corresponding exponents (a_i) are all even.
 * Combined with the fact that all c_n must be even (from the paper), we can write:
 *
 * r^2 = 3^b * g^2 * c^2
 * g = p_1^e_1 * ... * p_n^e_n
 * c = product of primes that are 2 modulo 3
 *
 * For a given solution g, multiplying by any c or 3^b will also be a solution.
 *
 * Our strategy is to generate all possible cases for g. These are our "base" cases.
 * For each g, and possible c (up to the limit), we check to see how many possible values there are for b.
 * r = 3^(b/2) * g * c
 * b = 2 * log3(r / (g * c))
 * (Note that b = 4 -> 3^0, 3^1, 3^2, 3^3, and 3^4 are all possible, so it contributes 5 counts.)
 *
 * Note the paper operates on a lattice consisting of vertex and centre points.
 * The PE problem only consists of central points. However the two differ only by a factor of sqrt(3) (and a rotation).
 * We scale our target limit by sqrt(3) and pretend we're working with the denser lattice.
 */


// Find the "basic" cases
std::vector<int64> calc_generators(std::vector<int64> &primes_1) {
    std::vector<int64> generators;

    // get generators for exponents 2, 4, 4
    // we're going to look at "sqrt(g)" instead of "g" because our exponents are all even
    for (auto it = primes_1.begin(); it < primes_1.end(); ++it) {
        bigint p1 = *it;
        for (auto jt = primes_1.begin(); jt < primes_1.end(); ++jt) {
            if (jt == it) continue;
            bigint p2 = *jt * *jt;
            if (p2 > LIMIT_SCALED || p1 * p2 > LIMIT_SCALED) {
                break;
            }
            for (auto kt = jt + 1; kt < primes_1.end(); ++kt) {
                if (kt == it) continue;
                bigint p3 = *kt * *kt;
                bigint p = p1 * p2 * p3;
                if (p > LIMIT_SCALED) {
                    break;
                };
                generators.push_back((int64) p);
            }
        }
    }

    // get generators for exponents 14 and 4
    for (bigint p1 : primes_1) {
        bigint s1 = p1 * p1;
        if (s1 > LIMIT_SCALED) break;
        for (bigint p2 : primes_1) {
            if (p1 == p2) continue;
            bigint s2 = p2;
            for (int i = 0; i < 6; i++) {
                s2 *= p2;
            }
            bigint p = s1 * s2;
            if (p > LIMIT_SCALED) break;
            generators.push_back((int64) p);
        }
    }

    // get generators for exponents 24, 2
    for (bigint p1 : primes_1) {
        bigint s1 = p1 * p1;
        s1 *= s1;
        s1 = s1 * s1 * s1;
        if (s1 > LIMIT_SCALED) break;
        for (bigint p2 : primes_1) {
            if (p1 == p2) continue;
            bigint p = s1 * p2;
            if (p > LIMIT_SCALED) break;
            generators.push_back((int64) p);
        }
    }

    return generators;
}

std::vector<int64> products_of_primes_2_modulo_3(int64 limit, std::vector<int64> &primes) {
    std::vector<bool> is_product(limit);
    std::vector<int64> primes_01;
    for (int64 p : primes) if (p % 3 != 2) primes_01.push_back(p);

    for (int64 p : primes) {
        for (int64 i = 1; i * p < limit; i++) {
            is_product[i * p] = true;
        }
    }
    for (int64 p : primes_01) {
        for (int64 i = 1; i * p < limit; i++) {
            is_product[i * p] = false;
        }
    }

    std::vector<int64> products {1};
    for (int i = 0; i < limit; i++) {
        if (is_product[i]) products.push_back(i);
    }

    return products;
}

std::string solution() {
    std::vector<int64> primes = generatePrimeList<int64>(100000000);
    std::vector<int64> primes_1;
    for (int64 p : primes) if (p % 3 == 1) primes_1.push_back(p);

    std::vector<int64> generators = calc_generators(primes_1);
    // For a given solution to r^2 = 3^a * g * c
    // where g is a primitive solution, and c is a product of primes == 2 mod 3
    // you can multiply by 3 or any prime == 2 mod 3 and have another solution
    int64 min_gen = *std::min_element(generators.begin(), generators.end());
    int64 primes_2_limit = LIMIT_SCALED / min_gen + 1;
    std::vector<int64> primes_2_products = products_of_primes_2_modulo_3(primes_2_limit, primes);

    // for every product of a generator and a "prime-2" product, we need
    // to check the potential "multiplicity" of the sqrt(3)
    // i.e. how big can a be in (r^2 = 3^a * g * c)
    const double log3 = std::log(3);
    int64 count = 0;
    for (int64 g : generators) {
        for (int64 c : primes_2_products) {
            int64 v = g * c;
            if (v > LIMIT_SCALED) break;
            double x = (double) LIMIT_SCALED / v;
            count += 2 * std::log(x) / log3 + 1;
        }
    }

    return std::to_string(count);
}
