#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define MAX_NAME_LEN 128

struct student {
    char name[MAX_NAME_LEN];
    float gpa;
};

struct student *student_init(char *name, float gpa);

void student_print(struct student *s, FILE *ostream);

#endif // STUDENT_H
