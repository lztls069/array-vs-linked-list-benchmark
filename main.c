#include <stdio.h>
#include <stdlib.h>

#include "benchmark.h"
#include "data.h"
#include "linear_list.h"
#include "linked_list.h"
#include "seq.h"
#include "student.h"

/* 每个数据规模下的测试参数：
     scale       数据规模
     queryCount  查询次数（用于计算平均查询时间）
     insertCount 插入次数（用于计算平均插入时间）
     deleteCount 删除次数（用于计算平均删除时间）
     rounds      重复轮数，最终结果取多轮平均 */
typedef struct {
    int scale;
    int queryCount;
    int insertCount;
    int deleteCount;
    int rounds;
} ScaleConfig;

static const ScaleConfig kConfigs[] = {
    {100,       2000, 200, 200, 5},
    {1000,      1000, 200, 200, 5},
    {10000,      500, 100, 100, 3},
    {100000,     100,  50,  50, 2},
    {1000000,     10,  20,  20, 1}
};

/* 一种结构在某一规模下的平均结果 */
typedef struct {
    double createUs;
    double queryAvgUs;
    double insertAvgUs;
    double deleteAvgUs;
} AvgResult;

/* 对一种结构重复若干轮测试并取平均 */
static AvgResult measure(const ListOps* ops, void* list, const Student* data,
    const ScaleConfig* cfg, unsigned int baseSeed) {
    AvgResult avg;
    int r;

    avg.createUs = 0.0;
    avg.queryAvgUs = 0.0;
    avg.insertAvgUs = 0.0;
    avg.deleteAvgUs = 0.0;

    for (r = 0; r < cfg->rounds; ++r) {
        BenchResult one;
        run_benchmark(ops, list, data, cfg->scale,
            cfg->queryCount, cfg->insertCount, cfg->deleteCount,
            baseSeed + (unsigned int)r * 131u, &one);
        avg.createUs += one.createUs;
        avg.queryAvgUs += one.queryAvgUs;
        avg.insertAvgUs += one.insertAvgUs;
        avg.deleteAvgUs += one.deleteAvgUs;
    }

    avg.createUs /= cfg->rounds;
    avg.queryAvgUs /= cfg->rounds;
    avg.insertAvgUs /= cfg->rounds;
    avg.deleteAvgUs /= cfg->rounds;
    return avg;
}

static void print_table_header(void) {
    printf("%10s | %8s | %14s | %14s | %14s | %14s\n",
        "Scale", "Struct", "Create(us)", "Query(us)", "Insert(us)", "Delete(us)");
    printf("-----------|----------|----------------|----------------|----------------|----------------\n");
}

static void print_row(int scale, const char* structure, const AvgResult* r) {
    printf("%10d | %8s | %14.2f | %14.2f | %14.2f | %14.2f\n",
        scale, structure, r->createUs, r->queryAvgUs, r->insertAvgUs, r->deleteAvgUs);
}

int main(void) {
    int i;
    const int configCount = (int)(sizeof(kConfigs) / sizeof(kConfigs[0]));

    print_table_header();

    for (i = 0; i < configCount; ++i) {
        const ScaleConfig* cfg = &kConfigs[i];
        Student* data;
        SeqList seq;
        LinkedList linked;
        AvgResult seqAvg;
        AvgResult linkedAvg;

        fprintf(stderr, "[progress] scale = %d ...\n", cfg->scale);

        /* 同一规模只生成一份数据，数组和链表共用，保证输入完全一致 */
        data = (Student*)malloc(sizeof(Student) * (size_t)cfg->scale);
        if (data == NULL) {
            fprintf(stderr, "ERROR: out of memory\n");
            return 1;
        }
        generate_students(data, cfg->scale, 20240909u);

        /* 两种结构使用相同种子，操作序列完全一致 */
        seq_init(&seq);
        seqAvg = measure(seq_list_ops(), &seq, data, cfg, 1000u);
        seq_destroy(&seq);

        linked_init(&linked);
        linkedAvg = measure(linked_list_ops(), &linked, data, cfg, 1000u);
        linked_destroy(&linked);

        print_row(cfg->scale, "Array", &seqAvg);
        print_row(cfg->scale, "Linked", &linkedAvg);
        fflush(stdout);

        free(data);
    }

    fprintf(stderr, "[progress] done.\n");
    return 0;
}