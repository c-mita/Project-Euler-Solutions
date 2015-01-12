#include <iostream>
#include <chrono>
#include "pePrimes.h"

int nextCycle( int n, int length ) {
    int digit = n % 10;
    n /= 10;
    while ( --length > 0 ) digit *= 10;
    n += digit;
    return n;
}

int main() {
    auto startTime = std::chrono::steady_clock::now();
    int limit = 1000000;

    int cycleCount = 0;
    std::vector<bool> primes = generatePrimalityTable( limit );
    std::vector<bool> checked( limit, false );
    std::cout << primes[119] << std::endl;
    for ( int p = 2; p < limit; p++ ) {
        if ( !primes[p] || checked[p] ) continue;
        int prime = p;
        //perform basic sanity check and find its length
        bool sane = true;
        int length = 0;
        if ( prime < 10 ) {
            length = 1;
        } else {
            int n = prime;
            while ( n > 0 ) {
                int digit = n % 10;
                if ( (digit & 1) == 0 || digit == 5 ) {
                    sane = false;
                    break;
                }
                n /= 10;
                length++;
            }
        }
        if ( !sane ) continue;
        bool isCycle = true;
        int cycleLength = 0;
        for ( int i = 0; i < length; i++ ) {
            checked[prime] = true;
            cycleLength++;
            prime = nextCycle( prime, length );
            isCycle &= primes[prime];
            if ( !isCycle );
            if ( checked[prime] ) break;
        }
        cycleCount += isCycle ? cycleLength : 0;
        if (isCycle) std::cout << prime << std::endl;
    }

    std::cout << "Answer: " << cycleCount << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
