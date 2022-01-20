#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "student.h"

struct student *student_init(char *name, float gpa) {
    // TODO write the code to create and return a student struct.
    // Make sure to dynamically allocate the name field in the 
    // student struct!
}

void student_print(struct student *s, FILE *ostream) {
    fprintf(ostream, "Name: %s; GPA: %.2f\n", s->name, s->gpa);
}
