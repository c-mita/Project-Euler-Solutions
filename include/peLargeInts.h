#ifndef PE_LARGEINTS_H
#define PE_LARGEINTS_H

#include <climits>
#include <string>
#include <stdexcept>
#include "peCalculation.h"

template <class UInt>
class DoubleUInt {
private:
public:
    UInt low;
    UInt high;

    static const unsigned int bitDepth = sizeof(UInt) * 2 * CHAR_BIT;

    DoubleUInt() : low(0), high(0) {}
    DoubleUInt( UInt l ) : low(l), high(0) {}
    DoubleUInt( UInt h, UInt l ) : low(l), high(h) {}

    std::string toString( unsigned int base = 10 ) const {
        if ( this->high == UInt(0) && this->low == UInt(0) ) return "0";
        if ( base < 2 || base > 16 ) throw std::domain_error( "Invalid base" );
        static char buffer[ bitDepth + 1 ]; //worst case scenario is base 2, which is at most 128 digits + \0
        static char symbols[17] = "0123456789ABCDEF";
        buffer[ bitDepth ] = '\0';
        int i = bitDepth;
        DoubleUInt<UInt> n( *this );
        DoubleUInt<UInt> b( base );
        while ( n != DoubleUInt<UInt>(0) && i ) {
            DoubleUInt<UInt> rem;
            integerDivision( n, b, &n, &rem );
            buffer[--i] = symbols[ rem.low ];
        }
        return std::string( &buffer[i] );
    }

    DoubleUInt<UInt> &operator=( const DoubleUInt<UInt> &rhs ) {
        if ( &rhs != this ) {
            low = rhs.low;
            high = rhs.high;
        }
        return *this;
    }

    //comparisons

    friend bool operator==( const DoubleUInt<UInt>& lhs, const DoubleUInt<UInt>& rhs ) {
        return ( lhs.high == rhs.high ) && ( lhs.low == rhs.low );
    }
    friend bool operator==( const DoubleUInt<UInt>& lhs, const UInt& rhs ) {
        return ( lhs.high == 0 ) && ( lhs.low == rhs );
    }
    friend bool operator==( const UInt& lhs, const DoubleUInt<UInt>& rhs ) {
        return rhs == lhs;
    }
    friend bool operator<( const DoubleUInt<UInt>& lhs, const DoubleUInt<UInt>& rhs ) {
        return ( lhs.high == rhs.high ) ? lhs.low < rhs.low : lhs.high < rhs.high;
    }
    friend bool operator<( const DoubleUInt<UInt>& lhs, const UInt& rhs ) {
        return ( lhs.high > 0 ) ? false : lhs.low < rhs;
    }
    friend bool operator<( const UInt& lhs, const DoubleUInt<UInt>& rhs ) {
        return ( rhs.high > 0 ) ? true : lhs < rhs.low;
    }

    //Unary

    DoubleUInt<UInt>& operator++() {
        if ( ++low == 0 ) ++high;
        return *this;
    }

    DoubleUInt<UInt> operator++(int) {
        DoubleUInt<UInt> tmp(*this);
        operator++();
        return tmp;
    }

    DoubleUInt<UInt>& operator--() {
        if ( low-- == 0 ) --high;
        return *this;
    }

    DoubleUInt<UInt> operator--(int) {
        DoubleUInt<UInt> tmp(*this);
        operator--();
        return tmp;
    }

    DoubleUInt<UInt> operator~() const {
        return DoubleUInt( ~(this->high), ~(this->low) );
    }

    DoubleUInt<UInt> operator-() const {
        //2's complement
        return ~DoubleUInt(*this) + 1;
    }

    //compound assignment operators

    DoubleUInt<UInt>& operator+=( const DoubleUInt<UInt>& rhs ) {
        const UInt lowOld = low;
        low += rhs.low;
        high += rhs.high;
        if ( low < lowOld ) ++high;
        return *this;
    }

