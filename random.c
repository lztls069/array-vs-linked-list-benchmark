#include "random.h"

static unsigned int g_state = 1u;

void rng_seed(unsigned int seed) {
    g_state = (seed == 0u) ? 1u : seed;  /* xorshift 的状态不能为 0 */
}

unsigned int rng_next(void) {
    unsigned int x = g_state;
    x ^= x << 13;//左移13位后按位异或
    x ^= x >> 17;
    x ^= x << 5;
    g_state = x;
    return x;
}//设置下个种子

int rng_range(int lo, int hi) {
    unsigned int span = (unsigned int)(hi - lo + 1);//范围：（15， 0-25）
    return lo + (int)(rng_next() % span);
}