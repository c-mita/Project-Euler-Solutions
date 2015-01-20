#include <iostream>
#include <chrono>
#include <algorithm>
#include "pePrimes.h"


/*
 * m^2 = 1 mod n  =>  m^2 - 1 = 0 mod n
 * hence (m-1)(m+1) = 0 mod n
 *
 * So for all m between 2 and 20000000, get the prime factors
 * of m-1 and m+1, and use them to generate all divisors of  m^2 - 1.
 *
 * These divisors are now the test n's
 * For each one, test if it is greater than m + 1 and, if m is greater
 * than the previously calculated value for that m, store the new value for m
 * in the lookup table (looking up on "n" - the test divisor)
 *
 * Runtime ~ 110 seconds
 *
 * Still a way off that < 60 second mark
 */
int main() {
    auto startTime = std::chrono::steady_clock::now();

    int limit = 20000000;
    std::vector<long long int> records( limit + 1, 1 );
    PrimeFactorsList<long long int> factorList( limit + 1 );

    for ( int m = 2; m < limit - 1; m++ ) {
        std::vector<long long int> factorsL = factorList[m - 1];
        std::vector<long long int> factorsH = factorList[m + 1];
        std::vector<long long int> factorsC( factorsL );
        factorsC.insert( factorsC.end(), factorsH.begin(), factorsH.end() );
        std::sort( factorsC.begin(), factorsC.end() );

        std::vector<long long int> divisors = calculateAllDivisors( factorsC );
        for ( auto it = divisors.rbegin(); it != divisors.rend(); ++it ) {
            if ( m < (*it - 1) && *it <= limit && records[*it] < m ) records[*it] = m;
        }
    }

    long long int sum = 0;
    for ( auto it = records.begin() + 3; it != records.end(); ++it ) {
        sum += *it;
    }

    std::cout << "Answer: " << sum << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
