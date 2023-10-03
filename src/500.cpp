#include <algorithm>
#include <vector>

#include "main.h"
#include "pePrimes.h"

/**
 * Find the smallest number with 2^500500 divisors.
 * Give the answer modulo 500500507
*/

/**
 * A Fermi-Dirac prime is a prime power where the exponent is a power of two.
 * i.e. q = p^(2^k) for some k.
 *
 * It turns out that the smallest number with 2^n divisors is in fact the
 * product of the first "n" Fermi-Dirac primes.
 *
 * Since the sequence of FD primes is bounded above by the sequence of primes,
 * the 500500th FD prime is less than the 500500th regular prime.
 *
 * Fermi-Dirac primes can be generated from primes by repeatedly squaring them.
 * So the the FD primes we care about can be generated by taking each of the
 * first 500500 primes and repeatedly squaring them until they exceed 500500.
 * Then we just sort the set and multiply the first 500500.
 *
 * Runs in 100 ms on a laptop.
 */

std::vector<int> generate_fd_primes(int base, int limit) {
    std::vector<int> fd_primes;
    long long int v = base;
    while (v < limit) {
        fd_primes.push_back((int) v);
        v *= v;
    }
    return fd_primes;
}

std::string solution() {
    // the 500500th prime is under 8,000,000 so sieve up to there
    std::vector<int> primes = generatePrimeList<int>(8000000);
    primes = std::vector<int>(primes.begin(), primes.begin() + 500500);

    std::vector<int> fd_primes;
    for (int p : primes) {
        std::vector<int> new_primes = generate_fd_primes(p, primes[500499]);
        fd_primes.reserve(new_primes.size());
        fd_primes.insert(fd_primes.end(), new_primes.begin(), new_primes.end());
    }
    std::sort(fd_primes.begin(), fd_primes.end());
    // this "slice" is technically a copy, but I don't care and maybe the
    // compiler saves us.
    fd_primes = std::vector<int>(fd_primes.begin(), fd_primes.begin() + 500500);

    int mod = 500500507;
    long long result = 1;
    for (int p : fd_primes) {
        result *= p;
        result %= mod;
    }

    return std::to_string(result);
}