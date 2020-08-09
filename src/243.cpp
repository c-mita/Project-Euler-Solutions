#include <algorithm>
#include <vector>
#include "main.h"
#include "pePrimes.h"

const double RESILIENCE_VALUE = 15499.0 / 94744.0;

template<typename T>
std::vector<T> merge(std::vector<T> v1, std::vector<T> v2) {
    std::vector<T> out;
    std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(out));
    return out;
}

/*
 * Euler's totient function phi(n) = number of m < n s.t. m and n are coprime
 * phi(n) = n * prod{1 - 1 / p : p prime and divides n}
 * (note distinct primes!)
 *
 * So phi(n) / n-1 is "a bit like":
 * prod{1 - 1/p : p prime dividing n}
 * So if we want this to be small, we want lots of small distinct prime factors.
 *
 * Multiply sequential primes until we reach our target condition.
 * Then, to check we really have the smallest value, take out the largest prime
 * and replace it with candidate values, checking the condition at each turn.
 *
 * < 0.05ms on Ryzen
 */
std::string solution() {
    std::vector<int> primes = generatePrimeList<int>(100);
    long long int v = 1;
    int totient = 1;
    double resiliance = 1;
    std::vector<int> used_primes;
    for (int prime : primes) {
        used_primes.push_back(prime);
        v *= prime;
        totient *= prime - 1;
        resiliance = totient / (double) (v - 1);
        if (resiliance < RESILIENCE_VALUE) break;
    }

    /*
     * We need to calculate the totient function for every candidate we check.
     * We can't just multiply because we're not going to be coprime with previous
     * iterations anymore. But we know the prime factors for "v" and the number of prime
     * factors for our new non-prime will be small, so can be calculated cheaply.
     */
    const int last_prime = used_primes.back();
    used_primes.pop_back();
    v /= last_prime;
    PrimeFactorsList<int> factors(last_prime);

    int candidate = 1;
    for (int k = 2; k <= last_prime; k++) {
        candidate = v * k;
        std::vector<int> prime_factors = merge<int>(used_primes, factors[k]);
        int n_coprime = EulerTotient(prime_factors);
        resiliance = n_coprime / ((double) candidate - 1);
        if (resiliance < RESILIENCE_VALUE) break;
    }

    return std::to_string(candidate);
}

/**
 * 60-70 seconds on a Ryzen 3900X.
 * The brute force solution...
 */
std::string solution_slow() {
    PrimeFactorsList<int> factors(1000000000);
    std::cout << "HERE" << std::endl;
    double resiliance = 1;
    int d = 1;
    while (resiliance >= RESILIENCE_VALUE) {
        d++;
        int n_coprime = EulerTotient(factors[d]);
        resiliance = n_coprime / ((double) d - 1);
        if (d % 10000000 == 0) std::cout << d << ", " << resiliance << std::endl;
    }
    return std::to_string(d);
}
