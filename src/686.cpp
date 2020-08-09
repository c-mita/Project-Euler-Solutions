#include <cmath>
#include <vector>
#include "main.h"

const double LOG10_2 = std::log10(2);
const double LOG10_123 = std::log10(123);
const double LOG10_124 = std::log10(124);

/**
 * Given p(L, n) = nth smallest j s.t. 2^j starts with the digits of L
 * calculate p(123, 678910)
 *
 * For a number 2^j with base10 digit count d = floor(j*log10(2) + 1)
 * we can find its leading 3 digits by finding k s.t.
 * k*10^(d-3) < 2^j < (k+1)*10^(d-3)
 * or:
 * log10(k) + d-3 < j*log10(2) < log10(k+1) + d-3
 *
 * Since we know k = 123 and that there can be at most one j s.t.
 * 2^j starts with 123 for a given digit count, we just need to check if
 * the bounds of the j*log10(2) cross an integer for a given d
 * i.e:
 * floor(log10(123) + d-3) != floor(log10(124) + d-3)
 */

std::string solution() {
    std::vector<int> powers;
    int d = 4;
    while (powers.size() < 678910) {
        double v1 = (LOG10_123 + d - 3) / LOG10_2;
        double v2 = (LOG10_124 + d - 3) / LOG10_2;
        if (std::floor(v1) != std::floor(v2)) {
            powers.push_back(std::floor(v2));
        }
        d++;
    }
    return std::to_string(powers.back());
}
