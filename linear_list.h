#pragma once
#include "student.h"

/* 线性表统一操作接口。
   C 语言没有虚函数，这里用"函数指针表"达到同样效果：
   顺序表和链表各自提供一张表，测试模块只通过这张表调用，
   因此两种存储结构被测的操作序列完全相同。
   （返回 1 表示成功，0 表示位置越界失败。） */
typedef struct {
    void (*build)(void* self, const Student* data, int n);//建表
    int  (*find_by_name)(void* self, const char* name);//按姓名查
    int  (*insert_at)(void* self, int pos, const Student* stu); //在指定位置插入
    int  (*remove_at)(void* self, int pos);//删除指定位置
    int  (*size)(void* self);//返回元素个数
    void (*destroy)(void* self);//释放内存
} ListOps;