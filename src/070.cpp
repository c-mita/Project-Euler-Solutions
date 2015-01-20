#include <vector>
#include <utility>
#include <iostream>
#include <chrono>
#include "pePrimes.h"

bool Permutations( int x, int y ) {
    if ( (x % 3 == 0) != (y % 3 == 0) ) return false;
    int digits[10] = {0};
    while ( x > 0 ) {
        digits[x % 10]++;
        x /= 10;
    }
    while ( y > 0 ) {
        digits[y % 10]--;
        y /= 10;
    }
    for ( int i = 0; i < 10; i++ ) if (digits[i]) return false;
    return true;
}

int main() {
    auto startTime = std::chrono::steady_clock::now();

    int limit = 10000000;
    std::vector< std::pair<int, int> > results;
    PrimeFactorsList<int> primeFactors( limit );
    for ( int i = 1; i < limit; i++ ) {
        int totient = EulerTotient( primeFactors[i] );
        if ( Permutations( totient, i ) ) {
            results.push_back( std::pair<int, int>(i, totient) );
        }
    }

    double minQuotient = limit;
    int minN = 0;

    for ( auto it = results.begin(); it != results.end(); ++it ) {
        double quotient = it->first / (double) it->second;
        if ( minQuotient > quotient ) {
            minN = it->first;
            minQuotient = quotient;
        }
    }

    std::cout << "Answer: " << minN << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
