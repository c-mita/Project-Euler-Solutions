#include <array>
#include <cmath>
#include <sstream>
#include "main.h"
#include "peCombinations.h"
#include "pePermutations.h"

enum OPERATOR {
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3
};

struct operatorset_t {
    OPERATOR first;
    OPERATOR second;
    OPERATOR third;
};

std::array<operatorset_t, 64> generateOperatorCombinations() {
    std::array<operatorset_t, 64> operations;
    int index = 0;
    for( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            for ( int k = 0; k < 4; k++ ) {
                operations[index].first = static_cast<OPERATOR>(i);
                operations[index].second = static_cast<OPERATOR>(j);
                operations[index].third = static_cast<OPERATOR>(k);
                index++;
            }
        }
    }
    return operations;
}

double ope( double a, double b, OPERATOR op ) {
    switch ( op ) {
        case ADD:
            return a + b;
        case SUB:
            return a - b;
        case MUL:
            return a * b;
        case DIV:
            return a / b;
    }
    return 0;
}

int calculate( int a1, int a2, int a3, int a4,
               OPERATOR x1, OPERATOR x2, OPERATOR x3,
               int pattern ) {
    double result = 0;
    if ( pattern == 0 ) {
        //ABxCxDx
        result = ope(a1, a2, x1);
        result = ope(result, a3, x2);
        result = ope(result, a4, x3);
    } else if ( pattern == 1 ) {
        //ABxCDxx
        result = ope(a1, a2, x1);
        double s = ope(a3, a4, x2);
        result = ope(result, s, x3);
    }
    if ( result < 0 ) return 0;
    if ( floor(result) == result) return static_cast<int>(result);
    return 0;
}

int calculateScore( bool* valuesReached ) {
    int score = 0;
    //we ignore the first value
    while ( *(++valuesReached) ) score++;
    return score;
}

std::string solution() {
    std::array< operatorset_t, 64 > operationCombinations = generateOperatorCombinations();
    int combination[4] = {1, 2, 3, 4};
    int bestLength = 0;
    int winners[4] = {0};

    do {
        int digits[4] = { combination[0], combination[1], combination[2], combination[3] };
        bool valuesReached[3026] = {false};
        do {
            for ( int i = 0; i < 64; i++ ) {
                int value = calculate( digits[0], digits[1], digits[2], digits[3],
                                       operationCombinations[i].first,
                                       operationCombinations[i].second,
                                       operationCombinations[i].third,
                                       0 );
                valuesReached[value] = true;
                value = calculate( digits[0], digits[1], digits[2], digits[3],
                                   operationCombinations[i].first,
                                   operationCombinations[i].second,
                                   operationCombinations[i].third,
                                   1 );
                valuesReached[value] = true;
            }
        } while ( nextLexicalPermutation<int>( digits, 4 ) );
        int score = calculateScore( valuesReached );
        if ( score > bestLength ) {
            bestLength = score;
            winners[0] = combination[0];
            winners[1] = combination[1];
            winners[2] = combination[2];
            winners[3] = combination[3];
        }
    } while ( nextSeqNumericCombination( combination, 4, 9 ) );

    std::stringstream s;
    s << winners[0] << winners[1] << winners[2] << winners[3];
    return s.str();
}
