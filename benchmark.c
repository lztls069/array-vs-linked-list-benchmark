#if !defined(_WIN32)
#define _POSIX_C_SOURCE 199309L
#endif

#include <stdio.h>
#include <stdlib.h>

#include "random.h"
#include "benchmark.h"

#include <windows.h>

static double now_seconds(void) {
    static LARGE_INTEGER freq;
    static int inited = 0;
    LARGE_INTEGER counter;

    if (!inited) {
        QueryPerformanceFrequency(&freq);
        inited = 1;
    }
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)freq.QuadPart;
}

void run_benchmark(const ListOps* ops, void* list,
    const Student* data, int n,
    int queryCount, int insertCount, int deleteCount,
    unsigned int seed, BenchResult* out) {
    int* queryIdx = NULL;
    int* insertPos = NULL;
    int* deletePos = NULL;
    int i;
    int k;
    int sizeAfterInsert;
    int currentSize;
    double t0;
    double t1;
    volatile double sink = 0.0;  /* 汇总结果，防止编译器把测试代码优化掉 */
    Student stu;

    rng_seed(seed);

    /* ---- 第一步：先固定好整个操作序列，数组与链表使用完全相同的序列 ---- */

    if (n > 0 && queryCount > 0) {
        queryIdx = (int*)malloc(sizeof(int) * (size_t)queryCount);
        if (queryIdx == NULL) {
            fprintf(stderr, "ERROR: out of memory\n");
            exit(1);
        }
        for (i = 0; i < queryCount; ++i) {
            queryIdx[i] = rng_range(0, n - 1);  /* 抽取要查询的记录下标 */
        }
    }
    else {
        queryCount = 0;
    }

    if (insertCount > 0) {
        insertPos = (int*)malloc(sizeof(int) * (size_t)insertCount);
        if (insertPos == NULL) {
            fprintf(stderr, "ERROR: out of memory\n");
            exit(1);
        }
        for (k = 0; k < insertCount; ++k) {
            insertPos[k] = rng_range(0, n + k);  /* 第 k 次插入时表长为 n + k */
        }
    }

    /* 插入完成后表长为 n + insertCount，第 k 次删除时表长为 n + insertCount - k */
    sizeAfterInsert = n + insertCount;
    if (deleteCount > 0) {
        deletePos = (int*)malloc(sizeof(int) * (size_t)deleteCount);
        if (deletePos == NULL) {
            fprintf(stderr, "ERROR: out of memory\n");
            exit(1);
        }
        for (k = 0; k < deleteCount; ++k) {
            currentSize = sizeAfterInsert - k;
            deletePos[k] = rng_range(0, currentSize - 1);
        }
    }

    /* ---- 第二步：建表计时 ---- */
    t0 = now_seconds();
    ops->build(list, data, n);
    t1 = now_seconds();
    out->createUs = (t1 - t0) * 1e6;

    /* ---- 第三步：查询计时（按姓名查，求单次平均） ---- */
    t0 = now_seconds();
    for (i = 0; i < queryCount; ++i) {
        sink += (double)ops->find_by_name(list, data[queryIdx[i]].name);
    }
    t1 = now_seconds();
    out->queryAvgUs = (queryCount > 0) ? (t1 - t0) * 1e6 / (double)queryCount : 0.0;

    /* ---- 第四步：插入计时（随机位置插入，求单次平均） ---- */
    t0 = now_seconds();
    for (i = 0; i < insertCount; ++i) {
        student_set(&stu, (i % 2 == 0) ? "test_insert_even" : "test_insert_odd", 20, 90);
        if (!ops->insert_at(list, insertPos[i], &stu)) {
            sink -= 1.0;
        }
    }
    t1 = now_seconds();
    out->insertAvgUs = (insertCount > 0) ? (t1 - t0) * 1e6 / (double)insertCount : 0.0;

    /* ---- 第五步：删除计时（随机位置删除，求单次平均） ---- */
    t0 = now_seconds();
    for (i = 0; i < deleteCount; ++i) {
        if (!ops->remove_at(list, deletePos[i])) {
            sink -= 1.0;
        }
    }
    t1 = now_seconds();
    out->deleteAvgUs = (deleteCount > 0) ? (t1 - t0) * 1e6 / (double)deleteCount : 0.0;

    (void)sink;
    free(queryIdx);
    free(insertPos);
    free(deletePos);
}