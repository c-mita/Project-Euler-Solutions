#include <iostream>
#include "peLargeInts.h"

typedef unsigned long long int uLongInt;
typedef DoubleUInt<uLongInt> uVeryLongInt;

void addition() {
    std::cout << "Addition:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL ); // = 1834 << 64 + 1293875340032
    uVeryLongInt b( 232, 2134592384234ULL );
    uVeryLongInt result = a + b;
    uVeryLongInt expected( 2066, 3428467724266 );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void subtraction() {
    std::cout << "Subtraction:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL ); // = 1834 << 64 + 1293875340032
    uVeryLongInt b( 232, 2134592384234ULL );
    uVeryLongInt result = a - b;
    uVeryLongInt expected( 1601, 18446743232992507414ULL );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void multiplication() {
    std::cout << "Multiplication:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL );
    uVeryLongInt b( 0, 44675 );
    uVeryLongInt result = a * b;
    uVeryLongInt expected = uVeryLongInt( 81933950UL, 57803880815929600ULL );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void division() {
    std::cout << "Division:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL );
    uVeryLongInt b( 0, 44675 );
    uVeryLongInt result = a / b;
    uVeryLongInt expected = uVeryLongInt( 0, 757276522271453676ULL );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void bitwiseAnd() {
    std::cout << "Bitwise And:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL ); // = 1834 << 64 + 1293875340032
    uVeryLongInt b( 232, 2134592384234ULL );
    uVeryLongInt result = a & b;
    uVeryLongInt expected( 40, 1238034472960 );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void bitwiseOr() {
    std::cout << "Bitwise Or:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL ); // = 1834 << 64 + 1293875340032
    uVeryLongInt b( 232, 2134592384234ULL );
    uVeryLongInt result = a | b;
    uVeryLongInt expected( 2026, 2190433251306ULL );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void bitwiseXor() {
    std::cout << "Bitwise Xor:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL ); // = 1834 << 64 + 1293875340032
    uVeryLongInt b( 232, 2134592384234ULL );
    uVeryLongInt result = a ^ b;
    uVeryLongInt expected( 1986, 952398778346ULL );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void leftShift() {
    std::cout << "Left Shit:" << std::endl;
    uVeryLongInt a( 1834, 1293875340032ULL ); // = 1834 << 64 + 1293875340032
    int b = 43;
    uVeryLongInt result = a << b;
    uVeryLongInt expected( 16132034603346439ULL, 15499138117595561984ULL );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

void rightShift() {
    std::cout << "Right Shit:" << std::endl;
    uVeryLongInt a( 8891287343401834ULL, 1293875340032ULL ); // = 1834 << 64 + 1293875340032
    int b = 43;
    uVeryLongInt result = a >> b;
    uVeryLongInt expected( 1010, 15169520343195844608ULL );
    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected.toString() << std::endl;
        std::cout << "Calculated:\t " << result.toString() << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    addition();
    subtraction();
    multiplication();
    division();
    bitwiseAnd();
    bitwiseOr();
    bitwiseXor();
    leftShift();
    rightShift();
    return 0;
}
