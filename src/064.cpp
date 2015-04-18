#include <iostream>
#include <chrono>
#include <cmath>

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

int main() {
    auto startTime = std::chrono::steady_clock::now();

    int result = 0;
    for ( int n = 2; n <= 10000; n++ ) {
        if ( calculatePeriod( n ) % 2 != 0 ) result++;
    }

    std::cout << "Answer: " << result << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
