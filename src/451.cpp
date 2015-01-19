#include <iostream>
#include <chrono>
#include "pePrimes.h"


/*
 * Sum I(n) for 3 <= n <= 2 * 10 ^7
 * where I(n) = max { m : m < n - 1 and m^2 = 1 mod n }
 *
 * write n = ap for some prime p (a prime factor)
 * suppose we know all m s.t. m^2 = 1 mod p
 *
 * then, to find s s.t. s^2 = 1 mod n, we iterate through
 * t + a*b (for integer b and t+ab < n) and check that that
 * (t + ab)^2 = 1 mod n (or mod a, it doesn't matter)
 *
 * Runtime ~50 minutes (3.4Ghz i5)
 */
std::vector<int> calculateSolutions( int n, PrimeFactorsList& factors, std::vector<std::vector<int>>& solutions ) {
    if ( factors.isPrime(n) ) return {1, n-1};
    std::vector<int> sol;
    int p = factors.getLowestFactor(n);
    int a = n / p;
    std::vector<int>& pastSolutions = solutions[a];
    for ( int i = 0; i < n; i += a ) {
        for ( auto it = pastSolutions.begin(); it != pastSolutions.end(); ++it ) {
            long long int s = i + *it;
            if ( (s * s) % n == 1 ) sol.push_back(s);
        }
    }
    return sol;
}

int main() {
    auto startTime = std::chrono::steady_clock::now();

    int limit = 20000000;
    int solutionLimit = (limit / 2);
    PrimeFactorsList primeFactors( limit + 1 );

    std::vector< std::vector<int> > solutions( solutionLimit + 1 );
    solutions[2].push_back(1);

    long long int sum = 0;
    for ( int i = 3; i <= limit; ++i ) {
        std::vector<int> temp = calculateSolutions( i, primeFactors, solutions );
        if ( i < solutionLimit ) solutions[i] = temp;
        sum += temp[ temp.size() - 2 ];
        if ( i % 100000 == 0 ) std::cout << sum << std::endl;
    }

    std::cout << "Answer: " << sum << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
