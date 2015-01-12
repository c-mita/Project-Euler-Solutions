#include <iostream>
#include <chrono>
#include <vector>
#include "pePrimes.h"
#include "peLargeInts.h"

typedef unsigned long long int longint;
typedef DoubleUInt< longint > veryLongInt;

/*
 * Sum of n for n < 150 million and
 * n^2 + d are consecutive primes for
 * d in {1, 3, 7, 9, 13, 27}
 *
 * Can see n must be even for n^2 + d
 * to be prime.
 * Considering modulo 5, we see n = 0 mod 5
 * Other values fail.
 * Similarly n != 0 mod 3 and n != 0 mod 7
 * (other values fail for 7, but we skip them)
 *
 * We must also check for prime "gaps" since they
 * must be consecutive.
 * d in {5, 15, 25} obviously fails.
 * Since n^2 = 1 mod 3 (n = 1 or 2 mod 3) and
 * 11, 17, 23 = 2 mod 3, we can rule them out.
 * So only {19, 21} need to be checked that they
 * don't produce primes.
 *
 * Runtime ~35-40 minutes 3.4GHz i5
 */
int main() {
    auto startTime = std::chrono::steady_clock::now();

    int limit = 150000000;
    std::vector<int> addToPrime { 1, 3, 7, 9, 13, 27 };
    std::vector<int> addToNonPrime { 19, 21 };
    int skip3 = 0, skip7 = 0;
    int sum = 0;
    for ( longint n = 0; n < limit; n += 10 ) {
        //note bitwise or - both sides must evaluate
        if ( (skip3++ % 3 == 0) | (skip7++ % 7 == 0) ) continue;
        longint k = n * n;
        for ( auto it = addToPrime.begin(); it != addToPrime.end(); ++it ) {
           if ( !isMrPrime<veryLongInt, longint>( veryLongInt(k + (longint)*it) ) ) goto FAILURE;
        }
        for ( auto it = addToNonPrime.begin(); it != addToNonPrime.end(); ++it ) {
            if ( isMrPrime<veryLongInt, longint>( veryLongInt(k + (longint)*it) ) ) goto FAILURE;
        }
        std::cout << n << std::endl;
        sum += n;
FAILURE:;
    }

    std::cout << "Answer: " << sum << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