    DoubleUInt<UInt>& operator+=( const int rhs ) {
        DoubleUInt<UInt> tmp( 0, rhs );
        (*this) += tmp;
        return *this;
    }

    DoubleUInt<UInt>& operator-=( const DoubleUInt<UInt>& rhs ) {
        return *this += -rhs;
    }

    DoubleUInt<UInt>& operator-=( const int rhs ) {
        DoubleUInt<UInt> tmp( 0, rhs );
        (*this) -= tmp;
        return *this;
    }

    DoubleUInt<UInt>& operator|=( const DoubleUInt<UInt>& rhs ) {
        high |= rhs.high;
        low |= rhs.low;
        return *this;
    }

    DoubleUInt<UInt>& operator|=( const UInt& rhs ) {
        low |= rhs;
        return *this;
    }

    DoubleUInt<UInt>& operator&=( const DoubleUInt<UInt>& rhs ) {
        high &= rhs.high;
        low &= rhs.low;
        return *this;
    }

    DoubleUInt<UInt>& operator&=( const UInt& rhs ) {
        high = 0;
        low &= rhs;
        return *this;
    }

    DoubleUInt<UInt>& operator^=( const DoubleUInt<UInt>& rhs ) {
        high ^= rhs.high;
        low ^= rhs.low;
        return *this;
    }

    DoubleUInt<UInt>& operator<<=( unsigned int rhs ) {
        if ( rhs >= bitDepth ) {
            high = 0;
            low = 0;
        } else {
            unsigned int halfDepth = bitDepth / 2;
            if ( rhs >= halfDepth ) {
                rhs -= halfDepth;
                high = low;
                low = 0;
            }

            if ( rhs != 0 ) {
                high <<= rhs;
                UInt mask( ~( UInt(-1) >> rhs ) );
                high |= ( low & mask ) >> ( halfDepth - rhs );
                low <<= rhs;
            }
        }
        return *this;
    }

    DoubleUInt<UInt>& operator>>=( unsigned int rhs ) {
        if ( rhs >= bitDepth ) {
            high = 0;
            low = 0;
        } else {
            unsigned int halfDepth = bitDepth / 2;
            if ( rhs >= halfDepth ) {
                rhs -= halfDepth;
                low = high;
                high = 0;
            }
            if ( rhs != 0 ) {
                low >>= rhs;
                UInt mask( ~( UInt(-1) << rhs ) );
                low |= ( high & mask ) << ( halfDepth - rhs );
                high >>= rhs;
            }
        }
        return *this;
    }

    DoubleUInt<UInt>& operator*=( const DoubleUInt<UInt>& rhs ) {
        if ( rhs == 1 ) return *this;
        if ( rhs == 0 ) {
            high = 0;
            low = 0;
        } else {
            DoubleUInt<UInt> a( *this );
            DoubleUInt<UInt> b = rhs;
            high = 0;
            low = 0;

            for ( unsigned int i = 0; i < bitDepth; i++ ) {
                if ( ( b & 1 ) != (UInt)0 ) *this += ( a << i );
                b >>= 1;
            }
        }
        return *this;
    }

    DoubleUInt<UInt>& operator*=( const int rhs ) {
        DoubleUInt<UInt> tmp( 0, rhs );
        (*this) *= tmp;
        return *this;
    }

    DoubleUInt<UInt>& operator/=( const DoubleUInt<UInt>& rhs ) {
        DoubleUInt<UInt> remainder;
        integerDivision( *this, rhs, this, &remainder );
        return *this;
    }

    DoubleUInt<UInt>& operator/=( const int& rhs ) {
        DoubleUInt<UInt> remainder;
        DoubleUInt<UInt> denominator(rhs);
        integerDivision( *this, denominator, this, &remainder );
        return *this;
    }

    DoubleUInt<UInt>& operator%=( const DoubleUInt<UInt>& rhs ) {
        DoubleUInt<UInt> quotient;
        integerDivision( *this, rhs, &quotient, this );
        return *this;
    }

