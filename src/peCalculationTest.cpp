#include <iostream>
#include "peCalculation.h"
#include "peLargeInts.h"

typedef unsigned long long int uLongInt;

void ModularExpTest() {
    std::cout << "ModExp test:" << std::endl;
    uLongInt base = 384721332245ULL;
    uLongInt exp = 3423433464232ULL;
    uLongInt mod = 983827445ULL;

    uLongInt expected = 953686460;
    uLongInt result = ModularExp<uLongInt>( base, exp, mod );

    bool pass = result == expected;
    if ( pass ) {
        std::cout << "Passed" << std::endl;
    } else {
        std::cout << "Failed:" << std::endl;
        std::cout << "Expected:\t " << expected << std::endl;
        std::cout << "Calculated:\t " << result << std::endl;
    }
    std::cout << std::endl;

}

int main() {
    ModularExpTest();
    return 0;
}
