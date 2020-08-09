#include <tuple>
#include <math.h>
#include <stdexcept>
#include <gmpxx.h>
#include "main.h"

/*
 * http://www.isibang.ac.in/~sury/chakravala.pdf
 */

typedef mpz_class bigint_t;

std::tuple<bigint_t, bigint_t>
chak_method(int n) {
    bigint_t p, q;
    bigint_t p2, q2;
    int m, x;
    double sq_n = sqrt(n);
    if (((int) sq_n) * ((int) sq_n) == n) {
        throw std::domain_error("No solution for perfect squares");
    }
    p = (int) sq_n;
    q = 1;
    m = (int) sq_n * (int) sq_n - n;
    x = (int) sq_n;

    while (m != 1) {
        int abs_m = m > 0 ? m : -m;
        int abs_x = x > 0 ? x : -x;
        x = (abs_m - abs_x) % abs_m;
        while (x + abs_m < sq_n) x += abs_m;

        p2 = (p*x + n*q) / abs_m;
        q2 = (p + q*x) / abs_m;
        m = (x*x - n) / m;
        p = p2;
        q = q2;
    }

    return std::make_tuple(p, q);
}

std::string solution() {
    int limit = 1000;
    bigint_t max_x = 0;
    int max_i = 0;
    for (int i = 2; i < limit; i++) {
        int rt = (int) sqrt(i);
        if (rt * rt == i) continue;
        std::tuple<bigint_t, bigint_t> res = chak_method(i);
        bigint_t x  = std::get<0>(res);
        if (x > max_x) {
            max_x = x;
            max_i = i;
        }
    }
    return std::to_string(max_i);
}
