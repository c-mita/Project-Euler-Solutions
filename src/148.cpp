#include "main.h"

/*
 * Problem 148
 *
 * Lucas' Theorem states that:
 * n C k = Product( ni C ki ) mod p where
 * p prime
 * n = sum( ni * p^i )
 * k = sum( ki * p^i )
 * (i.e. written as numbers in base p)
 *
 * Note that p does not divide n C k iff
 * p does not divide ni C ki for all i
 *
 * Since ni < p, p does not divide ni C ki iff
 * 0 <= ki <= ni
 * (Since all elements in the first p rows of Pascal's triangle
 * are not divisible by p - expand the terms to see)
 *
 * So for each ni, there are ni + 1 values for ki that satisfy
 * the problem statement (that are not divisible by p)
 *
 * So, for each row n, we can write n as a base p number
 * and take the product of each (digit + 1).
 *
 * Runtime ~30 seconds (Visual Studio 2013)
 *
 * Could improve by exploiting recursive nature of triangle
 * (Pascal's Triangle mod p is very much like Sierpinksi's triangle)
 */

int* getRepresentation( int n, int base, int* buffer ) {
    do {
        *(buffer++) = n % base;
    } while ( n /= base );
    return buffer;
}

std::string solution() {
    int prime = 7;
    int limit = 1000000000;
    int buffer[11];
    long long int sum = 0;

    for ( int n = 0; n < limit; n++ ) {
        long long int rowSum = 1;
        int* end = getRepresentation( n, prime, buffer );
        for ( int* ptr = &buffer[0]; ptr != end; ++ptr ) {
            rowSum *= (long long int)( *ptr + 1 );
        }

        sum += rowSum;
    }

    return std::to_string(sum);
}
