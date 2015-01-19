#include "pePrimes.h"

PrimeFactorsList::PrimeFactorsList( int length ) {
    size = length;
    factors = std::vector<int>( size, 0 );
    factors[1] = 1;
    int n = 2;
    do {
        if ( factors[n] == 0 ) {
            for ( int i = 1; i <= ( (size - 1) / n ); i++ ) {
                factors[i*n] = i;
            }
        }
    } while ( n++ < size - 1 );
}

PrimeFactorsList::~PrimeFactorsList() {}

bool PrimeFactorsList::isPrime( int i ) const {
    return factors[i] == 1;
}

const std::vector<int> PrimeFactorsList::operator[]( std::size_t index ) const {
    if ( index >= static_cast<unsigned int>(size) ) throw;
    std::vector<int> primeFactors;
    int factor = index;
    do {
        int nextFactor = factors[factor];
        primeFactors.push_back( factor / nextFactor );
        factor = nextFactor;
    } while ( factor != 1 );
    return primeFactors;
}

int PrimeFactorsList::getLowestFactor( std::size_t index ) const {
    if ( index >= static_cast<unsigned int>(size) ) throw;
    return factors[index];
}
