#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 返回第 index 个数据结点；index 为 -1 时返回哑头结点。
   调用者需保证 index 在 [-1, size-1] 范围内。 */
static Node* linked_node_at(LinkedList* list, int index) {
    Node* cur = list->head;
    int i;
    for (i = -1; i < index; ++i) {
        cur = cur->next;
    }
    return cur;
}

/* 释放除哑头结点外的所有结点，并把表清空 */
static void linked_clear(LinkedList* list) {
    Node* cur = list->head->next;
    while (cur != NULL) {
        Node* next = cur->next;
        free(cur);
        cur = next;
    }
    list->head->next = NULL;
    list->tail = list->head;
    list->size = 0;
}

void linked_init(LinkedList* list) {
    list->head = (Node*)malloc(sizeof(Node));
    if (list->head == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        exit(1);
    }
    list->head->next = NULL;
    list->tail = list->head;
    list->size = 0;
}

void linked_destroy(LinkedList* list) {
    if (list->head == NULL) {
        return;
    }
    linked_clear(list);
    free(list->head);
    list->head = NULL;
    list->tail = NULL;
}

void linked_build(LinkedList* list, const Student* data, int n) {
    int i;
    linked_clear(list);
    /* 尾插建表：每个结点一次 malloc，配合尾指针整体为 O(n) */
    for (i = 0; i < n; ++i) {
        Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL) {
            fprintf(stderr, "ERROR: out of memory\n");
            exit(1);
        }
        node->data = data[i];
        node->next = NULL;
        list->tail->next = node;
        list->tail = node;
        ++list->size;
    }
}

int linked_find_by_name(const LinkedList* list, const char* name) {
    const Node* cur = list->head->next;
    int index = 0;
    while (cur != NULL) {
        if (strcmp(cur->data.name, name) == 0) {
            return index;
        }
        cur = cur->next;
        ++index;
    }
    return -1;
}

int linked_insert_at(LinkedList* list, int pos, const Student* stu) {
    Node* prev;
    Node* node;

    if (pos < 0 || pos > list->size) {
        return 0;
    }

    /* pos == 0 时 linked_node_at(-1) 返回哑头结点，不需要单独处理表头 */
    prev = linked_node_at(list, pos - 1);
    node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "ERROR: out of memory\n");
        exit(1);
    }
    node->data = *stu;
    node->next = prev->next;
    prev->next = node;

    if (node->next == NULL) {  /* 新结点挂在表尾，更新尾指针 */
        list->tail = node;
    }
    ++list->size;
    return 1;
}

int linked_remove_at(LinkedList* list, int pos) {
    Node* prev;
    Node* target;

    if (pos < 0 || pos >= list->size) {
        return 0;
    }

    prev = linked_node_at(list, pos - 1);
    target = prev->next;
    prev->next = target->next;

    if (target == list->tail) {  /* 删掉的是尾结点，尾指针回退 */
        list->tail = prev;
    }
    free(target);
    --list->size;
    return 1;
}

int linked_size(const LinkedList* list) {
    return list->size;
}

/* ---- 统一接口适配层：把具体函数装进函数指针表 ---- */

static void linked_ops_build(void* self, const Student* data, int n) {
    linked_build((LinkedList*)self, data, n);
}

static int linked_ops_find(void* self, const char* name) {
    return linked_find_by_name((const LinkedList*)self, name);
}

static int linked_ops_insert(void* self, int pos, const Student* stu) {
    return linked_insert_at((LinkedList*)self, pos, stu);
}

static int linked_ops_remove(void* self, int pos) {
    return linked_remove_at((LinkedList*)self, pos);
}

static int linked_ops_size(void* self) {
    return linked_size((const LinkedList*)self);
}

static void linked_ops_destroy(void* self) {
    linked_destroy((LinkedList*)self);
}

const ListOps* linked_list_ops(void) {
    static const ListOps ops = {
        linked_ops_build,
        linked_ops_find,
        linked_ops_insert,
        linked_ops_remove,
        linked_ops_size,
        linked_ops_destroy
    };
    return &ops;
}