    DoubleUInt<UInt>& operator%=( const int& rhs ) {
        DoubleUInt<UInt> quotient;
        DoubleUInt<UInt> denominator(rhs);
        integerDivision( *this, denominator, &quotient, this );
        return *this;
    }
};

//Comparisons

template<typename T>
bool operator!=( const DoubleUInt<T>& lhs, const DoubleUInt<T>& rhs ) {
    return !( lhs == rhs );
}

template<typename T>
bool operator!=( const DoubleUInt<T>& lhs, const T& rhs ) {
    return !(lhs == rhs);
}

template<typename T>
bool operator!=( const T& rhs, const DoubleUInt<T>& lhs ) {
    return !(lhs == rhs);
}

template<typename T>
bool operator>( const DoubleUInt<T>& lhs, const DoubleUInt<T>& rhs ) {
    return rhs < lhs;
}

template<typename T>
bool operator>( const DoubleUInt<T>& lhs, const T& rhs ) {
    return rhs < lhs;

}

template<typename T>
bool operator>( const T& lhs, const DoubleUInt<T>& rhs ) {
    return rhs < lhs;
}

template<typename T>
bool operator<=( const DoubleUInt<T>& lhs, const DoubleUInt<T>& rhs ) {
    return !( lhs > rhs );
}

template<typename T>
bool operator<=( const DoubleUInt<T>& lhs, const T& rhs ) {
    return !( lhs > rhs );

}

template<typename T>
bool operator<=( const T& lhs, const DoubleUInt<T>& rhs ) {
    return !( lhs > rhs );
}

template<typename T>
bool operator>=( const DoubleUInt<T>& lhs, const DoubleUInt<T>& rhs ) {
    return !( lhs < rhs );
}

template<typename T>
bool operator>=( const DoubleUInt<T>& lhs, const T& rhs ) {
    return !( lhs < rhs );

}

template<typename T>
bool operator>=( const T& lhs, const DoubleUInt<T>& rhs ) {
    return !( lhs < rhs );
}


//Binary operators

template<typename T>
DoubleUInt<T> operator+( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs += rhs;
    return lhs;
}
template<typename T>
DoubleUInt<T> operator+( int lhs, const DoubleUInt<T>& rhs ) {
    return DoubleUInt<T>( lhs ) + rhs;
}

template<typename T>
DoubleUInt<T> operator+( DoubleUInt<T> lhs, const int& rhs ) {
    lhs += rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator-( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs -= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator-( DoubleUInt<T> lhs, const int& rhs ) {
    lhs -= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator*( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs *= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator*( DoubleUInt<T> lhs, const int& rhs ) {
    lhs *= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator*( int lhs, const DoubleUInt<T>& rhs ) {
    return DoubleUInt<T>( lhs ) + rhs;
}

template<typename T>
DoubleUInt<T> operator/( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs /= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator/( DoubleUInt<T> lhs, const int& rhs ) {
    lhs /= rhs;
    return lhs;
}
template<typename T>
DoubleUInt<T> operator%( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs %= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator%( DoubleUInt<T> lhs, const int& rhs ) {
    lhs %= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator&( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs &= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator&( DoubleUInt<T> lhs, const int& rhs ) {
    lhs &= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator|( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs |= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator|( DoubleUInt<T> lhs, const int& rhs ) {
    lhs |= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator^( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs ^= rhs;
    return lhs;
}
template<typename T>
DoubleUInt<T> operator^( DoubleUInt<T> lhs, const int* rhs ) {
    lhs ^= rhs;
    return lhs;
}
template<typename T>
DoubleUInt<T> operator<<( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs <<= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator<<( DoubleUInt<T> lhs, const int& rhs ) {
    lhs <<= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator>>( DoubleUInt<T> lhs, const DoubleUInt<T>& rhs ) {
    lhs >>= rhs;
    return lhs;
}

template<typename T>
DoubleUInt<T> operator>>( DoubleUInt<T> lhs, const int& rhs ) {
    lhs >>= rhs;
    return lhs;
}
#endif
