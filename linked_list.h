#pragma once

#include "linear_list.h"

/* 单链表结点 */
typedef struct Node {
    Student data;
    struct Node* next;
} Node;

/* 带头结点（哑结点）的单链表。
   哑结点的作用：让"在 0 号位置插入/删除"和中间位置的处理方式完全一致。 */
typedef struct {
    Node* head;  /* 哑头结点，始终存在 */
    Node* tail;  /* 尾指针，使建表（尾插）整体保持 O(n) */
    int size;    /* 当前元素个数 */
} LinkedList;

void linked_init(LinkedList* list);
void linked_destroy(LinkedList* list);
void linked_build(LinkedList* list, const Student* data, int n);
int  linked_find_by_name(const LinkedList* list, const char* name);
int  linked_insert_at(LinkedList* list, int pos, const Student* stu);
int  linked_remove_at(LinkedList* list, int pos);
int  linked_size(const LinkedList* list);

/* 返回链表的统一操作表 */
const ListOps* linked_list_ops(void);
