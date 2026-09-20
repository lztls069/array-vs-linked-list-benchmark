#pragma once
#include "student.h"
#include "linear_list.h"
/* 顺序表（数组存储结构）：底层为动态数组，容量不足时按 2 倍扩容 */
typedef struct {
    Student* data;  /* 动态数组 */
    int size;       /* 当前元素个数 */
    int capacity;   /* 当前容量 */
} SeqList;

void seq_init(SeqList* list);
void seq_destroy(SeqList* list);
void seq_build(SeqList* list, const Student* data, int n);
int  seq_find_by_name(const SeqList* list, const char* name);
int  seq_insert_at(SeqList* list, int pos, const Student* stu);
int  seq_remove_at(SeqList* list, int pos);
int  seq_size(const SeqList* list);

/* 返回顺序表的统一操作表 */
const ListOps* seq_list_ops(void);