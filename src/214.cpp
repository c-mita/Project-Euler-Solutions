#include <vector>
#include "main.h"
#include "pePrimes.h"

/*
 * Generate prime factors
 * Use them to iteratively calculate Euler Totients
 * Cache already calculated chains.
 *
 * Runtime: ~3.4 seconds
 */
std::string solution() {
    int limit = 40000000;
    int exhaustiveLimit = 3000000; //don't calculate totient for all non-prime i, only below this limit
    std::vector<int> chainLengths( limit, 0 );
    long long int sum = 0;
    PrimeFactorsList<int> primeFactors( limit );
    for ( int i = 5; i < limit; i++ ) {
        if ( i > exhaustiveLimit && !primeFactors.isPrime(i) ) continue;
        int chainLength = 0;
        int x = i;
        while ( ++chainLength <= 25 && x > 1 ) {
            int nextChainLength = chainLengths[x];
            if ( nextChainLength > 0 ) {
                chainLength += nextChainLength - 1; //-1 since we've already incremented chainLength
                x = 1;
                goto CACHE_HIT;
            }
            x = EulerTotient( primeFactors[x] );
        }
CACHE_HIT:
        chainLengths[i] = chainLength;
        if ( chainLength == 25 && x == 1 && primeFactors.isPrime(i) ) sum += i;
    }
    return std::to_string(sum);
}
