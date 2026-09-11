#pragma once

#include "linear_list.h"
#include "student.h"

/* 一种存储结构在某一数据规模下的测试结果，时间单位统一为微秒(us) */
typedef struct {
    double createUs;     /* 建表总耗时 */
    double queryAvgUs;   /* 单次查询平均耗时 */
    double insertAvgUs;  /* 单次插入平均耗时 */
    double deleteAvgUs;  /* 单次删除平均耗时 */
} BenchResult;

/* 在给定数据集上测试一种线性表：建表 -> 查询 -> 插入 -> 删除，分别计时。
   list 是具体结构的对象指针（SeqList* / LinkedList*），
   ops 是它对应的操作表，二者必须匹配。 */
void run_benchmark(const ListOps* ops, void* list,
    const Student* data, int n,
    int queryCount, int insertCount, int deleteCount,
    unsigned int seed, BenchResult* out);
