#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

struct student {
    char *name;
    float gpa;
};

struct student *student_init(char *name, float gpa);
void student_print(struct student *s, FILE *ostream);

#endif // STUDENT_H