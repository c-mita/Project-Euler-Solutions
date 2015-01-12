#include <iostream>
#include <chrono>
#include "pePrimes.h"
#include "pePermutations.h"

int checkPartitions( int start, int prev, int* perms ) {
    int count = 0;
    for ( int i = start; i < 9; i++ ) {
        int n = 0;
        for ( int j = start; j <= i; j++ ) {
            n *= 10;
            n += perms[j];
        }
        if ( n < prev ) continue;
        if ( !isPrime( n ) ) continue;
        if ( i == 8 ) return count + 1;
        count += checkPartitions( i + 1, n, perms );
    }
    return count;
}

int main() {
    auto startTime = std::chrono::steady_clock::now();

    int count = 0;
    int perms[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    do {
        count += checkPartitions( 0, 0, perms );
    } while ( nextLexicalPermutation( perms, 9 ) );

    std::cout << count << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
