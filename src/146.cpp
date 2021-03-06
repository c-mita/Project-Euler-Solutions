#include <boost/multiprecision/cpp_int.hpp>
#include "main.h"
#include "pePrimes.h"

typedef unsigned long long int uLongInt;
typedef boost::multiprecision::cpp_int bigint;

/*
 * Sum of n for n < 150 million and
 * n^2 + d are consecutive primes for
 * d in {1, 3, 7, 9, 13, 27}

 * Write n as n = p * x + r for prime p
 * Then n^2 + d = p^2 * x^2 + 2pxr + r^2 + d
 * q | (r^2 + d) iff q | (n^2 + d)
 * q | (n^2 +d) only if q | ((n%q)^2 + d)
 *
 * Since we don't want q to divide r^2 + d
 * (we want (n^2 + d) to be prime)
 * we can filter out candidates for n by
 * checking for this relationship against
 * various primes (say p < 1 million)
 *
 * Runtime ~3sec 3.4GHz i5
 */

std::string solution() {
    int limit = 150000000;
    int initialPrimeCount = 1000000;
    std::vector<int> additions { 1, 3, 7, 9, 13, 27 };
    std::vector<int> nonAdditions { 19, 21 };
    std::vector<int> primes = generatePrimeList<int>( initialPrimeCount );
    std::vector<int> candidatePrimes;
    int sum = 0;

    for ( int n = 0; n < limit; n += 10 ) {
        if ( n % 3 == 0 || n % 7 == 0 ) continue;
        for ( auto it = primes.begin(); it != primes.end(); ++it ) {
            if ( *it > n ) break;
            int p = *it;
            long long int r = n % p; //multiplication may overflow int
            for ( auto jt = additions.begin(); jt != additions.end(); ++jt ) {
                if ( ( r * r + *jt ) % p == 0 ) goto CANDIDATE_FAILURE;
            }
        }
        candidatePrimes.push_back( n );
    CANDIDATE_FAILURE:;
    }

    for ( auto it = candidatePrimes.begin(); it != candidatePrimes.end(); ++it ) {
        uLongInt n = *it;
        for ( auto jt = additions.begin(); jt != additions.end(); ++jt ) {
            uLongInt r = n * n + *jt;
            //if ( !isPrime( r ) ) goto CHECK_FAILURE;
            bool isPrime = isMrPrime<bigint, uLongInt>( bigint(r) );
            if ( !isPrime ) goto CHECK_FAILURE;
        }
        for ( auto jt = nonAdditions.begin(); jt != nonAdditions.end(); ++jt ) {
            uLongInt r = n * n + *jt;
            //if ( isPrime( r ) ) goto CHECK_FAILURE;
            bool isPrime = isMrPrime<bigint, uLongInt>( bigint(r) );
            if ( isPrime ) goto CHECK_FAILURE;
        }
        sum += n;
    CHECK_FAILURE:;
    }

    return std::to_string(sum);
}
