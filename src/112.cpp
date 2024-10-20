#include "main.h"

/*
 * Just simply check for bounciness for each integer, until the
 * proportion reaches the desired threshold.
 *
 * Runtime < 50 ms
 */

bool is_bouncy(int n) {
    bool decreases = false;
    bool increases = false;
    int last = n % 10;
    n /= 10;
    while (n > 0) {
        int r = n % 10;
        if (r > last) decreases = true;
        if (r < last) increases = true;
        last = r;
        n /= 10;
    }
    return decreases && increases;
}

std::string solution() {
    int n = 0;
    int bouncy = 0;
    do {
      n += 1;
      if (is_bouncy(n)) {
          bouncy += 1;
      }
    // b * 100 < 99 * n <==> b < 99/100 * n
    } while (bouncy * 100 < 99 * n);

    return std::to_string(n);
}
