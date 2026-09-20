#define _CRT_SECURE_NO_WARNINGS
#include "seq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 保证容量至少为 need，不够时按 2 倍扩容并搬移已有元素 */
static void seq_ensure_capacity(SeqList* list, int need) {
    int newCapacity;
    Student* newData;
    int i;

    if (need <= list->capacity) {
        return;
    }
    newCapacity = (list->capacity == 0) ? 4 : list->capacity;
    while (newCapacity < need) {
        newCapacity *= 2;
    }

    newData = (Student*)malloc(sizeof(Student) * (size_t)newCapacity);
    if (newData == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        exit(1);
    }
    for (i = 0; i < list->size; ++i) {
        newData[i] = list->data[i];
    }
    free(list->data);
    list->data = newData;
    list->capacity = newCapacity;
}

void seq_init(SeqList* list) {
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void seq_destroy(SeqList* list) {
    free(list->data);
    list->data = NULL;
    list->size = 0;
    list->capacity = 0;
}

void seq_build(SeqList* list, const Student* data, int n) {
    int i;
    seq_destroy(list);  /* 先清空，支持重复建表 */
    if (n <= 0) {
        return;
    }
    list->data = (Student*)malloc(sizeof(Student) * (size_t)n);
    if (list->data == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        exit(1);
    }
    list->capacity = n;
    list->size = n;
    for (i = 0; i < n; ++i) {
        list->data[i] = data[i];  /* 整体拷贝，O(n) */
    }
}

int seq_find_by_name(const SeqList* list, const char* name) {
    int i;
    for (i = 0; i < list->size; ++i) {
        if (strcmp(list->data[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int seq_insert_at(SeqList* list, int pos, const Student* stu) {
    int i;
    if (pos < 0 || pos > list->size) {
        return 0;
    }
    seq_ensure_capacity(list, list->size + 1);
    for (i = list->size; i > pos; --i) {  /* 从后往前搬移，给 pos 空出位置 */
        list->data[i] = list->data[i - 1];
    }
    list->data[pos] = *stu;
    ++list->size;
    return 1;
}

int seq_remove_at(SeqList* list, int pos) {
    int i;
    if (pos < 0 || pos >= list->size) {
        return 0;
    }
    for (i = pos; i < list->size - 1; ++i) {  /* 后续元素整体前移一位 */
        list->data[i] = list->data[i + 1];
    }
    --list->size;
    return 1;
}

int seq_size(const SeqList* list) {
    return list->size;
}

/* ---- 统一接口适配层：把具体函数装进函数指针表 ---- */

static void seq_ops_build(void* self, const Student* data, int n) {
    seq_build((SeqList*)self, data, n);
}

static int seq_ops_find(void* self, const char* name) {
    return seq_find_by_name((const SeqList*)self, name);
}

static int seq_ops_insert(void* self, int pos, const Student* stu) {
    return seq_insert_at((SeqList*)self, pos, stu);
}

static int seq_ops_remove(void* self, int pos) {
    return seq_remove_at((SeqList*)self, pos);
}

static int seq_ops_size(void* self) {
    return seq_size((const SeqList*)self);
}

static void seq_ops_destroy(void* self) {
    seq_destroy((SeqList*)self);
}

const ListOps* seq_list_ops(void) {
    static const ListOps ops = {
        seq_ops_build,
        seq_ops_find,
        seq_ops_insert,
        seq_ops_remove,
        seq_ops_size,
        seq_ops_destroy
    };
    return &ops;
}