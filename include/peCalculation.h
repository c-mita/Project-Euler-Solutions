#ifndef PE_CALCULATION_H
#define PE_CALCULATION_H

#include <stdexcept>
#include <climits>

template<typename T, typename V = int>
T ModularExp( T base, T exp, T mod ) {
    T result = 1;
    base %= mod;
    while ( exp > V(0) ) {
        if ( exp % V(2) == V(1) ) result = ( result * base ) % mod;
        exp >>= V(1);
        base = (base * base) % mod;
    }
    return result;
}

template<typename T>
T ModularInv( T a, T mod ) {
    // returns zero if not invertible
    // based on extended euclidean algorithm
    // see https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm
    T t = 0, next_t = 1;
    T r = mod, next_r = a;
    while ( next_r != 0 ) {
        T q = r / next_r;
        T temp = t - q * next_t;
        t = next_t;
        next_t = temp;

        temp = r - q * next_r;
        r = next_r;
        next_r = temp;
    }
    if ( r > 1 ) return 0;
    if ( t < 0 ) t += mod;
    return t;
}

template<typename T>
void integerDivision( const T& numerator, const T& denominator, T* quotient, T* remainder ) {
    if ( denominator == 0 ) throw std::domain_error("Division by zero");
    T num = numerator;
    T den = denominator;
    T& quot = *quotient;
    T& rem = *remainder;
    quot = 0;
    rem = 0;

    int bits = sizeof( T ) * CHAR_BIT;
    for ( int i = bits - 1; i >= 0; --i ) {
        rem <<= 1;

        //set rem bit 0 to numerator bit i
        T iBit = ( num & ( T(1) << i ) ) >> i;
        rem &= ( ~T(0) << 1 );
        rem |= iBit;

        if ( rem >= den ) {
            rem -= den;
            quot |= T(1) << i;
        }

    }
}

#endif
