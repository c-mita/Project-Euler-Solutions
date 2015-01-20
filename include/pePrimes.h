#ifndef PE_PRIMES_H
#define PE_PRIMES_H

#include <vector>
#include "peCalculation.h"

/*
 * Calculates EulerTotient function using list
 * of prime factors (including multiplicity)
 * Assumes factors are in order!
 * e.g. 2,2,3,7
 * NOT 2,7,2,3
 */
inline int EulerTotient( const std::vector<int>& primeFactors ) {
    int answer = 1;
    int prev = 0;
    for ( auto it = primeFactors.begin(); it != primeFactors.end(); ++it ) {
        answer *= prev == *it ? *it : *it - 1;
        prev = *it;
    }
    return answer;
}

template<typename T>
bool isPrime( T n ) {
    if ( n == 1 ) return false;
    if ( n == 2 || n == 3 ) return true;
    int i = 2;
    while ( (T)i * (T)i <= n ) {
        if ( n % (T)i++ == 0 ) return false;
    }
    return true;
}

template<typename T>
bool isPrime( T n, const std::vector<bool>& table ) {
    if ( n < 2 ) return false;
    if ( n < table.size() ) return table[n];
    if ( n == 2 || n == 3 ) return true;
    if ( table.size() < 5 ) return isPrime<T>( n );
    T i;
    for ( i = 2; i < table.size(); i++ ) {
        if ( i * i > n ) return true;
        if ( table[i] && ( n % i == 0 ) ) return false;
    }
    while ( i * i <= n ) {
        if ( n % i++ == 0 ) return false;
    }
    return true;
}

/*
 * Miller-Rabin primality test
 */
template<typename T, typename V = int>
bool isMrPrime( T n, const std::vector<int>& testCases ) {
    if ( n <= (V)1 ) return false;
    if ( n == (V)2 ) return true;
    if ( n % (V)2 == 0 ) return false;
    if ( n < (V)9 ) return true;
    if ( n % (V)3 == 0 || n % (V)5 == 0 || n % V(7) == 0 ) return false;

    //find s s.t. n - 1 == 2^s * d for odd d;
    T k = n - 1;
    T s = 0;
    while ( (k & 1) == 0 ) {
        k >>= 1;
        s++;
    }

    for ( auto it = testCases.begin(); it != testCases.end(); ++it ) {
        T a(*it);
        T x = ModularExp<T, V>( a, k, n );
        T y;
        for ( T i = 0; i < s; i++ ) {
            y = ( x * x ) % n;
            if ( y == V(1) && x != V(1) && x != n - V(1) ) return false;
            x = y;
        }
        if ( y != V(1) ) return false;
    }
    return true;
}

/*
 * Miller-Rabin primality test.
 * Accurate for n < 3,825,123,056,546,413,051
 */
template<typename T, typename V = int>
bool isMrPrime( T n ) {
    std::vector<int> testCases;
    if ( n < 1373653 ) {
        testCases = {2, 3};
    } else if ( n < 9080191 ) {
        testCases = {31, 73};
    } else if ( n < 4759123141ULL ) {
        testCases = {2, 7, 61};
    } else if ( n < 2152302398787ULL ) {
        testCases = {2, 3, 5, 7, 11};
    } else if ( n < 3474749660383ULL ) {
        testCases = {2, 3, 5, 7, 11, 13};
    } else {
        testCases = {2, 3, 5, 7, 11, 13, 17};
    }
    return isMrPrime<T, V>( n, testCases );
}

/*
 * Generate lookup table that returns if a number is prime
 * Sieve of Eratosthenes
 */
template<typename T>
std::vector<bool> generatePrimalityTable( T upper ) {
    std::vector<bool> primes(upper, true);
    primes[0] = false;
    primes[1] = false;
    for ( T i = 2; i * i < upper; i++ ) {
        if ( primes[i] ) {
            for ( T j = i*i; j < upper; j += i ) {
                primes[j] = false;
            }
        }
    }
    return primes;
}

template<typename T>
std::vector<T> generatePrimeList( T upper ) {
    std::vector<bool> primeTable = generatePrimalityTable<T>( upper );
    std::vector<T> primes;
    T i = 0;
    for ( auto it = primeTable.begin(); it != primeTable.end(); ++it ) {
        if ( *it ) primes.push_back( i );
        i++;
    }
    return primes;
}

/*
 * Gives prime factors for all numbers up to its length.
 * Calculated in sieve like fashion.
 */
template<typename T>
class PrimeFactorsList {
public:
    PrimeFactorsList( std::size_t length ) {
        size = length;
        factors = std::vector<T>( size, 0 );
        factors[1] = 1;
        std::size_t n = 2;
        do {
            if ( factors[n] == 0 ) {
                for ( std::size_t i = 1; i <= ( (size - 1) / n ); i++ ) {
                    factors[i*n] = i;
                }
            }
        } while ( n++ < size - 1 );
    }

    ~PrimeFactorsList() {}

    const std::vector<T> operator[]( std::size_t index ) const {
        if ( index >= size ) throw;
        std::vector<T> primeFactors;
        int factor = index;
        do {
            int nextFactor = factors[factor];
            primeFactors.push_back( factor / nextFactor );
            factor = nextFactor;
        } while ( factor != 1 );
        return primeFactors;
    }

    bool isPrime( T i ) const {
        return factors[i] == 1;
    }

    int getLowestFactor( std::size_t index ) const {
        if ( index >= size ) throw;
        return factors[index];
    }
private:
    std::vector<T> factors;
    std::size_t size;
};

#endif
