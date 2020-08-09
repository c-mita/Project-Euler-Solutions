#include <cmath>
#include "main.h"

// http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Continued_fraction_expansion
int calculatePeriod( int n ) {
    int a0 = sqrt(n);
    if ( a0 * a0 == n ) return 0;
    int m = 0;
    int d = 1;
    int period = 0;
    for ( int a = a0; a != 2 * a0; period++ ) {
        m = d * a - m;
        d = ( n - m * m ) / d;
        a = ( a0 + m ) / d;
    }
    return period;
}

std::string solution() {
    int result = 0;
    for ( int n = 2; n <= 10000; n++ ) {
        if ( calculatePeriod( n ) % 2 != 0 ) result++;
    }

    return std::to_string(result);
}
