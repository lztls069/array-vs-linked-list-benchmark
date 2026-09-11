#include "data.h"
#include "random.h"
#include <stdio.h>

static const char kLetters[] = "abcdefghijklmnopqrstuvwxyz";

void generate_students(Student* out, int n, unsigned int seed) {
	int i;
	char buffer[NAME_MAX];
	rng_seed(seed);
    for (i = 0; i < n; i++) {
        // 4 个随机小写字母 + 6 位序号
        snprintf(buffer, sizeof(buffer), "%c%c%c%c%06d",
            kLetters[rng_range(0, 25)],
            kLetters[rng_range(0, 25)],
            kLetters[rng_range(0, 25)],
            kLetters[rng_range(0, 25)],i);
        student_set(&out[i], buffer, rng_range(15, 40), rng_range(0, 100));
    }
}