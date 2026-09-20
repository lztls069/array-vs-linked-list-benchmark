#pragma once
#define NAME_MAX 16


typedef struct student {
	char name[NAME_MAX];//4字母
	int age;
	int score;
}Student;

void student_set(Student* s, const char* name, int age, int score);