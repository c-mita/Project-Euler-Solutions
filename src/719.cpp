#include <vector>

#include "main.h"

/***
 * An "S" number is a pefect square whose square root is equal to the sum
 * of some split of the original number.
 *
 * e.g.9801 is an S number because sqrt(9081) == 98 + 0 + 1 == 99
 * Find the total number of S numbers below 1 trillion.
 */


/**
 * We consider n^2 for n <= 1 million.
 * For each one, we just check the ways of splitting them and check the sums.
 * There are only 2048 ways of splitting up a 12 digit number; so only about a
 * billion checks in all.
 * We need the splitting/summing to be efficient though.
 *
 * Brute force but solves in ~7 seconds on a laptop.
 */

void split_sums(long long value, std::vector<long long> *sums, long long current_sum, int run_length) {
    // Split a digit and calculate the sums.
    // Recursive; starting from the right and branch on whether we split at the
    // current position or not.
    int d = value % 10;
    long long t = d;
    // if we haven't split earlier, then the current digit contributes more to
    // the sum
    for (int i = 0; i < run_length; i++) {
        t *= 10;
    }
    long long sum = current_sum + t;
    if (value / 10 == 0) {
        sums->push_back(sum);
        return;
    }
    // split at the current position
    split_sums(value / 10, sums, sum, 0);
    // don't split
    split_sums(value / 10, sums, sum, run_length + 1);
}

std::string solution() {
    int limit = 1000000;
    long long sum = 0;

    for (long long n = 2; n <= limit; n++) {
        std::vector<long long> sums;
        split_sums(n*n, &sums, 0, 0);
        for (long long s : sums) {
            if (s == n) {
                sum += n * n;
                break;
            }
        }
    }
    return std::to_string(sum);
}
