#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "student.h"

struct student *student_init(char *name, float gpa) {
    struct student *s = malloc(sizeof(struct student));
    if (s == NULL) {
        perror("Out of memory for student struct.");
        exit(1);
    }
    
    // Make sure that we don't overflow the buffer!
    if (strlen(name) >= MAX_NAME_LEN) {
        strncpy(s->name, name, MAX_NAME_LEN);
        s->name[MAX_NAME_LEN - 1] = '\0';
    } else {
        strcpy(s->name, name);
    }

    s->gpa = gpa;
    return s;
}

void student_print(struct student *s, FILE *ostream) {
    fprintf(ostream, "Name: %s; GPA: %.2f\n", s->name, s->gpa);
}
