#include "util.h"

int intPow(int x, int n) {
    if (x == 2)
        return 1 << n;
    int res = 1;
    while(1) {
        if (n % 2 == 1)
            res *= x;
        n >>= 1;
        if (!n)
            break;
        x *= x;
    }
    return res;
}