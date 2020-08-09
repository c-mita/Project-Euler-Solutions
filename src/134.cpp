#include <stdexcept>
#include "main.h"
#include "peCalculation.h"
#include "pePrimes.h"

/*
 * for all consecutive primes p1, p2 with p1 < p2
 * require sum of smallest n s.t. p2 divides n and
 * n ends with the digits of p1
 *
 * i.e.
 * n = k * 10^m + p1 for some k (where m is digit count of p1)
 *
 * Then k * 10^m + p1 == 0 mod p2
 * k * 10^m == p2 - p1 mod p2
 * k == (p2 - p1) / (10^m) mod p2
 *
 * Calculate the modular inverse of 10^m (call it t_inv)
 * k == t_inv * (p2 - p1) mod p2
 */

static const int LIMIT = 1000000;

int power_of_ten(int target) {
    int t = 10;
    while (t < target) t *= 10;
    return t;
}

std::string solution() {
    long long int sum = 0;
    auto primes = generatePrimeList<int>(LIMIT + 100);
    for (auto p1_it = primes.begin() + 2, p2_it = primes.begin() + 3;
            p2_it != primes.end() && *p1_it <= LIMIT;
            ++p1_it, ++p2_it) {
        auto p1 = *p1_it, p2 = *p2_it;
        int ten_d = power_of_ten(p1);
        auto inv = ModularInv(ten_d, p2);
        if (inv == 0) throw std::domain_error("No inverse somehow!");
        long long int k = ((long long int) inv * (long long int) (p2 - p1) % p2);
        sum += k * ten_d + p1;
    }
    return std::to_string(sum);
}
