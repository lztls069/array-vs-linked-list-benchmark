#define _CRT_SECURE_NO_WARNINGS
#include "student.h"

void student_set(Student* s, const char* name, int age, int score) {
	strncpy(s->name, name, NAME_MAX - 1);
	s->name[NAME_MAX - 1] = '\0'; // Ensure null-termination
	s->age = age;
	s->score = score;
}	