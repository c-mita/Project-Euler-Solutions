#include "main.h"

/*
 * Consider only "decreasing" numbers.
 * dx_n is the number of ways to write an n-digit number that starts
 * with the digit "x".
 * dx_1 == 1 for all x
 *
 * We can use previous answers to build up data for the full answer
 * e.g.
 * d5_3 = d4_3 + d5_2
 * That is - the number of ways to write a 3 digit decreasing number
 * beginning with 5 is the same as the number of ways to write a 3 digit
 * number beginning with 4 plus the ways to write a 2 digit number starting
 * with 5.
 *
 * The sum of "all" decreasing n-digit numbers is simply the number of ways
 * to write an "n+1"-digit number starting with 9 (minus 1 because we don't
 * count zero).
 *
 * We just sum up all ways of writing 1, 2, ..., n+1 digit numbers starting
 * with 9
 *
 * Increasing is the same, but we have fewer digits (only 9 symbols since we
 * can't "lead" with zero). We can use the same dynamic programming array.
 *
 * Adding both increasing and decreasing sums will double count runs of the same
 * digit (111, 22222, etc).
 *
 * There is a combinatoric approach, but the calculations required are
 * actually the same (and the data in the array corresponds to diagonals in
 * Pascal's triangle).
 * We could reduce the size of the array though; we only really need to remember
 * two rows of it.
 *
 * Runtime < 1ms
 */

#define LIMIT 100

typedef long long int intl;

static intl DECR[LIMIT+1][10];

void init_data() {
    for (int c = 0; c < 10; c++) {
        DECR[0][c] = 1;
    }
    for (int r = 1; r < LIMIT+1; r++) {
        DECR[r][0] = 1;
        for (int c = 1; c < 10; c++) {
            DECR[r][c] = DECR[r-1][c] + DECR[r][c-1];
        }
    }
}

std::string solution() {
    init_data();

    intl count = 0;
    intl increasing = 0;
    intl decreasing = 0;
    for (int i = 0; i < LIMIT+1; i++) {
        decreasing += DECR[i][9];
        increasing += DECR[i][8];
    }
    // we've counted 0 as a number in both count so we need to remove that
    decreasing -= 1;
    increasing -= 1;
    count = increasing + decreasing - LIMIT * 10;

    return std::to_string(count);
}
