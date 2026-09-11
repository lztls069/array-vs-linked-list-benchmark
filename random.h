#pragma once
/* 简易伪随机数（xorshift32）。
   不用 rand() 的原因：Windows 下 RAND_MAX 只有 32767，
   取"随机下标"时会永远落在表的前 32768 个位置，破坏测试意义。
   自己实现还能保证不同编译器下生成的数据与操作序列完全一致。 */
void rng_seed(unsigned int seed);
unsigned int rng_next(void);

/* 返回闭区间 [lo, hi] 内的随机整数 */
int rng_range(int lo, int hi);
